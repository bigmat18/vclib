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

#ifndef VCL_ALGORITHMS_FITTING_H
#define VCL_ALGORITHMS_FITTING_H

#include <vclib/space/plane.h>

namespace vcl {

template<typename Scalar>
Plane<Scalar> fitPlaneToPointCloud(const std::vector<Point3<Scalar>>& pointVec);

template<typename Scalar>
Plane<Scalar> fitPlaneToWeightedPointCloud(const std::vector<Point3<Scalar>>& pointVec);

} // namespace vcl

#include "fitting.cpp"

#endif // VCL_ALGORITHMS_FITTING_H
