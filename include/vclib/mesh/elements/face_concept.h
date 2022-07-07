/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#ifndef VCL_MESH_ELEMENTS_FACE_CONCEPT_H
#define VCL_MESH_ELEMENTS_FACE_CONCEPT_H

#include "face_components.h"

namespace vcl {

template<typename...>
class Face;

namespace face {

// checks if a type derives from vcl::Face<Args...>
template<typename Derived>
using IsDerivedFromFace = IsDerivedFromTemplateSpecialization<Derived, Face>;

// checks if a type is a vcl::Face<Args...>
template<class T>
struct IsAFace : // Default case, no pattern match
		std::false_type
{
};

template<class... Args>
struct IsAFace<Face<Args...>> : // For types matching the pattern Face<Args...>
		std::true_type
{
};

/* Port concepts into the vert namespace */
template<typename T>
concept HasAdjacentEdges =
	comp::HasAdjacentEdges<T> && comp::HasRightNumberOfAdjacentEdges<T>;
template<typename T>
concept HasAdjacentFaces =
	comp::HasAdjacentFaces<T> && comp::HasRightNumberOfAdjacentFaces<T>;
template<typename T>
concept HasBitFlags = comp::HasBitFlags<T>;
template<typename T>
concept HasColor = comp::HasColor<T>;
template<typename T>
concept HasHalfEdgeReference = comp::HasFaceHalfEdgeReference<T>;
template<typename T>
concept HasMark = comp::HasMark<T>;
template<typename T>
concept HasNormal = comp::HasNormal<T>;
template<typename T>
concept HasPolygonBitFlags = comp::HasPolygonBitFlags<T>;
template<typename T>
concept HasPrincipalCurvature = comp::HasPrincipalCurvature<T>;
template<typename T>
concept HasScalar = comp::HasScalar<T>;
template<typename T>
concept HasTriangleBitFlags = comp::HasTriangleBitFlags<T>;
template<typename T>
concept HasVertexReferences = comp::HasVertexReferences<T>;
template<typename T>
concept HasWedgeColors =
	comp::HasWedgeColors<T> && comp::HasRightNumberOfWedgeColors<T>;
template<typename T>
concept HasWedgeTexCoords =
	comp::HasWedgeColors<T> && comp::HasRightNumberOfWedgeTexCoords<T>;

} // namespace vcl::face

/**
 * @brief FaceConcept
 *
 * The Face concept describes how a Face element that can be used for a FaceContainer should be
 * organized.
 *
 * The Face concept is satisfied for a class F if ALL the following sentences are true:
 * - The class F is vcl::Face, or derives from it;
 * - The class F has the BitFlags component (or a derivate);
 * - The class F has the VertexReferences component (or a derivate);
 * - The number of vertices of the VertexReferences is -1 (dynamic size) or at least 3 (static size)
 * - If the class F has the TriangleBitFlags component (or a derivate), the number of vertices must
 *   be 3 (static)
 * - If the class F has the AdjacentEdges component (or a derivate), its size must be the same of
 *   the vertices;
 * - If the class F has the AdjacentFaces component (or a derivate), its size must be the same of
 *   the vertices;
 * - If the class F has the WedgeColors component (or a derivate), its size must be the same of the
 *   vertices;
 * - If the class F has the WedgeTexCoords component (or a derivate), its size must be the same of
 *   the vertices;
 */
template<typename T>
concept FaceConcept =
	(face::IsDerivedFromFace<T>::value || face::IsAFace<T>::value) &&
	face::HasBitFlags<T> &&
	face::HasVertexReferences<T> &&
	(T::VERTEX_NUMBER < 0 || T::VERTEX_NUMBER >= 3) &&
	(!face::HasTriangleBitFlags<T> || T::VERTEX_NUMBER == 3) &&
	comp::SanityCheckAdjacentEdges<T> &&
	comp::SanityCheckAdjacentFaces<T> &&
	comp::SanityCheckWedgeColors<T> &&
	comp::SanityCheckWedgeTexCoords<T>;

} // namespace vcl

#endif // VCL_MESH_ELEMENTS_FACE_CONCEPT_H
