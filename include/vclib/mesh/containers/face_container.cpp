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

#include "face_container.h"

namespace vcl::mesh {

/**
 * @brief Empty constructor that creates an empty container of Faces.
 */
template<FaceConcept T>
FaceContainer<T>::FaceContainer()
{
}

/**
 * @brief Returns a const reference of the face at the i-th position in the Face Container of the
 * Mesh, which will be the face having index = i.
 *
 * This function does not perform any sanity check: if i is less than faceContainerSize(), this
 * function will return a valid Face reference (note that the Face may have been flagged as
 * deleted).
 *
 * @param[in] i: the index of the face that will be returned.
 */
template<FaceConcept T>
const typename FaceContainer<T>::FaceType& FaceContainer<T>::face(uint i) const
{
	return Base::element(i);
}

/**
 * @brief Returns a reference of the face at the i-th position in the Face Container of the Mesh,
 * which will be the face having index = i.
 *
 * This function does not perform any sanity check: if i is less than faceContainerSize(), this
 * function will return a valid Face reference (note that the Face may have been flagged as
 * deleted).
 *
 * @param[in] i: the index of the face that will be returned.
 */
template<FaceConcept T>
typename FaceContainer<T>::FaceType& FaceContainer<T>::face(uint i)
{
	return Base::element(i);
}

/**
 * @brief Returns the number of **non-deleted** faces contained in the Face container of the Mesh.
 *
 * If faceNumber() != faceContainerSize(), it means that there are some faces that are
 * flagged as deleted.
 *
 * @return The number of non-deleted faces of the Mesh.
 */
template<FaceConcept T>
uint FaceContainer<T>::faceNumber() const
{
	return Base::elementNumber();
}

/**
 * @brief Returns the number of faces (also deleted) contained in the Face container of the Mesh.
 *
 * If faceNumber() != faceContainerSize(), it means that there are some faces that are
 * flagged as deleted.
 *
 * @return The number of all the faces contained in the Mesh.
 */
template<FaceConcept T>
uint FaceContainer<T>::faceContainerSize() const
{
	return Base::elementContainerSize();
}

/**
 * @brief Returns the number of deleted faces in the Face container, that is faceContainerSize() -
 * faceNumber().
 *
 * @return The number of deleted faces in the container.
 */
template<FaceConcept T>
uint FaceContainer<T>::deletedFaceNumber() const
{
	return Base::deletedElementNumber();
}

template<FaceConcept T>
uint FaceContainer<T>::addFace()
{
	return Base::addElement();
}

template<FaceConcept T>
template<typename... V>
uint FaceContainer<T>::addFace(V... args) requires (sizeof...(args) >= 3)
{
	uint  fid = addFace();
	Face& f   = face(fid);

	constexpr uint n = sizeof...(args);

	if constexpr (T::VERTEX_NUMBER < 0) {
		f.resizeVertices(n);
	}
	else {
		static_assert(n == T::VERTEX_NUMBER, "Wrong number of vertices in Mesh::addFace.");
	}

	addFaceHelper(f, args...);
	return fid;
}

template<FaceConcept T>
template<typename Iterator>
uint FaceContainer<T>::addFace(Iterator begin, Iterator end)
{
	if (begin == end) return UINT_NULL;
	uint n = std::distance(begin, end);

	uint fid = UINT_NULL;

	assert(n >= 3);
	if (n < 3) return UINT_NULL;

	if constexpr (T::VERTEX_NUMBER < 0) {
		fid = addFace();
		face(fid).resizeVertices(n);
	}
	else {
		assert(n == T::VERTEX_NUMBER);
		if (n == T::VERTEX_NUMBER)
			fid = addFace();
	}

	if (fid != UINT_NULL) {
		Face& f = face(fid);

		unsigned int i = 0;
		for (Iterator it = begin; it != end; ++it) {
			if constexpr (std::integral<typename Iterator::value_type>)
				f.vertex(i) = &Base::parentMesh->vertex(*it);
			else
				f.vertex(i) = *it;
			++i;
		}
	}
	return fid;
}

template<FaceConcept T>
uint FaceContainer<T>::addFaces(uint n)
{
	return Base::addElements(n);
}

template<FaceConcept T>
void FaceContainer<T>::reserveFaces(uint n)
{
	return Base::reserveElements(n);
}

template<FaceConcept T>
void FaceContainer<T>::compactFaces()
{
	return Base::compactElements();
}

/**
 * @brief Marks as deleted the face with the given id.
 *
 * This member function does not perform any reallocation of the faces: the deleted faces
 * will stay in the Face Container, but will be marked as deleted.
 *
 * Deleted faces are automatically jumped by the iterators provided by the Face Container.
 *
 * @param[in] i: the id of the face that will be marked as deleted.
 */
template<FaceConcept T>
void FaceContainer<T>::deleteFace(uint i)
{
	Base::deleteElement(i);
}

/**
 * @brief Marks as deleted the given face, before asserting that the face belongs to this container.
 *
 * This member function does not perform any reallocation of the faces: the deleted faces
 * will stay in the Face Container, but will be marked as deleted.
 *
 * Deleted faces are automatically jumped by the iterators provided by the Face Container.
 *
 * @param[in] f: the pointer of the face that will be marked as deleted.
 */
template<FaceConcept T>
void FaceContainer<T>::deleteFace(const FaceType* f)
{
	Base::deleteElement(f);
}

/**
 * @brief This is an utility member function that returns the index of an element if the container
 * would be compact, that is the number of non-deleted elements before the face with the given
 * index.
 *
 * Complexity: O(n), with n the number of faces in the container.
 *
 * This function does not perform any sanity check on the given index.
 *
 * @param[in] i: the index of a face of the container.
 * @return The index that the face with index i would have if this container would be compact.
 */
template<FaceConcept T>
uint FaceContainer<T>::faceIndexIfCompact(uint i) const
{
	return Base::elementIndexIfCompact(i);
}

/**
 * @brief Returns a vector that tells, for each actual face index, the new index that the face
 * would have in a compacted container. For each deleted face index, the value of the vector will
 * be -1.
 *
 * This is useful if you need to know the indices of the faces that they would have in a
 * compact container, without considering the deleted ones.
 *
 * @return A vector containing, for each face index, its index if the container would be compact.
 */
template<FaceConcept T>
std::vector<int> FaceContainer<T>::faceCompactIndices() const
{
	return Base::elementCompactIndices();
}

/**
 * @brief Returns an iterator to the beginning of the container.
 *
 * The iterator is automatically initialized to jump deleted faces of the container. You can change
 * this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted faces.
 * @return An iterator the the first face of the container.
 */
template<FaceConcept T>
typename FaceContainer<T>::FaceIterator FaceContainer<T>::faceBegin(bool jumpDeleted)
{
	return Base::elementBegin(jumpDeleted);
}

/**
 * @brief Returns an iterator to the end of the container.
 * @return An iterator to the end of the container.
 */
template<FaceConcept T>
typename FaceContainer<T>::FaceIterator FaceContainer<T>::faceEnd()
{
	return Base::elementEnd();
}

/**
 * @brief Returns a const iterator to the beginning of the container.
 *
 * The iterator is automatically initialized to jump deleted faces of the container. You can change
 * this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted faces.
 * @return A const iterator the the first face of the container.
 */
template<FaceConcept T>
typename FaceContainer<T>::ConstFaceIterator FaceContainer<T>::faceBegin(bool jumpDeleted) const
{
	return Base::elementBegin(jumpDeleted);
}

/**
 * @brief Returns a const iterator to the end of the container.
 * @return A const iterator to the end of the container.
 */
template<FaceConcept T>
typename FaceContainer<T>::ConstFaceIterator FaceContainer<T>::faceEnd() const
{
	return Base::elementEnd();
}

/**
 * @brief Returns a small utility object that allows to iterate over the faces of the containers,
 * providing two member functions begin() and end().
 *
 * This member function is very useful when you want to iterate over the faces using the C++ foreach
 * syntax:
 *
 * @code{.cpp}
 * for (Face& f : m.faces()){
 *     // do something with this face
 * }
 * @endcode
 *
 * The iterator used to iterate over faces is automatically initialized to jump deleted faces of the
 * container. You can change this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted faces.
 * @return An object having begin() and end() function, allowing to iterate over the container.
 */
template<FaceConcept T>
auto FaceContainer<T>::faces(bool jumpDeleted)
{
	return Base::elements(jumpDeleted);
}

/**
 * @brief Returns a small utility object that allows to iterate over the faces of the containers,
 * providing two member functions begin() and end().
 *
 * This member function is very useful when you want to iterate over the faces using the C++ foreach
 * syntax:
 *
 * @code{.cpp}
 * for (const Face& f : m.faces()){
 *     // do something with this face
 * }
 * @endcode
 *
 * The iterator used to iterate over faces is automatically initialized to jump deleted faces of the
 * container. You can change this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted faces.
 * @return An object having begin() and end() function, allowing to iterate over the container.
 */
template<FaceConcept T>
auto FaceContainer<T>::faces(bool jumpDeleted) const
{
	return Base::elements(jumpDeleted);
}

/**
 * @brief Enables all the optional components associated to the Face type contained in the
 * FaceContainer.
 */
template<FaceConcept T>
void FaceContainer<T>::enableAllPerFaceOptionalComponents()
{
	Base::enableAllOptionalComponents();
}

/**
 * @brief Disables all the optional components associated to the Face type contained in the
 * FaceContainer.
 */
template<FaceConcept T>
void FaceContainer<T>::disableAllPerFaceOptionalComponents()
{
	Base::disableAllOptionalComponents();
}

/**
 * @brief Checks if the face Optional Adjacent Edges is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalAdjacentEdges
 * Component.
 *
 * @return true if the Optional AdjacentEdges is enabled, false otherwise.
 */
template<FaceConcept T>
bool FaceContainer<T>::isPerFaceAdjacentEdgesEnabled()
	const requires face::HasOptionalAdjacentEdges<T>
{
	return Base::template isOptionalComponentTypeEnabled<typename T::AdjacentEdges>();
}

/**
 * @brief Enable the Optional Adjacent Edges of the face.
 *
 * @note This function is available only if the Face Element has the OptionalAdjacentEdges
 * Component.
 *
 * @note If the Face is polygonal (dynamic size, N < 0), when enabled, the adjacent edges number
 * will be the same of the vertex number for each face of the container. This is because, for Faces,
 * Adjacent Edges number is tied to the number of vertices.
 */
template<FaceConcept T>
void FaceContainer<T>::enablePerFaceAdjacentEdges() requires face::HasOptionalAdjacentEdges<T>
{
	Base::template enableOptionalComponentType<typename T::AdjacentEdges>();
}

/**
 * @brief Disables the Optional Adjacent Edges of the face.
 *
 * @note This function is available only if the Face Element has the OptionalAdjacentEdges
 * Component.
 */
template<FaceConcept T>
void FaceContainer<T>::disablePerFaceAdjacentEdges() requires face::HasOptionalAdjacentEdges<T>
{
	Base::template disableOptionalComponentType<typename T::AdjacentEdges>();
}

/**
 * @brief Checks if the face Optional Adjacent Faces is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalAdjacentFaces
 * Component.
 *
 * @return true if the Optional AdjacentFaces is enabled, false otherwise.
 */
template<FaceConcept T>
bool FaceContainer<T>::isPerFaceAdjacentFacesEnabled()
	const requires face::HasOptionalAdjacentFaces<T>
{
	return Base::template isOptionalComponentTypeEnabled<typename T::AdjacentFaces>();
}

/**
 * @brief Enable the Optional Adjacent Faces of the face.
 *
 * @note This function is available only if the Face Element has the OptionalAdjacentFaces
 * Component.
 *
 * @note If the Face is polygonal (dynamic size, N < 0), when enabled, the adjacent faces number
 * will be the same of the vertex number for each face of the container. This is because, for Faces,
 * Adjacent Faces number is tied to the number of vertices.
 */
template<FaceConcept T>
void FaceContainer<T>::enablePerFaceAdjacentFaces() requires face::HasOptionalAdjacentFaces<T>
{
	Base::template enableOptionalComponentType<typename T::AdjacentFaces>();
}

/**
 * @brief Disables the Optional Adjacent Faces of the face.
 *
 * @note This function is available only if the Face Element has the OptionalAdjacentFaces
 * Component.
 */
template<FaceConcept T>
void FaceContainer<T>::disablePerFaceAdjacentFaces() requires face::HasOptionalAdjacentFaces<T>
{
	Base::template disableOptionalComponentType<typename T::AdjacentFaces>();
}

/**
 * @brief Checks if the face Optional Color is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalColor component.
 *
 * @return true if the Optional Color is enabled, false otherwise.
 */
template<FaceConcept T>
bool FaceContainer<T>::isPerFaceColorEnabled() const requires face::HasOptionalColor<T>
{
	return Base::template isOptionalComponentTypeEnabled<typename T::Color>();
}

/**
 * @brief Enables the Optional Color of the face.
 *
 * @note This function is available only if the Face Element has the OptionalColor Component.
 */
template<FaceConcept T>
void FaceContainer<T>::enablePerFaceColor() requires face::HasOptionalColor<T>
{
	return Base::template enableOptionalComponentType<typename T::Color>();
}

/**
 * @brief Disables the Optional Color of the face.
 *
 * @note This function is available only if the Face Element has the OptionalColor Component.
 */
template<FaceConcept T>
void FaceContainer<T>::disablePerFaceColor() requires face::HasOptionalColor<T>
{
	return Base::template disableOptionalComponentType<typename T::Color>();
}

/**
 * @brief Checks if the face Optional Mark is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalMark component.
 *
 * @return true if the Optional Mark is enabled, false otherwise.
 */
template<FaceConcept T>
bool FaceContainer<T>::isPerFaceMarkEnabled() const requires face::HasOptionalMark<T>
{
	return Base::template isOptionalComponentTypeEnabled<typename T::Mark>();
}

/**
 * @brief Enables the Optional Mark of the face.
 *
 * @note This function is available only if the Face Element has the OptionalMark Component.
 */
template<FaceConcept T>
void FaceContainer<T>::enablePerFaceMark() requires face::HasOptionalMark<T>
{
	return Base::template enableOptionalComponentType<typename T::Mark>();
}

/**
 * @brief Disables the Optional Mark of the face.
 *
 * @note This function is available only if the Face Element has the OptionalMark Component.
 */
template<FaceConcept T>
void FaceContainer<T>::disablePerFaceMark() requires face::HasOptionalMark<T>
{
	return Base::template disableOptionalComponentType<typename T::Mark>();
}

/**
 * @brief Checks if the face Optional Normal is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalNormal Component.
 *
 * @return true if the Optional Normal is enabled, false otherwise.
 */
template<FaceConcept T>
bool FaceContainer<T>::isPerFaceNormalEnabled() const requires face::HasOptionalNormal<T>
{
	return Base::template isOptionalComponentTypeEnabled<typename T::Normal>();
}

/**
 * @brief Enables the Optional Normal of the face.
 *
 * @note This function is available only if the Face Element has the OptionalNormal Component.
 */
template<FaceConcept T>
void FaceContainer<T>::enablePerFaceNormal() requires face::HasOptionalNormal<T>
{
	return Base::template disableOptionalComponentType<typename T::Normal>();
}

/**
 * @brief Disables the Optional Normal of the face.
 *
 * @note This function is available only if the Face Element has the OptionalNormal Component.
 */
template<FaceConcept T>
void FaceContainer<T>::disablePerFaceNormal() requires face::HasOptionalNormal<T>
{
	return Base::template disableOptionalComponent<typename T::Normal>();
}

/**
 * @brief Checks if the face Optional PrincipalCurvature is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalPrincipalCurvature
 * Component.
 *
 * @return true if the Optional PrincipalCurvature is enabled, false otherwise.
 */
template<FaceConcept T>
bool FaceContainer<T>::isPerFacePrincipalCurvatureEnabled()
	const requires face::HasOptionalPrincipalCurvature<T>
{
	return Base::template isOptionalComponentTypeEnabled<typename T::PrincipalCurvature>();
}
/**
 * @brief Enables the Optional PrincipalCurvature of the face.
 *
 * @note This function is available only if the Face Element has the OptionalPrincipalCurvature
 * Component.
 */
template<FaceConcept T>
void FaceContainer<T>::enablePerFacePrincipalCurvature()
	requires face::HasOptionalPrincipalCurvature<T>
{
	return Base::template enableOptionalComponentType<typename T::PrincipalCurvature>();
}

/**
 * @brief Disables the Optional PrincipalCurvature of the face.
 *
 * @note This function is available only if the Face Element has the OptionalPrincipalCurvature
 * Component.
 */
template<FaceConcept T>
void FaceContainer<T>::disablePerFacePrincipalCurvature()
	requires face::HasOptionalPrincipalCurvature<T>
{
	return Base::template disableOptionalComponentType<typename T::PrincipalCurvature>();
}

/**
 * @brief Checks if the face Optional Quality is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalQuality Component.
 *
 * @return true if the Optional Quality is enabled, false otherwise.
 */
template<FaceConcept T>
bool FaceContainer<T>::isPerFaceQualityEnabled() const requires face::HasOptionalQuality<T>
{
	return Base::template isOptionalComponentTypeEnabled<typename T::Quality>();
}
/**
 * @brief Enables the Optional Quality of the face.
 *
 * @note This function is available only if the Face Element has the OptionalQuality Component.
 */
template<FaceConcept T>
void FaceContainer<T>::enablePerFaceQuality() requires face::HasOptionalQuality<T>
{
	return Base::template enableOptionalComponentType<typename T::Quality>();
}

/**
 * @brief Disables the Optional Quality of the face.
 *
 * @note This function is available only if the Face Element has the OptionalQuality Component.
 */
template<FaceConcept T>
void FaceContainer<T>::disablePerFaceQuality() requires face::HasOptionalQuality<T>
{
	return Base::template disableOptionalComponentType<typename T::Quality>();
}

/**
 * @brief Checks if the face Optional WedgeColors Faces is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalWedgeColors Component.
 *
 * @return true if the Optional WedgeColors is enabled, false otherwise.
 */
template<FaceConcept T>
bool FaceContainer<T>::isPerFaceWedgeColorsEnabled() const requires face::HasOptionalWedgeColors<T>
{
	return Base::template isOptionalComponentTypeEnabled<typename T::WedgeColors>();
}

/**
 * @brief Enable the Optional Wedge Colors of the face.
 *
 * @note This function is available only if the Face Element has the OptionalWedgeColors Component.
 *
 * @note If the Face is polygonal (dynamic size, N < 0), when enabled, the wedge colors number will
 * be the same of the vertex number for each face of the container. This is because, for Faces,
 * Wedge Colors number is tied to the number of vertices.
 */
template<FaceConcept T>
void FaceContainer<T>::enablePerFaceWedgeColors() requires face::HasOptionalWedgeColors<T>
{
	Base::template enableOptionalComponentType<typename T::WedgeColors>();
}

/**
 * @brief Disables the Optional Wedge Colors of the face.
 *
 * @note This function is available only if the Face Element has the OptionalWedgeColors Component.
 */
template<FaceConcept T>
void FaceContainer<T>::disablePerFaceWedgeColors() requires face::HasOptionalWedgeColors<T>
{
	Base::template disableOptionalComponentType<typename T::WedgeColors>();
}

/**
 * @brief Checks if the face Optional WedgeTexCoords Faces is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalWedgeTexCoords
 * Component.
 * @return true if the Optional WedgeTexCoords is enabled, false otherwise.
 */
template<FaceConcept T>
bool FaceContainer<T>::isPerFaceWedgeTexCoordsEnabled()
	const requires face::HasOptionalWedgeTexCoords<T>
{
	return Base::template isOptionalComponentTypeEnabled<typename T::WedgeTexCoords>();
}

/**
 * @brief Enables the Optional Wedge TexCoords of the face.
 *
 * @note This function is available only if the Face Element has the OptionalWedgeTexCoords
 * Component.
 *
 * @note If the Face is polygonal (dynamic size, N < 0), when enabled, the wedge texcoord number
 * will be the same of the vertex number for each face of the container. This is because, for Faces,
 * Wedge TexCoords number is tied to the number of vertices.
 */
template<FaceConcept T>
void FaceContainer<T>::enablePerFaceWedgeTexCoords() requires face::HasOptionalWedgeTexCoords<T>
{
	Base::template enableOptionalComponentType<typename T::WedgeTexCoords>();
}

/**
 * @brief Disables the Optional WedgeTex Coords of the face.
 *
 * @note This function is available only if the Face Element has the OptionalWedgeTexCoords
 * Component.
 */
template<FaceConcept T>
void FaceContainer<T>::disablePerFaceWedgeTexCoords() requires face::HasOptionalWedgeTexCoords<T>
{
	Base::template disableOptionalComponentType<typename T::WedgeTexCoords>();
}

/**
 * @brief Checks if faces have a custom component with the given name.
 *
 * This function does not take into account the type of the custom component.
 *
 * @note This function is available only if the Face Element has the CustomComponents Component.
 *
 * @return `true` if the Face Element has a custom component with the given name.
 */
template<FaceConcept T>
bool FaceContainer<T>::hasPerFaceCustomComponent(const std::string& name) const
	requires face::HasCustomComponents<T>
{
	return Base::hasElemCustomComponent(name);
}

/**
 * @brief Returns a vector containing all the names of the custom components of any type associated
 * to the Face Element.
 *
 * @note This function is available only if the Face Element has the CustomComponents Component.
 *
 * @return A vector of strings representing all the names of the custom components.
 */
template<FaceConcept T>
std::vector<std::string> FaceContainer<T>::perFaceCustomComponentNames() const
	requires face::HasCustomComponents<T>
{
	return Base::elemCustomComponentNames();
}

/**
 * @brief Checks if the custom component of the Face Element having the given name has the same type
 * of the given template argument type of this function.
 *
 * For example, the following code checks if the component called `cc` is of type `double`:
 * @code{.cpp}
 * if (m.isPerFaceCustomComponentOfType<double>("cc")) {
 *   ...
 * }
 * @endcode
 *
 * @note This function is available only if the Face Element has the CustomComponents Component.
 *
 * @tparam K: the type of the custom component to check.
 *
 * @param[in] name: the name of the custom component to check.
 * @throws std::out_of_range if no custom component of the given name was found.
 *
 * @return `true` if the custom component is of the same type of the template argument.
 */
template<FaceConcept T>
template<typename K>
bool FaceContainer<T>::isPerFaceCustomComponentOfType(const std::string& name) const
	requires face::HasCustomComponents<T>
{
	return Base::template isElemCustomComponentOfType<K>(name);
}

/**
 * @brief Returns the std::type_index of the custom component of the Face Element having the given
 * input name.
 *
 * @note This function is available only if the Face Element has the CustomComponents Component.
 *
 * @param[in] name: the name of the custom component to get the std::type_index from.
 * @throws std::out_of_range if no custom component of the given name was found.
 *
 * @return The std::type_index of the custom component having the given input name.
 */
template<vcl::FaceConcept T>
std::type_index FaceContainer<T>::perFaceCustomComponentType(const std::string& name) const
	requires face::HasCustomComponents<T>
{
	return Base::elemComponentType(name);
}

/**
 * @brief Returns a vector containing all the names of the custom components associated to the Face
 * Element having the same type of the given template argument type of this function.
 *
 * For example, the following code gets a vector containing all the custom components of type
 * `double`:
 * @code{.cpp}
 * std::vector<std::string> cdouble = m.perFaceCustomComponentNamesOfType<double>();
 * @endcode
 *
 * @note This function is available only if the Face Element has the CustomComponents Component.
 *
 * @tparam K: the type of the custom component names.
 * @return A vector of strings representing the names of the custom components of a given type.
 */
template<FaceConcept T>
template<typename K>
std::vector<std::string> FaceContainer<T>::perFaceCustomComponentNamesOfType() const
	requires face::HasCustomComponents<T>
{
	return Base::template elemCustomComponentNamesOfType<K>();
}

/**
 * @brief Adds a custom component of type K to the Face, having the given name.
 *
 * @note This function is available only if the Face Element has the CustomComponents Component.
 *
 * @tparam K: the type of the custom component added to the Face.
 * @param[in] name: the name of the custom component added to the Face.
 */
template<FaceConcept T>
template<typename K>
void FaceContainer<T>::addPerFaceCustomComponent(const std::string& name)
	requires face::HasCustomComponents<T>
{
	Base::template addElemCustomComponent<K>(name);
}

/**
 * @brief Deletes the custom component of the given name from the Face Element.
 *
 * The function does nothing if the custom component does not exists.
 *
 * @note This function is available only if the Face Element has the CustomComponents Component.
 *
 * @param[in] name: the name of the custom component that will be removed from the Face.
 */
template<FaceConcept T>
void FaceContainer<T>::deletePerFaceCustomComponent(const std::string& name)
	requires face::HasCustomComponents<T>
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
 * For example, assuming that the mesh has a face custom component named "cc" of type int:
 *
 * @code{.cpp}
 * auto handle = m.perFaceCustomComponentVectorHandle<int>("cc");
 * for (Face& f : m.faces() {
 *    handle[m.index(f)] = 5; // f.customComponent<int>("cc") == 5
 *    assert(f.customComponent<int>("cc") == 5);
 * }
 * @endcode
 *
 * Using handles allows to access more efficiently to custom components rather accessing from an
 * element object. However, note that references are binded to the container of the mesh.
 *
 * @note Since the handle contains references, any operation that changes the size of the container
 * could be destructive and invalidate the references contained in the handle.
 *
 * @tparam K: the type of the custom component on which return the handle.
 *
 * @throws std::out_of_range if no custom component of the given name was found.
 * @param name: name of the custom component on which return the handle.
 *
 * @return a vector handle that allows to access to the custom component.
 */
template<FaceConcept T>
template<typename K>
CustomComponentVectorHandle<K>
FaceContainer<T>::perFaceCustomComponentVectorHandle(const std::string& name)
	requires face::HasCustomComponents<T>
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
 * For example, assuming that the mesh has a face custom component named "cc" of type int:
 *
 * @code{.cpp}
 * // access to the const handle by making const the template parameter:
 * auto handle = m.perFaceCustomComponentVectorHandle<const int>("cc");
 * int sum = 0;
 * for (const Face& f : m.faces() {
 *    sum += handle[m.index(f)];
 *    // handle[m.index(f)] = 5; // not allowed, because the handle is const
 * }
 * @endcode
 *
 * Using handles allows to access more efficiently to custom components rather accessing from an
 * element object. However, note that references are binded to the container of the mesh.
 *
 * @note Since the handle contains references, any operation that changes the size of the container
 * could be destructive and invalidate the references contained in the handle.
 *
 * @tparam K: the type of the custom component on which return the handle.
 *
 * @throws std::out_of_range if no custom component of the given name was found.
 * @param name: name of the custom component on which return the handle.
 *
 * @return a const vector handle that allows to access to the custom component.
 */
template<FaceConcept T>
template<typename K>
ConstCustomComponentVectorHandle<K> FaceContainer<T>::perFaceCustomComponentVectorHandle(
	const std::string& name) const requires face::HasCustomComponents<T>
{
	return Base::template customComponentVectorHandle<K>(name);
}

template<FaceConcept F>
void FaceContainer<F>::addFaceHelper(F&)
{
	// base case: no need to add any other vertices
}

template<FaceConcept F>
template<typename... V>
void FaceContainer<F>::addFaceHelper(F& f, typename F::VertexType* v, V... args)
{
	// position on which add the vertex
	const std::size_t n = f.vertexNumber() - sizeof...(args) - 1;
	f.vertex(n)         = v;   // set the vertex
	addFaceHelper(f, args...); // set the remanining vertices, recursive variadics
}

template<FaceConcept F>
template<typename... V>
void FaceContainer<F>::addFaceHelper(F& f, uint vid, V... args)
{
	// position on which add the vertex
	const std::size_t n = f.vertexNumber() - sizeof...(args) - 1;
	f.vertex(n)         = &Base::parentMesh->vertex(vid); // set the vertex
	addFaceHelper(f, args...); // set the remanining vertices, recursive variadics
}

} // namespace vcl::mesh
