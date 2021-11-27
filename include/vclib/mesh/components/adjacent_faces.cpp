﻿/*****************************************************************************
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

#include "adjacent_faces.h"

namespace vcl::comp {

/**
 * @brief Creates a Container of Adjacent Faces, stored as Face Pointers.
 * If the size of the container is static, all the values will be set to nullptr.
 * If the size is dynamic, the number of values in the container is zero.
 */
template<typename Face, int N>
AdjacentFaces<Face, N>::AdjacentFaces() : Base()
{
}

/**
 * @brief Returns the number of adjacent faces of this element.
 * @return The number of adjacent faces of this element.
 */
template<typename Face, int N>
uint AdjacentFaces<Face, N>::adjFacesNumber() const
{
	return Base::size();
}

/**
 * @brief Returns a reference of the pointer to the i-th adjacent face of this element.
 *
 * You can use this function to set the i-th adjacent face:
 *
 * @code{.cpp}
 * e.adjFace(2) = &m.face(k); // the second adj face of e will point to the k-th face of the mesh.
 * @endcode
 *
 * @param[in] i: the position of the required adjacent face in the container; the value must be
 * between 0 and the number of adj faces.
 * @return The pointer to the i-th adjacent face of this element.
 */
template<typename Face, int N>
Face*& AdjacentFaces<Face, N>::adjFace(uint i)
{
	return Base::at(i);
}

/**
 * @brief Returns a const pointer to the i-th adjacent face of this element.
 * @param[in] i: the position of the required adjacent face in the container; the value must be
 * between 0 and the number of adj faces.
 * @return The pointer to the i-th adjacent face of this element.
 */
template<typename Face, int N>
const Face* AdjacentFaces<Face, N>::adjFace(uint i) const
{
	return Base::at(i);
}

/**
 * @brief Returns a reference of the pointer to the i-th adjacent face of this element but using as
 * index the module between i and the number of adjacent faces. You can use this function if you
 * need to get the "next adjacent face after position k", without check if it is less than the
 * number of adj faces. Works also for negative numbers:
 *
 * @code{.cpp}
 * k = pos; // some position of an adjacent face
 * auto* next = e.adjFaceMod(k+1); // the adj face next to k, that may also be at pos 0
 * auto* last = e.adjFaceMod(-1); // the adj face in position adjFaceNumber()-1
 * @endcode
 *
 * @param[in] i: the position of the required adjacent face in the container, w.r.t. the position 0;
 * value is modularized on adjFaceNumber().
 * @return The pointer to the required adjacent face of this element.
 */
template<typename Face, int N>
Face*& AdjacentFaces<Face, N>::adjFaceMod(int i)
{
	return Base::atMod(i);
}

/**
 * @brief Same of adjFaceMod, but returns a const Pointer to the adjacent face.
 * @param[in] i: the position of the required adjacent face in the container, w.r.t. the position 0;
 * value is modularized on adjFaceNumber().
 * @return The pointer to the required adjacent face of this element.
 */
template<typename Face, int N>
const Face* AdjacentFaces<Face, N>::adjFaceMod(int i) const
{
	return Base::atMod(i);
}

/**
 * @brief Sets the i-th adjacent face of this element.
 * @param[in] f: The pointer to the adjacent face to set to this element.
 * @param[in] i: the position in the container on which set the adj face; the value must be
 * between 0 and the number of adj faces.
 */
template<typename Face, int N>
void AdjacentFaces<Face, N>::setAdjFace(Face* f, uint i)
{
	Base::set(f, i);
}

/**
 * @brief Sets all the adjacent faces of this element.
 *
 * If the size of the container is static, the size of the input vector must be the same one of the
 * container.
 *
 * @param[in] list: vector of adjacent faces to set.
 */
template<typename Face, int N>
void AdjacentFaces<Face, N>::setAdjFaces(const std::vector<Face*>& list)
{
	Base::set(list);
}

template<typename Face, int N>
bool AdjacentFaces<Face, N>::containsAdjFace(const Face* f) const
{
	return Base::contains(f);
}

template<typename Face, int N>
typename AdjacentFaces<Face, N>::AdjacentFaceIterator
AdjacentFaces<Face, N>::findAdjFace(const Face* f)
{
	return Base::find(f);
}

template<typename Face, int N>
typename AdjacentFaces<Face, N>::ConstAdjacentFaceIterator
AdjacentFaces<Face, N>::findAdjFace(const Face* f) const
{
	return Base::find(f);
}

template<typename Face, int N>
int AdjacentFaces<Face, N>::indexOfAdjFace(const Face* f) const
{
	return Base::indexOf(f);
}

/**
 * @brief Resize the container of the adjacent faces to the given size.
 * @note This function is available only if the container of the Adjacent Faces is has dynamic
 * size.
 * @param n
 */
template<typename Face, int N>
template<int M>
VCL_ENABLE_IF(M < 0, void)
AdjacentFaces<Face, N>::resizeAdjFaces(uint n)
{
	Base::resize(n);
}

template<typename Face, int N>
template<int M>
VCL_ENABLE_IF(M < 0, void)
AdjacentFaces<Face, N>::pushAdjFace(Face* f)
{
	Base::pushBack(f);
}

template<typename Face, int N>
template<int M>
VCL_ENABLE_IF(M < 0, void)
AdjacentFaces<Face, N>::insertAdjFace(uint i, Face* f)
{
	Base::insert(i, f);
}

template<typename Face, int N>
template<int M>
VCL_ENABLE_IF(M < 0, void)
AdjacentFaces<Face, N>::eraseAdjFace(uint i)
{
	Base::erase(i);
}

template<typename Face, int N>
template<int M>
VCL_ENABLE_IF(M < 0, void)
AdjacentFaces<Face, N>::clearAdjFaces()
{
	Base::clear();
}

template<typename Face, int N>
typename AdjacentFaces<Face, N>::AdjacentFaceIterator AdjacentFaces<Face, N>::adjFaceBegin()
{
	return Base::begin();
}

template<typename Face, int N>
typename AdjacentFaces<Face, N>::AdjacentFaceIterator AdjacentFaces<Face, N>::adjFaceEnd()
{
	return Base::end();
}

template<typename Face, int N>
typename AdjacentFaces<Face, N>::ConstAdjacentFaceIterator
AdjacentFaces<Face, N>::adjFaceBegin() const
{
	return Base::begin();
}

template<typename Face, int N>
typename AdjacentFaces<Face, N>::ConstAdjacentFaceIterator
AdjacentFaces<Face, N>::adjFaceEnd() const
{
	return Base::end();
}

template<typename Face, int N>
typename AdjacentFaces<Face, N>::AdjacentFaceRangeIterator AdjacentFaces<Face, N>::adjFaces()
{
	return Base::rangeIterator();
}

template<typename Face, int N>
typename AdjacentFaces<Face, N>::ConstAdjacentFaceRangeIterator
AdjacentFaces<Face, N>::adjFaces() const
{
	return Base::rangeIterator();
}

template<typename Face, int N>
void AdjacentFaces<Face, N>::updateFaceReferences(const Face* oldBase, const Face* newBase)
{
	Base::updateElementReferences(oldBase, newBase);
}

template<typename Face, int N>
void AdjacentFaces<Face, N>::updateFaceReferencesAfterCompact(
	const Face*             base,
	const std::vector<int>& newIndices)
{
	Base::updateElementReferencesAfterCompact(base, newIndices);
}

} // namespace vcl::comp