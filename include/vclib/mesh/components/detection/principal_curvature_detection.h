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

#ifndef VCL_MESH_COMPONENTS_PRINCIPAL_CURVATURE_DETECTION_H
#define VCL_MESH_COMPONENTS_PRINCIPAL_CURVATURE_DETECTION_H

#include <vclib/misc/types.h>

namespace vcl::comp {

/* Triggerers */

class PrincipalCurvatureTriggerer
{
};

class OptionalPrincipalCurvatureTriggerer
{
};

/* Detector to check if a class has (inherits) PrincipalCurvature or OptionalPrincipalCurvature*/

template<typename T>
using hasPrincipalCurvatureT = std::is_base_of<PrincipalCurvatureTriggerer, T>;

template<typename T>
using hasOptionalPrincipalCurvatureT = std::is_base_of<OptionalPrincipalCurvatureTriggerer, T>;

template<typename T>
bool constexpr hasPrincipalCurvature()
{
	return hasPrincipalCurvatureT<T>::value || hasOptionalPrincipalCurvatureT<T>::value;
}

template<typename T>
bool constexpr hasOptionalPrincipalCurvature()
{
	return hasOptionalPrincipalCurvatureT<T>::value;
}

template <typename T>
bool isPrincipalCurvatureEnabledOn(const T& element)
{
	if constexpr (hasOptionalPrincipalCurvature<T>()) {
		return element.isPrincipalCurvatureEnabled();
	}
	else {
		return hasPrincipalCurvature<T>();
	}
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_PRINCIPAL_CURVATURE_DETECTION_H
