/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_ALGORITHMS_UPDATE_CURVATURE_H
#define VCL_ALGORITHMS_UPDATE_CURVATURE_H

#include <mutex>

#include <vclib/algorithms/intersection.h>
#include <vclib/algorithms/core/polygon.h>
#include <vclib/algorithms/mesh/point_sampling.h>
#include <vclib/algorithms/mesh/stat.h>
#include <vclib/algorithms/update/normal.h>
#include <vclib/math/transform.h>
#include <vclib/mesh/requirements.h>
#include <vclib/mesh/utils/mesh_pos.h>
#include <vclib/misc/logger.h>
#include <vclib/misc/parallel.h>
#include <vclib/space/grid.h>
#include <vclib/space/principal_curvature.h>
#include <vclib/views.h>

namespace vcl {

typedef enum {
    VCL_PRINCIPAL_CURVATURE_TAUBIN95,
    VCL_PRINCIPAL_CURVATURE_PCA
} VCLibPrincipalCurvatureAlgorithm;

template<FaceMeshConcept MeshType, LoggerConcept LogType = NullLogger>
void updatePrincipalCurvatureTaubin95(MeshType& m, LogType& log)
{
    vcl::requirePerVertexPrincipalCurvature(m);
    vcl::requirePerVertexAdjacentFaces(m);
    vcl::requirePerFaceAdjacentFaces(m);

    using VertexType = MeshType::VertexType;
    using CoordType  = VertexType::CoordType;
    using NormalType = VertexType::NormalType;
    using ScalarType = CoordType::ScalarType;
    using FaceType   = MeshType::FaceType;

    // Auxiliary data structure
    struct AdjVertex
    {
        const VertexType* vert;
        double            doubleArea;
        bool              isBorder;
    };

    log.log(0, "Updating per vertex normals...");

    vcl::updatePerVertexNormalsAngleWeighted(m);
    vcl::normalizePerVertexNormals(m);

    log.log(5, "Computing per vertex curvature...");
    // log every 5%, starting from 5% to 100%
    log.startProgress("", m.vertexNumber(), 5, 5, 100);

    for (VertexType& v : m.vertices()) {
        std::vector<ScalarType> weights;
        std::vector<AdjVertex>  vertices;

        vcl::MeshPos<FaceType> pos(v.adjFace(0), &v);
        const VertexType*      firstVertex = pos.adjVertex();
        const VertexType*      tmpVertex;
        float                  totalDoubleAreaSize = 0;

        // compute the area of each triangle around the central vertex as well
        // as their total area
        do {
            pos.nextEdgeAdjacentToV();
            tmpVertex = pos.adjVertex();
            AdjVertex adjV;
            adjV.isBorder   = pos.isEdgeOnBorder();
            adjV.vert       = tmpVertex;
            adjV.doubleArea = vcl::faceArea(*pos.face()) * 2;
            totalDoubleAreaSize += adjV.doubleArea;
            vertices.push_back(adjV);
        } while (tmpVertex != firstVertex);

        weights.reserve(vertices.size());

        for (int i = 0; i < vertices.size(); ++i) {
            if (vertices[i].isBorder) {
                weights.push_back(vertices[i].doubleArea / totalDoubleAreaSize);
            }
            else {
                weights.push_back(
                    0.5f *
                    (vertices[i].doubleArea +
                     vertices[(i - 1) % vertices.size()].doubleArea) /
                    totalDoubleAreaSize);
            }
            assert(weights.back() < 1.0f);
        }

        // compute I-NN^t to be used for computing the T_i's
        Matrix33<ScalarType> Tp;

        NormalType n = v.normal();
        for (int i = 0; i < 3; ++i)
            Tp(i, i) = 1.0f - std::pow(n[i], 2);
        Tp(0, 1) = Tp(1, 0) = -1.0f * (n[0] * n[1]);
        Tp(1, 2) = Tp(2, 1) = -1.0f * (n[1] * n[2]);
        Tp(0, 2) = Tp(2, 0) = -1.0f * (n[0] * n[2]);

        // for all neighbors vi compute the directional curvatures k_i and the
        // T_i compute M by summing all w_i k_i T_i T_i^t
        Matrix33<ScalarType> tempMatrix;
        Matrix33<ScalarType> M = Matrix33<ScalarType>::Zero();
        for (size_t i = 0; i < vertices.size(); ++i) {
            CoordType edge = (v.coord() - vertices[i].vert->coord());
            float     curvature =
                (2.0f * (v.normal().dot(edge))) / edge.squaredNorm();
            CoordType t = Tp * edge;
            t.normalize();
            tempMatrix = t.outerProduct(t);
            M += tempMatrix * weights[i] * curvature;
        }

        // compute vector W for the Householder matrix
        CoordType w;
        CoordType e1(1.0f, 0.0f, 0.0f);
        if ((e1 - v.normal()).squaredNorm() > (e1 + v.normal()).squaredNorm())
            w = e1 - v.normal();
        else
            w = e1 + v.normal();
        w.normalize();

        // compute the Householder matrix I - 2WW^t
        Matrix33<ScalarType> Q = Matrix33<ScalarType>::Identity();
        tempMatrix             = w.outerProduct(w);
        Q -= tempMatrix * 2.0f;

        // compute matrix Q^t M Q
        Matrix33<ScalarType> QtMQ = (Q.transpose() * M * Q);

        Eigen::Matrix<ScalarType, 1, 3> T1 = Q.col(1);
        Eigen::Matrix<ScalarType, 1, 3> T2 = Q.col(2);

        // find sin and cos for the Givens rotation
        ScalarType s, c;
        // Gabriel Taubin hint and Valentino Fiorin impementation
        ScalarType alpha = QtMQ(1, 1) - QtMQ(2, 2);
        ScalarType beta  = QtMQ(2, 1);

        ScalarType h[2];
        ScalarType delta =
            std::sqrt(4.0f * std::pow(alpha, 2) + 16.0f * std::pow(beta, 2));
        h[0] = (2.0f * alpha + delta) / (2.0f * beta);
        h[1] = (2.0f * alpha - delta) / (2.0f * beta);

        ScalarType t[2];
        ScalarType bestC, bestS;
        ScalarType minError = std::numeric_limits<ScalarType>::infinity();
        for (uint i = 0; i < 2; i++) {
            delta = std::sqrt(std::pow(h[i], 2) + 4.0f);
            t[0]  = (h[i] + delta) / 2.0f;
            t[1]  = (h[i] - delta) / 2.0f;

            for (uint j = 0; j < 2; j++) {
                ScalarType squaredT    = std::pow(t[j], 2);
                ScalarType denominator = 1.0f + squaredT;

                s = (2.0f * t[j]) / denominator;
                c = (1 - squaredT) / denominator;

                ScalarType approximation =
                    c * s * alpha + (std::pow(c, 2) - std::pow(s, 2)) * beta;
                ScalarType angleSimilarity =
                    std::abs(std::acos(c) / std::asin(s));
                ScalarType error =
                    std::abs(1.0f - angleSimilarity) + std::abs(approximation);
                if (error < minError) {
                    minError = error;
                    bestC    = c;
                    bestS    = s;
                }
            }
        }
        c = bestC;
        s = bestS;

        Eigen::Matrix2f minor2x2;
        Eigen::Matrix2f S;

        // diagonalize M
        minor2x2(0, 0) = QtMQ(1, 1);
        minor2x2(0, 1) = QtMQ(1, 2);
        minor2x2(1, 0) = QtMQ(2, 1);
        minor2x2(1, 1) = QtMQ(2, 2);

        S(0, 0) = S(1, 1) = c;
        S(0, 1)           = s;
        S(1, 0)           = -1.0f * s;

        Eigen::Matrix2f StMS = S.transpose() * minor2x2 * S;

        // compute curvatures and curvature directions
        ScalarType principalCurv1 = (3.0f * StMS(0, 0)) - StMS(1, 1);
        ScalarType principalCurv2 = (3.0f * StMS(1, 1)) - StMS(0, 0);

        Eigen::Matrix<ScalarType, 1, 3> principalDir1 = T1 * c - T2 * s;
        Eigen::Matrix<ScalarType, 1, 3> principalDir2 = T1 * s + T2 * c;

        v.principalCurvature().maxDir()   = principalDir1;
        v.principalCurvature().minDir()   = principalDir2;
        v.principalCurvature().maxValue() = principalCurv1;
        v.principalCurvature().minValue() = principalCurv2;

        log.progress(m.index(v));
    }

    log.endProgress();
    log.log(100, "Per vertex curvature computed.");
}

/**
 * @brief Computes the Principal Curvature meseaure as described in the paper:
 * Robust principal curvatures on Multiple Scales, Yong-Liang Yang, Yu-Kun Lai,
 * Shi-Min Hu Helmut Pottmann SGP 2004. If montecarloSampling==true the
 * covariance is computed by montecarlo sampling on the mesh (faster); If
 * montecarloSampling==false the covariance is computed by (analytic)integration
 * over the surface (slower).
 * @param m
 * @param radius
 * @param montecarloSampling
 * @param log
 */
template<FaceMeshConcept MeshType, LoggerConcept LogType = NullLogger>
void updatePrincipalCurvaturePCA(
    MeshType&                                            m,
    typename MeshType::VertexType::CoordType::ScalarType radius,
    bool     montecarloSampling = true,
    LogType& log                = nullLogger)
{
    using VertexType = MeshType::VertexType;
    using CoordType  = VertexType::CoordType;
    using ScalarType = CoordType::ScalarType;
    using NormalType = VertexType::NormalType;
    using FaceType   = MeshType::FaceType;

    using VGrid         = vcl::StaticGrid3<VertexType*, ScalarType>;
    using VGridIterator = VGrid::ConstIterator;

    VGrid      pGrid;
    ScalarType area;

    log.log(0, "Updating per vertex normals...");

    vcl::updatePerVertexNormalsAngleWeighted(m);
    vcl::normalizePerVertexNormals(m);

    log.log(0, "Computing per vertex curvature...");
    log.startProgress("", m.vertexNumber());

    if (montecarloSampling) {
        area  = vcl::surfaceArea(m);
        pGrid = VGrid(m.vertices() | views::addrOf);
        pGrid.build();
    }

    vcl::parallelFor(m.vertices(), [&](VertexType& v) {
        // for (VertexType& v : m.vertices()) {
        vcl::Matrix33<ScalarType> A, eigenvectors;
        CoordType                 bp, eigenvalues;
        if (montecarloSampling) {
            vcl::Sphere                s(v.coord(), radius);
            std::vector<VGridIterator> vec = pGrid.valuesInSphere(s);
            std::vector<CoordType>     points;
            points.reserve(vec.size());
            for (const auto& it : vec) {
                points.push_back(it->second->coord());
            }
            A = vcl::covarianceMatrixOfPointCloud(points);
            A *= area * area / 1000;
        }
        else {
            Sphere<ScalarType> sph(v.coord(), radius);
            MeshType           tmpMesh = meshSphereIntersection(m, sph);

            A = covarianceMatrixOfMesh(tmpMesh);
        }
        Eigen::SelfAdjointEigenSolver<vcl::Matrix33<ScalarType>> eig(A);
        eigenvalues  = CoordType(eig.eigenvalues());
        eigenvectors = eig.eigenvectors(); // eigenvector are stored as columns.
        // get the estimate of curvatures from eigenvalues and eigenvectors
        // find the 2 most tangent eigenvectors (by finding the one closest to
        // the normal)
        uint       best  = 0;
        ScalarType bestv = std::abs(
            v.normal().dot(CoordType(eigenvectors.col(0).normalized())));
        for (uint i = 1; i < 3; ++i) {
            ScalarType prod = std::abs(
                v.normal().dot(CoordType(eigenvectors.col(i).normalized())));
            if (prod > bestv) {
                bestv = prod;
                best  = i;
            }
        }
        v.principalCurvature().maxDir() =
            (eigenvectors.col((best + 1) % 3).normalized());
        v.principalCurvature().minDir() =
            (eigenvectors.col((best + 2) % 3).normalized());

        vcl::Matrix33<ScalarType> rot;
        ScalarType                angle;
        angle = acos(v.principalCurvature().maxDir().dot(v.normal()));

        rot = vcl::rotationMatrix<vcl::Matrix33<ScalarType>>(
            v.principalCurvature().maxDir().cross(v.normal()),
            -(M_PI * 0.5 - angle));

        v.principalCurvature().maxDir() = rot * v.principalCurvature().maxDir();

        angle = acos(v.principalCurvature().minDir().dot(v.normal()));

        rot = vcl::rotationMatrix<vcl::Matrix33<ScalarType>>(
            v.principalCurvature().minDir().cross(v.normal()),
            -(M_PI * 0.5 - angle));

        v.principalCurvature().minDir() = rot * v.principalCurvature().minDir();

        // computes the curvature values
        const ScalarType r5 = std::pow(radius, 5);
        const ScalarType r6 = r5 * radius;
        v.principalCurvature().maxValue() =
            (2.0 / 5.0) *
            (4.0 * M_PI * r5 + 15 * eigenvalues[(best + 2) % 3] -
             45.0 * eigenvalues[(best + 1) % 3]) /
            (M_PI * r6);
        v.principalCurvature().minValue() =
            (2.0 / 5.0) *
            (4.0 * M_PI * r5 + 15 * eigenvalues[(best + 1) % 3] -
             45.0 * eigenvalues[(best + 2) % 3]) /
            (M_PI * r6);
        if (v.principalCurvature().maxValue() <
            v.principalCurvature().minValue())
        {
            std::swap(
                v.principalCurvature().minValue(),
                v.principalCurvature().maxValue());
            std::swap(
                v.principalCurvature().minDir(),
                v.principalCurvature().maxDir());
        }

        log.progress(m.index(v));
        //}
    });

    log.endProgress();
    log.log(100, "Per vertex curvature computed.");
}

template<FaceMeshConcept MeshType, LoggerConcept LogType = NullLogger>
void updatePrincipalCurvature(MeshType& m, LogType& log = nullLogger)
{
    vcl::requirePerVertexPrincipalCurvature(m);

    updatePrincipalCurvatureTaubin95(m, log);
}

template<FaceMeshConcept MeshType, LoggerConcept LogType = NullLogger>
void updatePrincipalCurvature(
    MeshType&                        m,
    VCLibPrincipalCurvatureAlgorithm alg = VCL_PRINCIPAL_CURVATURE_TAUBIN95,
    LogType&                         log = nullLogger)
{
    vcl::requirePerVertexPrincipalCurvature(m);

    double radius;
    switch (alg) {
    case VCL_PRINCIPAL_CURVATURE_TAUBIN95:
        updatePrincipalCurvatureTaubin95(m, log);
        break;
    case VCL_PRINCIPAL_CURVATURE_PCA:
        radius = vcl::boundingBox(m).diagonal() * 0.1;
        updatePrincipalCurvaturePCA(m, radius, true, log);
    }
}

} // namespace vcl

#endif // VCL_ALGORITHMS_UPDATE_CURVATURE_H
