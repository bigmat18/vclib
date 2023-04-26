/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_ADJACENT_VERTICES_H
#define VCL_CONCEPTS_MESH_COMPONENTS_ADJACENT_VERTICES_H

#include <vector>

#include "component.h"

namespace vcl::comp {

/**
 * @brief HasAdjacentVertices concept is satisfied only if a Element class provides the types and
 * member functions specified in this concept. These types and member functions allow to access to
 * an AdjacentVertices component of a given element.
 *
 * Note that this concept does not discriminate between the Horizontal AdjacentVertices component and
 * the vertical OptionalAdjacentVertices component, therefore it does not guarantee that a template
 * Element type that satisfies this concept provides AdjacentVertices component at runtime (it is
 * guaranteed only that the proper member functions are available at compile time).
 *
 * To be completely sure that AdjacentVertices is available at runtime, you need to call the member
 * function `isAdjVerticesEnabled()`.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasAdjacentVertices = requires(
	T o,
	const T& co,
	typename T::AdjacentVertexType v,
	std::vector<typename T::AdjacentVertexType*> vec)
{
	typename T::AdjacentVertexType;
	typename T::AdjacentVertexIterator;
	typename T::ConstAdjacentVertexIterator;
	typename T::AdjacentVertexView;
	typename T::ConstAdjacentVertexView;

	{ o.adjVerticesNumber() } -> std::same_as<uint>;
	{ o.adjVertex(uint()) } -> std::same_as<typename T::AdjacentVertexType*&>;
	{ co.adjVertex(uint()) } -> std::same_as<const typename T::AdjacentVertexType*>;
	{ o.adjVertexMod(int()) } -> std::same_as<typename T::AdjacentVertexType*&>;
	{ co.adjVertexMod(int()) } -> std::same_as<const typename T::AdjacentVertexType*>;
	{ o.setAdjVertex(&v, uint()) } -> std::same_as<void>;
	{ o.setAdjVertices(vec) } -> std::same_as<void>;
	{ co.containsAdjVertex(&v) } -> std::same_as<bool>;

	{ o.findAdjVertex(&v) } -> std::same_as<typename T::AdjacentVertexIterator>;
	{ co.findAdjVertex(&v) } -> std::same_as<typename T::ConstAdjacentVertexIterator>;
	{ co.indexOfAdjVertex(&v) } -> std::same_as<int>;
	{ co.isAdjVerticesEnabled() } -> std::same_as<bool>;

	{ o.adjVertexBegin() } -> std::same_as<typename T::AdjacentVertexIterator>;
	{ o.adjVertexEnd() } -> std::same_as<typename T::AdjacentVertexIterator>;
	{ co.adjVertexBegin() } -> std::same_as<typename T::ConstAdjacentVertexIterator>;
	{ co.adjVertexEnd() } -> std::same_as<typename T::ConstAdjacentVertexIterator>;
	{ o.adjVertices() } -> std::same_as<typename T::AdjacentVertexView>;
	{ co.adjVertices() } -> std::same_as<typename T::ConstAdjacentVertexView>;
};

/**
 * @private
 * @brief HasAdjacentVerticesComponent concept is used to discriminate between the AdjacentVertices
 * (or OptionalAdjacentVertices) component, and the VertexHalfEdgeReferences component, which using
 * half edges allows to access to adjacent vertices. This concept is intended only for internal use,
 * useful to check that a Vertex does not have both AdjacentVertices and VertexHalfEdgeReferences
 * components.
 */
template<typename T>
concept HasAdjacentVerticesComponent = requires(T o)
{
	requires HasAdjacentVertices<T>;
	typename T::AdjacentVerticesComponent;
};

/**
 * @brief HasOptionalAdjacentVertices concept is satisfied only if a class satisfies the
 * HasAdjacentVertices concept and has the static boolean constant IS_OPTIONAL is set to true.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalAdjacentVertices =
	HasAdjacentVerticesComponent<T> && IsOptionalComponent<typename T::AdjacentVerticesComponent>;

} // namespace vcl::comp

#endif // VCL_CONCEPTS_MESH_COMPONENTS_ADJACENT_VERTICES_H
