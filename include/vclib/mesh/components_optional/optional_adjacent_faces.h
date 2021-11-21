/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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

#ifndef VCL_MESH_COMPONENTS_OPTIONAL_ADJACENT_FACES_H
#define VCL_MESH_COMPONENTS_OPTIONAL_ADJACENT_FACES_H

#include <array>
#include <assert.h>
#include <type_traits>
#include <vector>

#include "optional_info.h"

#include "../iterators/range_iterator.h"

namespace vcl::mesh {
template<typename, typename>
class OptionalAdjacentFacesVector;
}

namespace vcl::comp {

namespace internal {

template<int M, typename T>
using ReturnIfIsVector = typename std::enable_if<(M < 0), T>::type;
template<int M, typename T>
using ReturnIfIsArray = typename std::enable_if<(M >= 0), T>::type;

} // namespace internal

class OptionalAdjacentFacesTriggerer
{
};

template<typename Face, int N, typename T>
class OptionalAdjacentFaces : public OptionalAdjacentFacesTriggerer, public virtual OptionalInfo<T>
{
	template<typename, typename>
	friend class OptionalAdjacentFacesVector;

private:
	using B = OptionalInfo<T>;

	// if we use the vector, the size of the array will be 0
	// actually the array will never be used and will not use memory, it's just for declaration
	static const int ARRAY_SIZE = N >= 0 ? N : 0;

public:
	// the AdjFacesContainer type will be array or vector, depending on N value
	using AdjFacesContainer = typename std::conditional<
		(N >= 0),
		typename std::array<Face*, ARRAY_SIZE>,
		typename std::vector<Face*>>::type;

	static const int ADJ_FACE_NUMBER = N;

	/** Iterator Types declaration **/

	// if using array, will be the array iterator, the vector iterator otherwise
	using AdjacentFaceIterator = typename std::conditional<
		(N >= 0),
		typename std::array<Face*, ARRAY_SIZE>::iterator,
		typename std::vector<Face*>::iterator>::type;

	using ConstAdjacentFaceIterator = typename std::conditional<
		(N >= 0),
		typename std::array<Face*, ARRAY_SIZE>::const_iterator,
		typename std::vector<Face*>::const_iterator>::type;

	using AdjacentFaceRangeIterator = RangeIterator<OptionalAdjacentFaces, AdjacentFaceIterator>;
	using ConstAdjacentFaceRangeIterator =
		ConstRangeIterator<OptionalAdjacentFaces, ConstAdjacentFaceIterator>;

	/** Constructor **/

	OptionalAdjacentFaces();

	/** Member functions **/

	unsigned int adjFacesNumber() const;

	Face*&      adjFace(unsigned int i);
	const Face* adjFace(unsigned int i) const;
	Face*&      adjFaceMod(int i);
	const Face* adjFaceMod(int i) const;

	void setAdjFace(Face* f, unsigned int i);
	void setAdjFaces(const std::vector<Face*>& list);

	bool containsAdjFace(const Face* f) const;

	AdjacentFaceIterator findAdjFace(const Face* f);
	ConstAdjacentFaceIterator findAdjFace(const Face* f) const;

	int indexOfAdjFace(const Face* f) const;

	/** Member functions specific for vector **/

	template<int U = N>
	internal::ReturnIfIsVector<U, void> resizeAdjFaces(unsigned int n);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> pushAdjFace(Face* f);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> insertAdjFace(unsigned int i, Face* f);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> eraseAdjFace(unsigned int i);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> clearAdjFaces();

	/** Iterator Member functions **/

	AdjacentFaceIterator           adjFaceBegin();
	AdjacentFaceIterator           adjFaceEnd();
	ConstAdjacentFaceIterator      adjFaceBegin() const;
	ConstAdjacentFaceIterator      adjFaceEnd() const;
	AdjacentFaceRangeIterator      adjFaces();
	ConstAdjacentFaceRangeIterator adjFaces() const;

protected:
	bool adjFacesEnabled() const;

	void updateFaceReferences(const Face* oldBase, const Face* newBase);

	void updateFaceReferencesAfterCompact(const Face* base, const std::vector<int>& newIndices);

private:
	unsigned int thisId() const { return B::index((T*)this); }
};

/**
 * Detector to check if a class has (inherits) OptionalAdjacentFaces
 */

template<typename T>
using hasOptionalAdjacentFacesT = std::is_base_of<OptionalAdjacentFacesTriggerer, T>;

template<typename U, typename T>
using ReturnIfHasOptionalAdjacentFaces =
	typename std::enable_if<hasOptionalAdjacentFacesT<U>::value, T>::type;

template<typename T>
bool constexpr hasOptionalAdjacentFaces()
{
	return hasOptionalAdjacentFacesT<T>::value;
}

} // namespace vcl::comp

#include "optional_adjacent_faces.cpp"

#endif // VCL_MESH_COMPONENTS_OPTIONAL_ADJACENT_FACES_H