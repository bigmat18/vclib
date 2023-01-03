/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#ifndef VCL_MESH_COMPONENTS_OPTIONAL_WEDGE_COLORS_H
#define VCL_MESH_COMPONENTS_OPTIONAL_WEDGE_COLORS_H

#include <array>
#include <vector>

#include <vclib/iterators/range_iterator.h>

#include "../vertical_component.h"

#include <vclib/space/color.h>

#include "../../concepts/wedge_colors.h"

namespace vcl::mesh {
template<typename, typename>
class OptionalWedgeColorsVector;
}

namespace vcl::comp {

template<int N, typename T>
class OptionalWedgeColors : public virtual VerticalComponent<T>
{
	template<typename, typename>
	friend class OptionalWedgeColorsVector;

private:
	using B = VerticalComponent<T>;
	uint thisId() const { return B::index((T*) this); }

	// if we use the vector, the size of the array will be 0
	// actually the array will never be used and will not use memory, it's just for declaration
	static const int ARRAY_SIZE = N >= 0 ? N : 0;

public:
	// possibility to access to the WedgeColors class, whether is optional or not.
	using WedgeColors = OptionalWedgeColors;

	static const int WEDGE_COLOR_NUMBER = N;

	/* Iterator Types declaration */

	// if using array, will be the array iterator, the vector iterator otherwise
	using WedgeColorsIterator = typename std::conditional<
		(N >= 0),
		typename std::array<vcl::Color, ARRAY_SIZE>::iterator,
		typename std::vector<vcl::Color>::iterator>::type;

	using ConstWedgeColorsIterator = typename std::conditional<
		(N >= 0),
		typename std::array<vcl::Color, ARRAY_SIZE>::const_iterator,
		typename std::vector<vcl::Color>::const_iterator>::type;

	using WedgeColorsRangeIterator = RangeIterator<OptionalWedgeColors, WedgeColorsIterator>;
	using ConstWedgeColorsRangeIterator =
		ConstRangeIterator<OptionalWedgeColors, ConstWedgeColorsIterator>;

	/* Constructor */

	OptionalWedgeColors();

	/* Member functions */

	vcl::Color&       wedgeColor(uint i);
	const vcl::Color& wedgeColor(uint i) const;

	vcl::Color&       wedgeColorMod(int i);
	const vcl::Color& wedgeColorMod(int i) const;

	void setWedgeColor(const vcl::Color& t, uint i);
	void setWedgeColors(const std::vector<vcl::Color>& list);

	bool isWedgeColorsEnabled() const;

	/* Iterator Member functions */

	WedgeColorsIterator           wedgeColorBegin();
	WedgeColorsIterator           wedgeColorEnd();
	ConstWedgeColorsIterator      wedgeColorBegin() const;
	ConstWedgeColorsIterator      wedgeColorEnd() const;
	WedgeColorsRangeIterator      wedgeColors();
	ConstWedgeColorsRangeIterator wedgeColors() const;

	// dummy member to discriminate between non-optional and optional component
	void __optionalWedgeColors() const {};

	// dummy member to discriminate between this component and the one simulated by the
	// FaceHalfEdgeReference component
	void __compWedgeColors() const {};

protected:
	/* Member functions specific for vector */

	void resizeWedgeColors(uint n) requires (N < 0);
	void pushWedgeColor(const vcl::Color& t) requires (N < 0);
	void insertWedgeColor(uint i, const vcl::Color& t) requires (N < 0);
	void eraseWedgeColor(uint i) requires (N < 0);
	void clearWedgeColor() requires (N < 0);

	template <typename Element>
	void importFrom(const Element& e);
private:
	template<typename Element>
	void importWedgeColorsFrom(const Element& e);

	uint wedgeColorsNumber() const;
};

} // namespace vcl::comp

#include "optional_wedge_colors.cpp"

#endif // VCL_MESH_COMPONENTS_OPTIONAL_WEDGE_COLORS_H
