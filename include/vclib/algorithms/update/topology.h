/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#ifndef VCL_ALGORITHMS_TOPOLOGY_H
#define VCL_ALGORITHMS_TOPOLOGY_H

#include <vclib/mesh/requirements.h>

namespace vcl {

template<MeshConcept MeshType>
void clearPerVertexAdjacentFaces(MeshType& m);

template<FaceMeshConcept MeshType>
void updatePerVertexAdjacentFaces(MeshType& m);

template<MeshConcept MeshType>
void clearPerVertexAdjacentVertices(MeshType& m);

template<FaceMeshConcept MeshType>
void updatePerVertexAdjacentVertices(MeshType& m);

template<FaceMeshConcept MeshType>
void clearPerFaceAdjacentFaces(MeshType& m);

template<FaceMeshConcept MeshType>
void updatePerFaceAdjacentFaces(MeshType& m);

} // namespace vcl

#include "topology.cpp"

#endif // VCL_ALGORITHMS_TOPOLOGY_H
