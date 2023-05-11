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

#include "vertex_container.h"

namespace vcl::mesh {

/**
 * @brief Empty constructor that creates an empty container of Vertices.
 */
template<VertexConcept T>
VertexContainer<T>::VertexContainer()
{
}

/**
 * @brief Returns a const reference of the vertex at the i-th position in the Vertex Container of
 * the Mesh, which will be the vertex having index = i.
 *
 * This function does not perform any sanity check: if i is less than vertexContainerSize(), this
 * function will return a valid Vertex reference (note that the Vertex may have been flagged as
 * deleted).
 *
 * @param[in] i: the index of the vertex that will be returned.
 */
template<VertexConcept T>
const typename VertexContainer<T>::VertexType& VertexContainer<T>::vertex(uint i) const
{
	return Base::element(i);
}

/**
 * @brief Returns a reference of the vertex at the i-th position in the Vertex Container of the
 * Mesh, which will be the vertex having index = i.
 *
 * This function does not perform any sanity check: if i is less than vertexContainerSize(), this
 * function will return a valid Vertex reference (note that the Vertex may have been flagged as
 * deleted).
 *
 * @param[in] i: the index of the vertex that will be returned.
 */
template<VertexConcept T>
typename VertexContainer<T>::VertexType& VertexContainer<T>::vertex(uint i)
{
	return Base::element(i);
}

/**
 * @brief Returns the number of **non-deleted** vertices contained in the Vertex container of the
 * Mesh.
 *
 * If vertexNumber() != vertexContainerSize(), it means that there are some vertices that are
 * flagged as deleted.
 *
 * @return The number of non-deleted vertices of the Mesh.
 */
template<VertexConcept T>
uint VertexContainer<T>::vertexNumber() const
{
	return Base::elementNumber();
}

/**
 * @brief Returns the number of vertices (also deleted) contained in the Vertex container of the
 * Mesh.
 *
 * If vertexNumber() != vertexContainerSize(), it means that there are some vertices that are
 * flagged as deleted.
 *
 * @return The number of all the vertices contained in the Mesh.
 */
template<VertexConcept T>
uint VertexContainer<T>::vertexContainerSize() const
{
	return Base::elementContainerSize();
}

/**
 * @brief Returns the number of deleted vertices in the Vertex container, that is
 * vertexContainerSize() - vertexNumber().
 *
 * @return The number of deleted vertices in the container.
 */
template<VertexConcept T>
uint vcl::mesh::VertexContainer<T>::deletedVertexNumber() const
{
	return Base::deletedElementNumber();
}

/**
 * @brief Marks as deleted the vertex with the given id.
 *
 * This member function does not perform any reallocation of the vertices: the deleted vertices
 * will stay in the Vertex Container, but will be marked as deleted.
 *
 * Deleted vertices are automatically jumped by the iterators provided by the Vertex Container.
 *
 * @param[in] i: the id of the vertex that will be marked as deleted.
 */
template<VertexConcept T>
void VertexContainer<T>::deleteVertex(uint i)
{
	Base::deleteElement(i);
}

/**
 * @brief Marks as deleted the given vertex, before asserting that the vertex belongs to this
 * container.
 *
 * This member function does not perform any reallocation of the vertices: the deleted vertices
 * will stay in the Vertex Container, but will be marked as deleted.
 *
 * Deleted vertices are automatically jumped by the iterators provided by the Vertex Container.
 *
 * @param[in] v: the pointer of the vertex that will be marked as deleted.
 */
template<VertexConcept T>
void VertexContainer<T>::deleteVertex(const VertexType* v)
{
	Base::deleteElement(v);
}

/**
 * @brief This is an utility member function that returns the index of an element if the container
 * would be compact, that is the number of non-deleted elements before the vertex with the given
 * index.
 *
 * Complexity: O(n), with n the number of vertices in the container.
 *
 * This function does not perform any sanity check on the given index.
 *
 * @param[in] i: the index of a vertex of the container.
 * @return The index that the vertex with index i would have if this container would be compact.
 */
template<VertexConcept T>
uint vcl::mesh::VertexContainer<T>::vertexIndexIfCompact(uint i) const
{
	return Base::elementIndexIfCompact(i);
}

/**
 * @brief Returns a vector that tells, for each actual vertex index, the new index that the vertex
 * would have in a compacted container. For each deleted vertex index, the value of the vector will
 * be -1.
 *
 * This is useful if you need to know the indices of the vertices that they would have in a
 * compact container, without considering the deleted ones.
 *
 * @return A vector containing, for each vertex index, its index if the container would be compact.
 */
template<VertexConcept T>
std::vector<int> VertexContainer<T>::vertexCompactIndices() const
{
	return Base::elementCompactIndices();
}

/**
 * @brief Returns an iterator to the beginning of the container.
 *
 * The iterator is automatically initialized to jump deleted vertices of the container. You can
 * change this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted
 * vertices.
 * @return An iterator the the first vertex of the container.
 */
template<VertexConcept T>
typename VertexContainer<T>::VertexIterator VertexContainer<T>::vertexBegin(bool jumpDeleted)
{
	return Base::elementBegin(jumpDeleted);
}

/**
 * @brief Returns an iterator to the end of the container.
 * @return An iterator to the end of the container.
 */
template<VertexConcept T>
typename VertexContainer<T>::VertexIterator VertexContainer<T>::vertexEnd()
{
	return Base::elementEnd();
}

/**
 * @brief Returns a const iterator to the beginning of the container.
 *
 * The iterator is automatically initialized to jump deleted vertices of the container. You can
 * change this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted
 * vertices.
 * @return A const iterator the the first vertex of the container.
 */
template<VertexConcept T>
typename VertexContainer<T>::ConstVertexIterator
VertexContainer<T>::vertexBegin(bool jumpDeleted) const
{
	return Base::elementBegin(jumpDeleted);
}

/**
 * @brief Returns a const iterator to the end of the container.
 * @return A const iterator to the end of the container.
 */
template<VertexConcept T>
typename VertexContainer<T>::ConstVertexIterator VertexContainer<T>::vertexEnd() const
{
	return Base::elementEnd();
}

/**
 * @brief Returns a small utility object that allows to iterate over the vertices of the containers,
 * providing two member functions begin() and end().
 *
 * This member function is very useful when you want to iterate over the vertices using the C++
 * foreach syntax:
 *
 * @code{.cpp}
 * for (Vertex& v : m.vertices()){
 *     // do something with this vertex
 * }
 * @endcode
 *
 * The iterator used to iterate over vertices is automatically initialized to jump deleted vertices
 * of the container. You can change this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted
 * vertices.
 * @return An object having begin() and end() function, allowing to iterate over the container.
 */
template<VertexConcept T>
typename VertexContainer<T>::VertexView VertexContainer<T>::vertices(bool jumpDeleted)
{
	return Base::elements(jumpDeleted);
}

/**
 * @brief Returns a small utility object that allows to iterate over the vertices of the containers,
 * providing two member functions begin() and end().
 *
 * This member function is very useful when you want to iterate over the vertices using the C++
 * foreach syntax:
 *
 * @code{.cpp}
 * for (const Vertex& v : m.vertices()){
 *     // do something with this vertex
 * }
 * @endcode
 *
 * The iterator used to iterate over vertices is automatically initialized to jump deleted vertices
 * of the container. You can change this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted
 * vertices.
 * @return An object having begin() and end() function, allowing to iterate over the container.
 */
template<VertexConcept T>
typename VertexContainer<T>::ConstVertexView
VertexContainer<T>::vertices(bool jumpDeleted) const
{
	return Base::elements(jumpDeleted);
}

/**
 * @brief Enables all the optional components associated to the Vertex type contained in the
 * VertexContainer.
 */
template<VertexConcept T>
void VertexContainer<T>::enableAllPerVertexOptionalComponents()
{
	if constexpr (vert::HasOptionalAdjacentEdges<T>)
		enablePerVertexAdjacentEdges();
	if constexpr (vert::HasOptionalAdjacentFaces<T>)
		enablePerVertexAdjacentFaces();
	if constexpr (vert::HasOptionalAdjacentVertices<T>)
		enablePerVertexAdjacentVertices();
	if constexpr (vert::HasOptionalColor<T>)
		enablePerVertexColor();
	if constexpr (vert::HasOptionalMark<T>)
		enablePerVertexMark();
	if constexpr (vert::HasOptionalNormal<T>)
		enablePerVertexNormal();
	if constexpr (vert::HasOptionalPrincipalCurvature<T>)
		enablePerVertexPrincipalCurvature();
	if constexpr (vert::HasOptionalQuality<T>)
		enablePerVertexQuality();
	if constexpr (vert::HasOptionalTexCoord<T>)
		enablePerVertexTexCoord();
}

/**
 * @brief Disables all the optional components associated to the Vertex type contained in the
 * VertexContainer.
 */
template<VertexConcept T>
void VertexContainer<T>::disableAllPerVertexOptionalComponents()
{
	if constexpr (vert::HasOptionalAdjacentEdges<T>)
		disablePerVertexAdjacentEdges();
	if constexpr (vert::HasOptionalAdjacentFaces<T>)
		disablePerVertexAdjacentFaces();
	if constexpr (vert::HasOptionalAdjacentVertices<T>)
		disablePerVertexAdjacentVertices();
	if constexpr (vert::HasOptionalColor<T>)
		disablePerVertexColor();
	if constexpr (vert::HasOptionalMark<T>)
		disablePerVertexMark();
	if constexpr (vert::HasOptionalNormal<T>)
		disablePerVertexNormal();
	if constexpr (vert::HasOptionalPrincipalCurvature<T>)
		disablePerVertexPrincipalCurvature();
	if constexpr (vert::HasOptionalQuality<T>)
		disablePerVertexQuality();
	if constexpr (vert::HasOptionalTexCoord<T>)
		disablePerVertexTexCoord();
}

/**
 * @brief Checks if the vertex Optional Adjacent Edges component is enabled.
 *
 * @note This function is available only if the Vertex Element has the OptionalAdjacentEdges
 * Component.
 *
 * @return true if the Optional Adjacent Edges is enabled, false otherwise.
 */
template<VertexConcept T>
bool VertexContainer<T>::isPerVertexAdjacentEdgesEnabled()
	const requires vert::HasOptionalAdjacentEdges<T>
{
	return Base::template isOptionalComponentEnabled<typename T::AdjacentEdges>();
}

/**
 * @brief Enables the Optional Adjacent Edges of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalAdjacentEdges
 * Component.
 */
template<VertexConcept T>
void VertexContainer<T>::enablePerVertexAdjacentEdges() requires vert::HasOptionalAdjacentEdges<T>
{
	Base::template enableOptionalComponent<typename T::AdjacentEdges>();
}

/**
 * @brief Disables the Optional Adjacent Edges of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalAdjacentEdges
 * Component.
 */
template<VertexConcept T>
void VertexContainer<T>::disablePerVertexAdjacentEdges() requires vert::HasOptionalAdjacentEdges<T>
{
	Base::template disableOptionalComponent<typename T::AdjacentEdges>();
}

/**
 * @brief Checks if the vertex Optional Adjacent Faces component is enabled.
 *
 * @note This function is available only if the Vertex Element has the OptionalAdjacentFaces
 * Component.
 *
 * @return true if the Optional Adjacent Faces is enabled, false otherwise.
 */
template<VertexConcept T>
bool VertexContainer<T>::isPerVertexAdjacentFacesEnabled() const requires vert::HasOptionalAdjacentFaces<T>
{
	return Base::template isOptionalComponentEnabled<typename T::AdjacentFaces>();
}

/**
 * @brief Enables the Optional Adjacent Faces of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalAdjacentFaces
 * Component.
 */
template<VertexConcept T>
void VertexContainer<T>::enablePerVertexAdjacentFaces() requires vert::HasOptionalAdjacentFaces<T>
{
	Base::template enableOptionalComponent<typename T::AdjacentFaces>();
}

/**
 * @brief Disables the Optional Adjacent Faces of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalAdjacentFaces
 * Component.
 */
template<VertexConcept T>
void VertexContainer<T>::disablePerVertexAdjacentFaces() requires vert::HasOptionalAdjacentFaces<T>
{
	Base::template disableOptionalComponent<typename T::AdjacentFaces>();
}

/**
 * @brief Checks if the vertex Optional Adjacent Vertices component is enabled.
 *
 * @note This function is available **only if the Vertex Element has the OptionalAdjacentVertices
 * Component.
 *
 * @return true if the Optional Adjacent Vertices is enabled, false otherwise.
 */
template<VertexConcept T>
bool VertexContainer<T>::isPerVertexAdjacentVerticesEnabled()
	const requires vert::HasOptionalAdjacentVertices<T>
{
	return Base::template isOptionalComponentEnabled<typename T::AdjacentVertices>();
}

/**
 * @brief Enables the Optional Adjacent Vertices of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalAdjacentVertices
 * Component.
 */
template<VertexConcept T>
void VertexContainer<T>::enablePerVertexAdjacentVertices()
	requires vert::HasOptionalAdjacentVertices<T>
{
	Base::template enableOptionalComponent<typename T::AdjacentVertices>();
}

/**
 * @brief Disables the Optional Adjacent Vertices of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalAdjacentVertices
 * Component.
 */
template<VertexConcept T>
void VertexContainer<T>::disablePerVertexAdjacentVertices()
	requires vert::HasOptionalAdjacentVertices<T>
{
	Base::template disableOptionalComponent<typename T::AdjacentVertices>();
}

/**
 * @brief Checks if the vertex Optional Color is enabled.
 *
 * @note This function is available only if the Vertex Element has the OptionalColor Component.
 *
 * @return true if the Optional Color is enabled, false otherwise.
 */
template<VertexConcept T>
bool VertexContainer<T>::isPerVertexColorEnabled() const requires vert::HasOptionalColor<T>
{
	return Base::template isOptionalComponentEnabled<typename T::Color>();
}

/**
 * @brief Enables the Optional Color of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalColor Component.
 */
template<VertexConcept T>
void VertexContainer<T>::enablePerVertexColor() requires vert::HasOptionalColor<T>
{
	return Base::template enableOptionalComponent<typename T::Color>();
}

/**
 * @brief Disables the Optional Color of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalColor Component.
 */
template<VertexConcept T>
void VertexContainer<T>::disablePerVertexColor() requires vert::HasOptionalColor<T>
{
	return Base::template disableOptionalComponent<typename T::Color>();
}

/**
 * @brief Checks if the vertex Optional Mark is enabled.
 *
 * @note This function is available only if the Vertex Element has the OptionalMark Component.
 *
 * @return true if the Optional Mark is enabled, false otherwise.
 */
template<VertexConcept T>
bool VertexContainer<T>::isPerVertexMarkEnabled() const requires vert::HasOptionalMark<T>
{
	return Base::template isOptionalComponentEnabled<typename T::Mark>();
}

/**
 * @brief Enables the Optional Mark of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalMark Component.
 */
template<VertexConcept T>
void VertexContainer<T>::enablePerVertexMark() requires vert::HasOptionalMark<T>
{
	return Base::template enableOptionalComponent<typename T::Mark>();
}

/**
 * @brief Container::disableVertexMark disables the Optional Mark of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalMark Component.
 */
template<VertexConcept T>
void VertexContainer<T>::disablePerVertexMark() requires vert::HasOptionalMark<T>
{
	return Base::template disableOptionalComponent<typename T::Mark>();
}

/**
 * @brief Checks if the vertex Optional Normal is enabled.
 *
 * @note This function is available only if the Vertex Element has the OptionalNormal
 * Component.
 *
 * @return true if the Optional Normal is enabled, false otherwise.
 */
template<VertexConcept T>
bool VertexContainer<T>::isPerVertexNormalEnabled() const requires vert::HasOptionalNormal<T>
{
	return Base::template isOptionalComponentEnabled<typename T::Normal>();
}

/**
 * @brief Enables the Optional Normal of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalNormal Component.
 */
template<VertexConcept T>
void VertexContainer<T>::enablePerVertexNormal() requires vert::HasOptionalNormal<T>
{
	return Base::template enableOptionalComponent<typename T::Normal>();
}

/**
 * @brief Disables the Optional Normal of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalNormal Component.
 */
template<VertexConcept T>
void VertexContainer<T>::disablePerVertexNormal() requires vert::HasOptionalNormal<T>
{
	return Base::template disableOptionalComponent<typename T::Normal>();
}

/**
 * @brief Checks if the vertex Optional PrincipalCurvature is enabled.
 *
 * @note This function is available only if the Vertex Element has the OptionalPrincipalCurvature
 * Component.
 *
 * @return true if the Optional PrincipalCurvature is enabled, false otherwise.
 */
template<VertexConcept T>
bool VertexContainer<T>::isPerVertexPrincipalCurvatureEnabled()
	const requires vert::HasOptionalPrincipalCurvature<T>
{
	return Base::template isOptionalComponentEnabled<typename T::PrincipalCurvature>();
}

/**
 * @brief Enables the Optional PrincipalCurvature of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalPrincipalCurvature
 * Component.
 */
template<VertexConcept T>
void VertexContainer<T>::enablePerVertexPrincipalCurvature()
	requires vert::HasOptionalPrincipalCurvature<T>
{
	return Base::template enableOptionalComponent<typename T::PrincipalCurvature>();
}

/**
 * @brief Disables the Optional PrincipalCurvature of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalPrincipalCurvature
 * Component.
 */
template<VertexConcept T>
void VertexContainer<T>::disablePerVertexPrincipalCurvature()
	requires vert::HasOptionalPrincipalCurvature<T>
{
	return Base::template disableOptionalComponent<typename T::PrincipalCurvature>();
}

/**
 * @brief Checks if the vertex Optional Quality is enabled.
 *
 * @note This function is available only if the Vertex Element has the OptionalQuality Component.
 *
 * @return true if the Optional Quality is enabled, false otherwise.
 */
template<VertexConcept T>
bool VertexContainer<T>::isPerVertexQualityEnabled() const requires vert::HasOptionalQuality<T>
{
	return Base::template isOptionalComponentEnabled<typename T::Quality>();
}

/**
 * @brief Enables the Optional Quality of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalQuality Component.
 */
template<VertexConcept T>
void VertexContainer<T>::enablePerVertexQuality() requires vert::HasOptionalQuality<T>
{
	return Base::template enableOptionalComponent<typename T::Quality>();
}

/**
 * @brief Disables the Optional Quality of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalQuality Component.
 */
template<VertexConcept T>
void VertexContainer<T>::disablePerVertexQuality() requires vert::HasOptionalQuality<T>
{
	return Base::template disableOptionalComponent<typename T::Quality>();
}

/**
 * @brief Checks if the vertex Optional TexCoord is enabled.
 *
 * @note This function is available only if the Vertex Element has the OptionalTexCoord Component.
 *
 * @return true if the Optional TexCoord is enabled, false otherwise.
 */
template<VertexConcept T>
bool VertexContainer<T>::isPerVertexTexCoordEnabled() const requires vert::HasOptionalTexCoord<T>
{
	return Base::template isOptionalComponentEnabled<typename T::TexCoord>();
}

/**
 * @brief Enables the Optional TexCoord of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalTexCoord Component.
 */
template<VertexConcept T>
void VertexContainer<T>::enablePerVertexTexCoord() requires vert::HasOptionalTexCoord<T>
{
	return Base::template enableOptionalComponent<typename T::TexCoord>();
}

/**
 * @brief Disables the Optional TexCoord of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalTexCoord Component.
 */
template<VertexConcept T>
void VertexContainer<T>::disablePerVertexTexCoord() requires vert::HasOptionalTexCoord<T>
{
	return Base::template disableOptionalComponent<typename T::TexCoord>();
}

/**
 * @brief Checks if vertices have a custom component with the given name.
 *
 * This function does not take into account the type of the custom component.
 *
 * @note This function is available only if the Vertex Element has the CustomComponents Component.
 *
 * @return `true` if the Vertex Element has a custom component with the given name.
 */
template<VertexConcept T>
bool VertexContainer<T>::hasPerVertexCustomComponent(const std::string& name) const
	requires vert::HasCustomComponents<T>
{
	return Base::hasElemCustomComponent(name);
}

/**
 * @brief Returns a vector containing all the names of the custom components of any type associated
 * to the Vertex Element.
 *
 * @note This function is available only if the Vertex Element has the CustomComponents Component.
 *
 * @return A vector of strings representing all the names of the custom components.
 */
template<VertexConcept T>
std::vector<std::string> VertexContainer<T>::perVertexCustomComponentNames() const
	requires vert::HasCustomComponents<T>
{
	return Base::elemCustomComponentNames();
}

/**
 * @brief Checks if the custom component of the Vertex Element having the given name has the same type
 * of the given template argument type of this function.
 *
 * For example, the following code checks if the component called `cc` is of type `double`:
 * @code{.cpp}
 * if (m.isPerVertexCustomComponentOfType<double>("cc")) {
 *   ...
 * }
 * @endcode
 *
 * @note This function is available only if the Vertex Element has the CustomComponents Component.
 *
 * @tparam K: the type of the custom component to check.
 * @param[in] name: the name of the custom component to check.
 * @throws std::out_of_range if no custom component of the given name was found.
 * @return `true` if the custom component is of the same type of the template argument.
 */
template<VertexConcept T>
template<typename K>
bool VertexContainer<T>::isPerVertexCustomComponentOfType(const std::string& name) const
	requires vert::HasCustomComponents<T>
{
	return Base::template isElemCustomComponentOfType<K>(name);
}

/**
 * @brief Returns the std::type_index of the custom component of the Vertex Element having the given
 * input name.
 *
 * @note This function is available only if the Vertex Element has the CustomComponents Component.
 *
 * @param[in] name: the name of the custom component to get the std::type_index from.
 * @throws std::out_of_range if no custom component of the given name was found.
 *
 * @return The std::type_index of the custom component having the given input name.
 */
template<vcl::VertexConcept T>
std::type_index VertexContainer<T>::perVertexCustomComponentType(const std::string& name) const
	requires vert::HasCustomComponents<T>
{
	return Base::elemComponentType(name);
}

/**
 * @brief Returns a vector containing all the names of the custom components associated to the
 * Vertex Element having the same type of the given template argument type of this function.
 *
 * For example, the following code gets a vector containing all the custom components of type
 * `double`:
 * @code{.cpp}
 * std::vector<std::string> cdouble = m.perVertexCustomComponentNamesOfType<double>();
 * @endcode
 *
 * @note This function is available only if the Vertex Element has the CustomComponents Component.
 *
 * @tparam K: the type of the custom component names.
 * @return A vector of strings representing the names of the custom components of a given type.
 */
template<VertexConcept T>
template<typename K>
std::vector<std::string>
VertexContainer<T>::perVertexCustomComponentNamesOfType()
	const requires vert::HasCustomComponents<T>
{
	return Base::template elemCustomComponentNamesOfType<K>();
}

/**
 * @brief Adds a custom component of type K to the Vertex, having the given name.
 *
 * @note This function is available only if the Vertex Element has the CustomComponents
 * Component.
 *
 * @tparam K: the type of the custom component added to the Vertex.
 * @param[in] name: the name of the custom component added to the Vertex.
 */
template<VertexConcept T>
template<typename K>
void VertexContainer<T>::addPerVertexCustomComponent(const std::string& name)
	requires vert::HasCustomComponents<T>
{
	Base::template addElemCustomComponent<K>(name);
}

/**
 * @brief Deletes the custom component of the given name from the Vertex Element.
 *
 * The function does nothing if the custom component does not exists.
 *
 * @note This function is available only if the Vertex Element has the CustomComponents Component.
 *
 * @param[in] name: the name of the custom component that will be removed from the Vertex.
 */
template<VertexConcept T>
void VertexContainer<T>::deletePerVertexCustomComponent(const std::string& name)
	requires vert::HasCustomComponents<T>
{
	Base::deleteElemCustomComponent(name);
}

/**
 * @brief Returns a vector handle to the given custom component. The handle can be used like a
 * normal std::vector, but does not have access to the modifiers member functions (resize,
 * push_back...). The handle contains **references** to the custom component, therefore you can
 * modify the custom component by modifying the element of the handle vector normally. Since
 * the handle stores references, there are no copies performed when calling this function.
 *
 * For example, assuming that the mesh has a vertex custom component named "cc" of type int:
 *
 * @code{.cpp}
 * auto handle = m.perVertexCustomComponentVectorHandle<int>("cc");
 * for (Vertex& v : m.vertices() {
 *    handle[m.index(v)] = 5; // v.customComponent<int>("cc") == 5
 *    assert(v.customComponent<int>("cc") == 5);
 * }
 * @endcode
 *
 * Using handles allows to access more efficiently to custom components rather accessing from an
 * element object. However, note that references are binded to the container of the mesh.
 *
 * @note This function is available only if the Vertex Element has the CustomComponents Component.
 *
 * @note Since the handle contains references, any operation that changes the size of the container
 * could be destructive and invalidate the references contained in the handle.
 *
 * @tparam K: the type of the custom component on which return the handle.
 *
 * @param[in] name: name of the custom component on which return the handle.
 * @throws std::out_of_range if no custom component of the given name was found.
 * 
 * @return a vector handle that allows to access to the custom component.
 */
template<VertexConcept T>
template<typename K>
CustomComponentVectorHandle<K> VertexContainer<T>::perVertexCustomComponentVectorHandle(
	const std::string& name) requires vert::HasCustomComponents<T>
{
	return Base::template customComponentVectorHandle<K>(name);
}

/**
 * @brief Returns a const vector handle to the given custom component. The handle can be used like a
 * normal std::vector, but does not have access to the modifiers member functions (resize,
 * push_back...). The handle contains **const references** to the custom component, therefore you
 * can access to the custom component by accessing the element of the handle vector normally. Since
 * the handle stores references, there are no copies performed when calling this function.
 *
 * For example, assuming that the mesh has a vertex custom component named "cc" of type int:
 *
 * @code{.cpp}
 * // access to the const handle by making const the template parameter:
 * auto handle = m.perVertexCustomComponentVectorHandle<const int>("cc");
 * int sum = 0;
 * for (const Vertex& v : m.vertices() {
 *    sum += handle[m.index(v)];
 *    // handle[m.index(v)] = 5; // not allowed, because the handle is const
 * }
 * @endcode
 *
 * Using handles allows to access more efficiently to custom components rather accessing from an
 * element object. However, note that references are binded to the container of the mesh.
 *
 * @note This function is available only if the Vertex Element has the CustomComponents Component.
 *
 * @note Since the handle contains references, any operation that changes the size of the container
 * could be destructive and invalidate the references contained in the handle.
 *
 * @tparam K: the type of the custom component on which return the handle.
 *
 * @param[in] name: name of the custom component on which return the handle.
 * @throws std::out_of_range if no custom component of the given name was found.
 * 
 * @return a const vector handle that allows to access to the custom component.
 */
template<VertexConcept T>
template<typename K>
ConstCustomComponentVectorHandle<K>
VertexContainer<T>::perVertexCustomComponentVectorHandle(const std::string& name) const
	requires vert::HasCustomComponents<T>
{
	return Base::template customComponentVectorHandle<K>(name);
}

} // namespace vcl::mesh
