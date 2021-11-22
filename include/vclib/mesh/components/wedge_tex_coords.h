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

#ifndef VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_H
#define VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_H

#include "generic_container.h"

#include <vclib/space/tex_coord.h>

namespace vcl::comp {

class WedgeTexCoordsTriggerer
{
};

template<typename Scalar, int N>
class WedgeTexCoords :
		protected GenericContainer<vcl::TexCoord<Scalar>, N>,
		public WedgeTexCoordsTriggerer
{
	using Base = GenericContainer<vcl::TexCoord<Scalar>, N>;

public:
	static const int WEDGE_TEX_COORD_NUMBER = Base::CONTAINER_SIZE;

	/** Iterator Types declaration **/

	using WedgeTexCoordsIterator           = typename Base::GCIterator;
	using ConstWedgeTexCoordsIterator      = typename Base::ConstGCIterator;
	using WedgeTexCoordsRangeIterator      = typename Base::GCRangeIterator;
	using ConstWedgeTexCoordsRangeIterator = typename Base::ConstGCRangeIterator;

	WedgeTexCoords();

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
};

template<typename T>
using hasWedgeTexCoordsT = std::is_base_of<WedgeTexCoordsTriggerer, T>;

template<typename T>
bool constexpr hasWedgeTexCoords()
{
	return hasWedgeTexCoordsT<T>::value;
}

} // namespace vcl::comp

#include "wedge_tex_coords.cpp"

#endif // VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_H
