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

#include "wedge_colors.h"

namespace vcl::comp {

template<int N, typename El, bool h, bool o>
vcl::Color& WedgeColors<N, El, h, o>::wedgeColor(uint i)
{
	return colors().at(i);
}

template<int N, typename El, bool h, bool o>
const vcl::Color& WedgeColors<N, El, h, o>::wedgeColor(uint i) const
{
	return colors().at(i);
}

template<int N, typename El, bool h, bool o>
vcl::Color& WedgeColors<N, El, h, o>::wedgeColorMod(int i)
{
	return colors().atMod(i);
}

template<int N, typename El, bool h, bool o>
const vcl::Color& WedgeColors<N, El, h, o>::wedgeColorMod(int i) const
{
	return colors().atMod(i);
}

template<int N, typename El, bool h, bool o>
void WedgeColors<N, El, h, o>::setWedgeColor(const vcl::Color& t, uint i)
{
	colors().set(t, i);
}

template<int N, typename El, bool h, bool o>
void WedgeColors<N, El, h, o>::setWedgeColors(const std::vector<vcl::Color>& list)
{
	colors().set(list);
}

template<int N, typename El, bool h, bool o>
typename WedgeColors<N, El, h, o>::WedgeColorsIterator WedgeColors<N, El, h, o>::wedgeColorBegin()
{
	return colors().begin();
}

template<int N, typename El, bool h, bool o>
typename WedgeColors<N, El, h, o>::WedgeColorsIterator WedgeColors<N, El, h, o>::wedgeColorEnd()
{
	return colors().end();
}

template<int N, typename El, bool h, bool o>
typename WedgeColors<N, El, h, o>::ConstWedgeColorsIterator WedgeColors<N, El, h, o>::wedgeColorBegin() const
{
	return colors().begin();
}

template<int N, typename El, bool h, bool o>
typename WedgeColors<N, El, h, o>::ConstWedgeColorsIterator WedgeColors<N, El, h, o>::wedgeColorEnd() const
{
	return colors().end();
}

template<int N, typename El, bool h, bool o>
typename WedgeColors<N, El, h, o>::WedgeColorsRangeIterator WedgeColors<N, El, h, o>::wedgeColors()
{
	return colors().rangeIterator();
}

template<int N, typename El, bool h, bool o>
typename WedgeColors<N, El, h, o>::ConstWedgeColorsRangeIterator WedgeColors<N, El, h, o>::wedgeColors() const
{
	return colors().rangeIterator();
}

template<int N, typename El, bool h, bool o>
RandomAccessContainer<vcl::Color, N>& WedgeColors<N, El, h, o>::colors()
{
	return data.template get<El>(this);
}

template<int N, typename El, bool h, bool o>
const RandomAccessContainer<vcl::Color, N>& WedgeColors<N, El, h, o>::colors() const
{
	return data.template get<El>(this);
}

template<int N, typename El, bool h, bool o>
void WedgeColors<N, El, h, o>::resizeWedgeColors(uint n) requires (N < 0)
{
	colors().resize(n);
}

template<int N, typename El, bool h, bool o>
void WedgeColors<N, El, h, o>::pushWedgeColor(const vcl::Color& c) requires (N < 0)
{
	colors().pushBack(c);
}

template<int N, typename El, bool h, bool o>
void WedgeColors<N, El, h, o>::insertWedgeColor(uint i, const vcl::Color& c) requires (N < 0)
{
	colors().insert(i, c);
}

template<int N, typename El, bool h, bool o>
void WedgeColors<N, El, h, o>::eraseWedgeColor(uint i) requires (N < 0)
{
	colors().erase(i);
}

template<int N, typename El, bool h, bool o>
void WedgeColors<N, El, h, o>::clearWedgeColor() requires (N < 0)
{
	colors().clear();
}

template<int N, typename El, bool h, bool o>
template<typename Element>
void WedgeColors<N, El, h, o>::importFrom(const Element& e)
{
	if constexpr (HasWedgeColors<Element>) {
		if (isWedgeColorsEnabledOn(e)) {
			if constexpr(N > 0) {
				// same static size
				if constexpr (N == Element::WEDGE_COLOR_NUMBER) {
					importWedgeColorsFrom(e);
				}
				// from dynamic to static, but dynamic size == static size
				else if constexpr (Element::WEDGE_COLOR_NUMBER < 0){
					if (e.vertexNumber() == N){
						importWedgeColorsFrom(e);
					}
				}
				else {
					// do not import in this case: cannot import from dynamic size != static size
				}
			}
			else {
				// from static/dynamic to dynamic size: need to resize first, then import
				resizeWedgeColors(e.vertexNumber());
				importWedgeColorsFrom(e);
			}
		}
	}
}

template<int N, typename El, bool h, bool o>
template<typename Element>
void WedgeColors<N, El, h, o>::importWedgeColorsFrom(const Element& e)
{
	for (uint i = 0; i < e.vertexNumber(); ++i){
		wedgeColor(i) = e.wedgeColor(i);
	}
}

} // namespace vcl::comp
