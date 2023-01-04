/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include "curvature.h"

#include <vclib/algorithms/update/normal.h>
#include <vclib/algorithms/polygon.h>
#include <vclib/math/matrix.h>
#include <vclib/mesh_utils/mesh_pos.h>
#include <vclib/space/principal_curvature.h>

namespace vcl {

template<HasPerVertexPrincipalCurvature MeshType>
void updatePrincipalCurvatureTaubin95(MeshType& m)
{
	vcl::requirePerVertexPrincipalCurvature(m);
	vcl::requirePerVertexAdjacentFaces(m);
	vcl::requirePerFaceAdjacentFaces(m);

	using VertexType = typename MeshType::VertexType;
	using CoordType = typename VertexType::CoordType;
	using NormalType = typename VertexType::NormalType;
	using ScalarType = typename CoordType::ScalarType;
	using FaceType   = typename MeshType::FaceType;

	// Auxiliary data structure
	struct AdjVertex {
		const VertexType* vert;
		double doubleArea;
		bool isBorder;
	};

	vcl::updatePerVertexNormalsAngleWeighted(m);
	vcl::normalizePerVertexNormals(m);

	for (VertexType& v : m.vertices()) {
		std::vector<ScalarType> weights;
		std::vector<AdjVertex> vertices;

		vcl::MeshPos<FaceType> pos(v.adjFace(0), &v);
		const VertexType* firstVertex = pos.adjVertex();
		const VertexType* tmpVertex;
		float totalDoubleAreaSize = 0;

		// compute the area of each triangle around the central vertex as well as their total area
		do {
			pos.nextEdgeAdjacentToV();
			tmpVertex = pos.adjVertex();
			AdjVertex adjV;
			adjV.isBorder = pos.isEdgeOnBorder();
			adjV.vert = tmpVertex;
			adjV.doubleArea = vcl::polygonArea(*pos.face()) * 2;
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
					(vertices[i].doubleArea + vertices[(i - 1) % vertices.size()].doubleArea) /
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

		// for all neighbors vi compute the directional curvatures k_i and the T_i
		// compute M by summing all w_i k_i T_i T_i^t
		Matrix33<ScalarType> tempMatrix;
		Matrix33<ScalarType> M = Matrix33<ScalarType>::Zero();
		for (size_t i = 0; i < vertices.size(); ++i) {
			CoordType edge = (v.coord() - vertices[i].vert->coord());
			float curvature = (2.0f * (v.normal().dot(edge)) ) / edge.squaredNorm();
			CoordType t(Tp * edge.eigenVector().transpose());
			t.normalize();
			tempMatrix = outerProduct(t, t);
			M += tempMatrix * weights[i] * curvature ;
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
		tempMatrix = outerProduct(w, w);
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
		ScalarType delta = std::sqrt(4.0f * std::pow(alpha, 2) + 16.0f * std::pow(beta, 2));
		h[0] = (2.0f * alpha + delta) / (2.0f * beta);
		h[1] = (2.0f * alpha - delta) / (2.0f * beta);

		ScalarType t[2];
		ScalarType bestC, bestS;
		ScalarType minError = std::numeric_limits<ScalarType>::infinity();
		for (uint i=0; i < 2; i++) {
			delta = std::sqrt(std::pow(h[i], 2) + 4.0f);
			t[0]  = (h[i] + delta) / 2.0f;
			t[1]  = (h[i] - delta) / 2.0f;

			for (uint j=0; j<2; j++) {
				ScalarType squaredT    = std::pow(t[j], 2);
				ScalarType denominator = 1.0f + squaredT;

				s = (2.0f * t[j]) / denominator;
				c = (1 - squaredT) / denominator;

				ScalarType approximation = c * s * alpha + (std::pow(c, 2) - std::pow(s, 2)) * beta;
				ScalarType angleSimilarity = std::abs(std::acos(c) / std::asin(s));
				ScalarType error = std::abs(1.0f - angleSimilarity) + std::abs(approximation);
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
		ScalarType principalCurv1 = (3.0f * StMS(0,0)) - StMS(1,1);
		ScalarType principalCurv2 = (3.0f * StMS(1,1)) - StMS(0,0);

		Eigen::Matrix<ScalarType, 1, 3> principalDir1 = T1 * c - T2 * s;
		Eigen::Matrix<ScalarType, 1, 3> principalDir2 = T1 * s + T2 * c;

		v.principalCurvature().maxDir()   = principalDir1;
		v.principalCurvature().minDir()   = principalDir2;
		v.principalCurvature().maxValue() = principalCurv1;
		v.principalCurvature().minValue() = principalCurv2;
	}
}

template<HasPerVertexPrincipalCurvature MeshType>
void updatePrincipalCurvature(MeshType& m, VCLibPrincipalCurvatureAlgorithm alg)
{
	vcl::requirePerVertexPrincipalCurvature(m);

	switch (alg) {
	case VCL_PRINCIPAL_CURVATURE_TAUBIN95:
		updatePrincipalCurvatureTaubin95(m);
		break;
	}
}

} // namespace vcl