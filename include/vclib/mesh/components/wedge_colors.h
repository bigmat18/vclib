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

#ifndef VCL_MESH_COMPONENTS_WEDGE_COLORS_H
#define VCL_MESH_COMPONENTS_WEDGE_COLORS_H

#include <vclib/concepts/mesh/components/wedge_colors.h>
#include <vclib/views/view.h>
#include <vclib/space/vector.h>

#include "bases/container_component.h"

namespace vcl::comp {

/**
 * @brief The WedgeColors class
 *
 * @ingroup components
 */
template<int N, typename ElementType = void, bool optional = false>
class WedgeColors : public ContainerComponent<vcl::Color, N, void, ElementType, optional, true>
{
	using Base = ContainerComponent<vcl::Color, N, void, ElementType, optional, true>;
	using ThisType = WedgeColors<N, ElementType, optional>;

public:
	using WedgeTexCoordsComponent = ThisType; // expose the type to allow access to this component
	
	static const int WEDGE_COLOR_NUMBER = Base::SIZE;

	/* Iterator Types declaration */

	using WedgeColorsIterator      = typename Base::Iterator;
	using ConstWedgeColorsIterator = typename Base::ConstIterator;

	bool isEnabled() const;
	bool isWedgeColorsEnabled() const;

	/* Member functions */

	vcl::Color&       wedgeColor(uint i);
	const vcl::Color& wedgeColor(uint i) const;

	vcl::Color&       wedgeColorMod(int i);
	const vcl::Color& wedgeColorMod(int i) const;

	void setWedgeColor(const vcl::Color& t, uint i);
	void setWedgeColors(const std::vector<vcl::Color>& list);

	/* Iterator Member functions */

	WedgeColorsIterator      wedgeColorBegin();
	WedgeColorsIterator      wedgeColorEnd();
	ConstWedgeColorsIterator wedgeColorBegin() const;
	ConstWedgeColorsIterator wedgeColorEnd() const;
	auto                     wedgeColors();
	auto                     wedgeColors() const;

protected:
	/* Member functions specific for vector of wedge colors */

	void resizeWedgeColors(uint n) requires (N < 0);
	void pushWedgeColor(const vcl::Color& c) requires (N < 0);
	void insertWedgeColor(uint i, const vcl::Color& c) requires (N < 0);
	void eraseWedgeColor(uint i) requires (N < 0);
	void clearWedgeColor() requires (N < 0);

	template <typename Element>
	void importFrom(const Element& e);

private:
	template<typename Element>
	void importWedgeColorsFrom(const Element& e);

	Vector<vcl::Color, N>& colors();
	const Vector<vcl::Color, N>& colors() const;
};

/* Detector function to check if a class has WedgeColors enabled */

template <typename T>
bool isWedgeColorsEnabledOn(const T& element);

} // namespace vcl::comp

#include "wedge_colors.cpp"

#endif // VCL_MESH_COMPONENTS_WEDGE_COLORS_H
