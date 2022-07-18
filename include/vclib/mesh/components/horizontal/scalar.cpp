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

#include "scalar.h"

namespace vcl::comp {

template<typename T>
const T& Scalar<T>::scalar() const
{
	return s;
}

template<typename T>
T& Scalar<T>::scalar()
{
	return s;
}

template<typename T>
template<typename Element>
void Scalar<T>::importFrom(const Element& e)
{
	if constexpr (HasScalar<Element>) {
		if (isScalarEnabledOn(e)){
			s = e.scalar();
		}
	}
}

} // namespace vcl::comp
