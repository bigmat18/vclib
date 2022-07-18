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

#ifndef VCL_MESH_ALGORITHMS_H
#define VCL_MESH_ALGORITHMS_H

#include <vclib/space/point.h>

namespace vcl::mesh {

template<typename Scalar>
std::vector<uint> earCut(const std::vector<Point2<Scalar>>& polygon);

template<typename Scalar>
std::vector<uint> earCut(const std::vector<Point3<Scalar>>& polygon);

template<typename Polygon>
std::vector<uint> earCut(const Polygon& polygon);

} // namespace vcl::mesh

#include "mesh_algorithms.cpp"

#endif // VCL_MESH_ALGORITHMS_H
