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

#ifndef VCL_ALGORITHMS_STAT_SCALAR_H
#define VCL_ALGORITHMS_STAT_SCALAR_H

#include <vclib/mesh/requirements.h>

namespace vcl {

template<MeshConcept MeshType>
std::pair<typename MeshType::VertexType::ScalarType, typename MeshType::VertexType::ScalarType>
vertexScalarMinMax(const MeshType& m);

template<FaceMeshConcept MeshType>
std::pair<typename MeshType::FaceType::ScalarType, typename MeshType::FaceType::ScalarType>
faceScalarMinMax(const MeshType& m);

template<MeshConcept MeshType>
typename MeshType::VertexType::ScalarType vertexScalarAverage(const MeshType& m);

template<FaceMeshConcept MeshType>
typename MeshType::FaceType::ScalarType faceScalarAverage(const MeshType& m);

template<MeshConcept MeshType>
std::vector<typename MeshType::VertexType::ScalarType> vertexRadiusFromScalar(
	const MeshType& m,
	double          diskRadius,
	double          radiusVariance,
	bool            invert = false);

} // namespace vcl

#include "scalar.cpp"

#endif // VCL_ALGORITHMS_STAT_SCALAR_H