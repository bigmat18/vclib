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

#include "face.h"

namespace vcl {

template<typename MeshType, typename... Args>
Face<MeshType, Args...>::Face()
{
}

/**
 * @brief Constructs a Face with the given set of vertices.
 *
 * Sets a list of Vertex pointers to the face. If the Face size is non-dcel dynamic, will
 * take care to update the also the number of adjacent faces and the number of wedge components, if
 * these components are part of the Face and if the size of the Face is changed. On the contrary, if
 * the Face size is static or it belongs to a Dcel mesh, the number of vertices of the list must be
 * equal to the size of the Face (the value returned by vertexNumber()).
 *
 * @note This constructor cannot resize the number of vertices (and other components tied to the
 * number of vertices) if it called from a Face of a Dcel mesh.
 *
 * @todo do proper checks on the number of vertices at compile time and at runtime
 *
 * @param[in] list: a container of vertex pointers in counterclockwise order that will be set as
 *                  vertices of the face.
 */
template<typename MeshType, typename... Args>
Face<MeshType, Args...>::Face(const std::vector<VertexType*>& list) // todo add requires
{
	setVertices(list);
}

template<typename MeshType, typename... Args>
uint Face<MeshType, Args...>::index() const
{
	return Element<MeshType, Args...>::template index<typename MeshType::FaceType>();
}

/**
 * @brief Sets a list of Vertex references to the face. If the Face size is non-dcel dynamic, will
 * take care to update the also the number of adjacent faces and the number of wedge components, if
 * these components are part of the Face and if the size of the Face is changed. On the contrary, if
 * the Face size is static or it belongs to a Dcel mesh, the number of arguments of this function
 * must be equal to the size of the Face (the value returned by vertexNumber()).
 *
 * @note This constructor cannot be called if the Face type belongs to a Dcel mesh.
 *
 * @param[in] args: a variable number of vertex pointers in counterclockwise order that will be set
 *                  as vertices of the face.
 */
template<typename MeshType, typename... Args>
template<typename... V>
Face<MeshType, Args...>::Face(V... args) // todo add requires
{
	setVertices({args...});
}

/**
 * @brief Sets a list of Vertex references to the face. If the Face size is non-dcel dynamic, will
 * take care to update the also the number of adjacent faces and the number of wedge components, if
 * these components are part of the Face and if the size of the Face is changed. On the contrary, if
 * the Face size is static or it belongs to a Dcel mesh, the number of vertices of the list must be
 * equal to the size of the Face (the value returned by vertexNumber()).
 *
 * @param[in] list: a container of vertex pointers in counterclockwise order that will be set as
 *                  vertices of the face.
 */
template<typename MeshType, typename... Args>
void Face<MeshType, Args...>::setVertices(const std::vector<VertexType*>& list)
{
	using F = Face<MeshType, TypeWrapper<Args...>>;
	
	VPtrs::setVertices(list);

	if constexpr (comp::HasPolygonBitFlags<F>) {
		using T = typename F::PolygonBitFlags;
		T::resizeBitFlags(list.size());
	}

	if constexpr (comp::HasAdjacentEdges<F> && NonDcelPolygonFaceConcept<F>) {
		using T = typename F::AdjacentEdges;

		if (T::isAdjEdgesEnabled())
			T::resizeAdjEdges(list.size());
	}

	if constexpr (face::HasAdjacentFaces<F> && NonDcelPolygonFaceConcept<F>) {
		using T = typename F::AdjacentFaces;

		if (T::isAdjFacesEnabled())
			T::resizeAdjFaces(list.size());
	}

	if constexpr (face::HasWedgeColors<F> && NonDcelPolygonFaceConcept<F>) {
		using T = typename F::WedgeColors;

		if (T::isWedgeColorsEnabled())
			T::resizeWedgeColors(list.size());
	}

	if constexpr (face::HasWedgeTexCoords<F> && NonDcelPolygonFaceConcept<F>) {
		using T = typename F::WedgeTexCoords;

		if (T::isWedgeTexCoordsEnabled())
			T::resizeWedgeTexCoords(list.size());
	}
}

template<typename MeshType, typename... Args>
template<typename... V>
void Face<MeshType, Args...>::setVertices(V... args)
{
	setVertices({args...});
}

/**
 * @brief Resize the number of Vertex Pointers of the Face, taking care of updating also the
 * other components of the Face that are tied to that number.
 *
 * If n is greater than the old number of vertex pointers, n vertex pointers (and relative
 * tied components) will be added. If n is lower than the old number of vertex pointers, the
 * difference of vertex pointers (and relative tied components) will be removed.
 *
 * This member function is available only **if the face is polygonal (its size is dynamic, N < 0)**.
 *
 * @param n: the new number of vertices.
 */
//TODO: Clang bug. Move definition here when it will be solved
//template<typename MeshType, typename... Args>
//void Face<MeshType, Args...>::resizeVertices(uint n) requires PolygonFaceConcept<Face>
//{

//}

//TODO: Clang bug. Move definition here when it will be solved
//template<typename MeshType, typename... Args>
//void Face<MeshType, Args...>::pushVertex(VertexType* v) requires PolygonFaceConcept<Face>
//{

//}

//TODO: Clang bug. Move definition here when it will be solved
//template<typename MeshType, typename... Args>
//void Face<MeshType, Args...>::insertVertex(uint i, VertexType* v) requires PolygonFaceConcept<Face>
//{

//}

//TODO: Clang bug. Move definition here when it will be solved
//template<typename MeshType, typename... Args>
//void Face<MeshType, Args...>::eraseVertex(uint i) requires PolygonFaceConcept<Face>
//{

//}

//TODO: Clang bug. Move definition here when it will be solved
//template<typename MeshType, typename... Args>
//void Face<MeshType, Args...>::clearVertices() requires PolygonFaceConcept<Face>
//{

//}

} // namespace vcl


