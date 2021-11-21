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

#ifndef VCL_MESH_COMPONENTS_OPTIONAL_WEDGE_TEX_COORDS_H
#define VCL_MESH_COMPONENTS_OPTIONAL_WEDGE_TEX_COORDS_H

#include <array>
#include <assert.h>
#include <type_traits>
#include <vector>

#include "optional_info.h"

#include <vclib/space/tex_coord.h>

#include "../iterators/range_iterator.h"

namespace vcl::mesh {
template<typename, typename>
class OptionalWedgeTexCoordsVector;
}

namespace vcl::comp {

namespace internal {

template<int M, typename T>
using ReturnIfIsVector = typename std::enable_if<(M < 0), T>::type;
template<int M, typename T>
using ReturnIfIsArray = typename std::enable_if<(M >= 0), T>::type;

} // namespace internal

class OptionalWedgeTexCoordsTriggerer
{
};

template<typename Scalar, int N, typename T>
class OptionalWedgeTexCoords
{
	template<typename, typename>
	friend class OptionalWedgeTexCoordsVector;

private:
	using B = OptionalInfo<T>;

	// if we use the vector, the size of the array will be 0
	// actually the array will never be used and will not use memory, it's just for declaration
	static const int ARRAY_SIZE = N >= 0 ? N : 0;

public:
	// the AdjFacesContainer type will be array or vector, depending on N value
	using WedgeTexCoordsContainer = typename std::conditional<
		(N >= 0),
		typename std::array<vcl::TexCoord<Scalar>, ARRAY_SIZE>,
		typename std::vector<vcl::TexCoord<Scalar>>>::type;

	static const int ADJ_FACE_NUMBER = N;

	/** Iterator Types declaration **/

	// if using array, will be the array iterator, the vector iterator otherwise
	using WedgeTexCoordsIterator = typename std::conditional<
		(N >= 0),
		typename std::array<vcl::TexCoord<Scalar>, ARRAY_SIZE>::iterator,
		typename std::vector<vcl::TexCoord<Scalar>>::iterator>::type;

	using ConstWedgeTexCoordsIterator = typename std::conditional<
		(N >= 0),
		typename std::array<vcl::TexCoord<Scalar>, ARRAY_SIZE>::const_iterator,
		typename std::vector<vcl::TexCoord<Scalar>>::const_iterator>::type;

	using WedgeTexCoordsRangeIterator =
		RangeIterator<OptionalWedgeTexCoords, WedgeTexCoordsIterator>;
	using ConstWedgeTexCoordsRangeIterator =
		ConstRangeIterator<OptionalWedgeTexCoords, ConstWedgeTexCoordsIterator>;

	/** Constructor **/

	OptionalWedgeTexCoords();

	/** Member functions **/

	vcl::TexCoord<Scalar>&       wedgeTexCoord(unsigned int i);
	const vcl::TexCoord<Scalar>& wedgeTexCoord(unsigned int i) const;

	vcl::TexCoord<Scalar>&       wedgeTexCoordMod(int i);
	const vcl::TexCoord<Scalar>& wedgeTexCoordMod(int i) const;

	void setWedgeTexCoord(const vcl::TexCoord<Scalar>& t, unsigned int i);
	void setWedgeTexCoords(const std::vector<vcl::TexCoord<Scalar>>& list);

	/** Iterator Member functions **/

	WedgeTexCoordsIterator           wedgeTexCoordBegin();
	WedgeTexCoordsIterator           wedgeTexCoordEnd();
	ConstWedgeTexCoordsIterator      wedgeTexCoordBegin() const;
	ConstWedgeTexCoordsIterator      wedgeTexCoordEnd() const;
	WedgeTexCoordsRangeIterator      wedgeTexCoordIterator();
	ConstWedgeTexCoordsRangeIterator wedgeTexCoordIterator() const;

protected:
	using WedgeTexCoordScalarType = Scalar;

	/** Member functions specific for vector **/

	template<int U = N>
	internal::ReturnIfIsVector<U, void> resizeWedgeTexCoords(unsigned int n);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> pushWedgeTexCoord(const vcl::TexCoord<Scalar>& t);

	template<int U = N>
	internal::ReturnIfIsVector<U, void>
	insertWedgeTexCoord(unsigned int i, const vcl::TexCoord<Scalar>& t);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> eraseWedgeTexCoord(unsigned int i);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> clearWedgeTexCoord();

	/** Utility member functions **/

	bool wedgeTexCoordsEnabled() const;

private:
	unsigned int thisId() const { return B::index((T*)this); }

	unsigned int wedgeTexCoordsNumber() const;
};

/**
 * Detector to check if a class has (inherits) OptionalWedgeTexCoords
 */

template<typename T>
using hasOptionalWedgeTexCoordsT = std::is_base_of<OptionalWedgeTexCoordsTriggerer, T>;

template<typename U, typename T>
using ReturnIfHasOptionalWedgeTexCoords =
	typename std::enable_if<hasOptionalWedgeTexCoordsT<U>::value, T>::type;

template<typename T>
bool constexpr hasOptionalWedgeTexCoords()
{
	return hasOptionalWedgeTexCoordsT<T>::value;
}

} // namespace vcl::comp

#include "optional_wedge_tex_coords.cpp"

#endif // VCL_MESH_COMPONENTS_OPTIONAL_WEDGE_TEX_COORDS_H