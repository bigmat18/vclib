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

#ifndef VCL_ALGORITHMS_CORE_POLYGON_GEOMETRY_H
#define VCL_ALGORITHMS_CORE_POLYGON_GEOMETRY_H

#include <vclib/concepts/mesh/elements/face.h>
#include <vclib/space/core/polygon.h>
#include <vclib/views/mesh.h>

namespace vcl {

/**
 * @brief Computes the normal of a face, without modifying the face. Works both
 * for triangle and polygonal faces, and it is optimized in case of triangle
 * faces.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: the input face.
 * @return the normal of the face.
 */
template<FaceConcept FaceType>
typename FaceType::VertexType::CoordType faceNormal(const FaceType& f)
{
    using CoordType = FaceType::VertexType::CoordType;
    if constexpr (TriangleFaceConcept<FaceType>) {
        return Triangle<CoordType>::normal(
            f.vertex(0)->coord(), f.vertex(1)->coord(), f.vertex(2)->coord());
    }
    else {
        if (f.vertexNumber() == 3) {
            return Triangle<CoordType>::normal(
                f.vertex(0)->coord(),
                f.vertex(1)->coord(),
                f.vertex(2)->coord());
        }
        else {
            return Polygon<CoordType>::normal(f.vertices() | views::coords);
        }
    }
}

/**
 * @brief Computes the barycenter of a face. Works both for triangle and
 * polygonal faces, and it is optimized in case of triangle faces.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: the input face.
 * @return the barycenter of the face.
 */
template<FaceConcept FaceType>
typename FaceType::VertexType::CoordType faceBarycenter(const FaceType& f)
{
    using CoordType = FaceType::VertexType::CoordType;
    if constexpr (TriangleFaceConcept<FaceType>) {
        return Triangle<CoordType>::barycenter(
            f.vertex(0)->coord(), f.vertex(1)->coord(), f.vertex(2)->coord());
    }
    else {
        return Polygon<CoordType>::barycenter(f.vertices() | views::coords);
    }
}

/**
 * @brief Computes the area of a face. Works both for triangle and polygonal
 * faces, and it is optimized in case of triangle faces.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: the input face.
 * @return the area of the face.
 */
template<FaceConcept FaceType>
auto faceArea(const FaceType& f)
{
    using CoordType = FaceType::VertexType::CoordType;
    if constexpr (TriangleFaceConcept<FaceType>) {
        return Triangle<CoordType>::area(
            f.vertex(0)->coord(), f.vertex(1)->coord(), f.vertex(2)->coord());
    }
    else {
        if (f.vertexNumber() == 3) {
            return Triangle<CoordType>::area(
                f.vertex(0)->coord(),
                f.vertex(1)->coord(),
                f.vertex(2)->coord());
        }
        else {
            return Polygon<CoordType>::area(f.vertices() | views::coords);
        }
    }
}

/**
 * @brief Computes the perimeter of a face. Works both for triangle and
 * polygonal faces, and it is optimized in case of triangle faces.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: the input face.
 * @return the perimeter of the face.
 */
template<FaceConcept FaceType>
auto facePerimeter(const FaceType& f)
{
    using CoordType = FaceType::VertexType::CoordType;
    if constexpr (TriangleFaceConcept<FaceType>) {
        return Triangle<CoordType>::perimeter(
            f.vertex(0)->coord(), f.vertex(1)->coord(), f.vertex(2)->coord());
    }
    else {
        if (f.vertexNumber() == 3) {
            return Triangle<CoordType>::perimeter(
                f.vertex(0)->coord(),
                f.vertex(1)->coord(),
                f.vertex(2)->coord());
        }
        else {
            return Polygon<CoordType>::perimeter(f.vertices() | views::coords);
        }
    }
}

/**
 * @brief Returns the internal angle (in radians) of the vi-th vertex of the
 * face.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: the input face.
 * @param[in] vi: the index of the vertex in the face on which calculate the
 * angle
 * @return the angle in radians at the vi-th vertex.
 */
template<FaceConcept FaceType>
auto faceAngleOnVertexRad(const FaceType& f, uint vi)
{
    const auto& p0 = f.vertex(vi)->coord();
    const auto& p1 = f.vertexMod((int) vi + 1)->coord();
    const auto& p2 = f.vertexMod((int) vi - 1)->coord();
    return (p2 - p0).angle(p1 - p0);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_POLYGON_GEOMETRY_H
