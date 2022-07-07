/*****************************************************************************
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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_WEDGE_COLORS_H
#define VCL_MESH_COMPONENTS_CONCEPTS_WEDGE_COLORS_H

#include <vclib/misc/types.h>

namespace vcl::comp {

/**
 * @brief HasWedgeColors concept
 *
 * This concept is satisfied only if a class has a member function 'wedgeColor(uint)'.
 */
template<typename T>
concept HasWedgeColors = requires(T v) // requires that an object of type T has the following members
{
	v.wedgeColor(uint());
};

/**
 * @brief HasOptionalWedgeColors concept
 *
 * This concept is satisfied only if a class has two member functions:
 * - 'wedgeColor(uint)'
 * - 'isWedgeColorsEnabled()' which returns a bool
 */
template<typename T>
concept HasOptionalWedgeColors = HasWedgeColors<T> && requires(T v)
{
	{ v.isWedgeColorsEnabled() } -> std::same_as<bool>;
};

/**
 * @brief HasRightNumberOfWedgeColors concept
 *
 * This concept is designed to be used with Face components, where the number of wedge colors must
 * be consistent w.r.t. the number of vertices of the face.
 *
 * This concept is satisfied only if static number of wedge colors is the same of the static
 * number of vertices.
 */
template<typename T>
concept HasRightNumberOfWedgeColors = T::VERTEX_NUMBER == T::WEDGE_COLOR_NUMBER;

/**
 * @brief SanityCheckWedgeColors concept
 *
 * This concept is designed to be used with Face components, where the number of wedge colors must
 * be consistent w.r.t. the number of vertices of the face.
 *
 * It is satisfied if:
 * - the component does *not* have wedge colors;
 * - in case it has wedge colors, they have the same number of vertices of the face.
 */
template<typename T>
concept SanityCheckWedgeColors = !HasWedgeColors<T> || HasRightNumberOfWedgeColors<T>;

/* Detector functions to check if a class has WedgeColors or OptionalWedgeColors */

template<typename T>
bool constexpr hasWedgeColors()
{
	return HasWedgeColors<T>;
}

template<typename T>
bool constexpr hasOptionalWedgeColors()
{
	return HasOptionalWedgeColors<T>;
}

template <typename T>
bool isWedgeColorsEnabledOn(const T& element)
{
	if constexpr(hasOptionalWedgeColors<T>()) {
		return element.isWedgeColorsEnabled();
	}
	else {
		return HasWedgeColors<T>;
	}
}

template<typename T>
bool constexpr sanityCheckWedgeColors()
{
	if constexpr (hasWedgeColors<T>()) {
		return T::VERTEX_NUMBER == T::WEDGE_COLOR_NUMBER;
	}
	else {
		return true;
	}
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_WEDGE_COLORS_H
