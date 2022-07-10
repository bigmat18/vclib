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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_BIT_FLAGS_H
#define VCL_MESH_COMPONENTS_CONCEPTS_BIT_FLAGS_H

#include <vclib/misc/types.h>

namespace vcl::comp {

class PolygonBitFlags;
class TriangleBitFlags;

/**
 * @brief HasBitFlags concept
 *
 * This concept is satisfied only if a class has a member function 'isDeleted()' which returns
 * a bool
 */
template<typename T>
concept HasBitFlags = requires(T o)
{
	{ o.isDeleted() } -> std::same_as<bool>;
};

/**
 * @brief HasTriangleBitFlags concept
 *
 * This concept is satisfied only if a class has (inherits) PolygonBitFlags
 */
template<typename T>
concept HasPolygonBitFlags = std::derived_from<T, PolygonBitFlags>;

/**
 * @brief HasPolygonBitFlags concept
 *
 * This concept is satisfied only if a class has (inherits) TriangleBitFlags
 */
template<typename T>
concept HasTriangleBitFlags = std::derived_from<T, TriangleBitFlags>;

/**
 * @brief HasFaceBitFlags concept
 *
 * This concept is satisfied only if a class has PolygonBitFlags or TriangleBitFlags
 */
template<typename T>
concept HasFaceBitFlags = HasPolygonBitFlags<T> || HasTriangleBitFlags<T>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_BIT_FLAGS_H
