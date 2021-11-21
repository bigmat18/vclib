/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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

#ifndef VCL_MESH_VERTEX_REQUIREMENTS_H
#define VCL_MESH_VERTEX_REQUIREMENTS_H

#include "element_requirements.h"

namespace vcl {

/********************
 * is/has functions *
 ********************/

// Vertex Adjacent Faces

template<typename MeshType>
bool constexpr hasPerVertexAdjacentFaces();

template<typename MeshType>
bool isPerVertexAdjacentFacesEnabled(const MeshType&);

template<typename MeshType>
bool enableIfPerVertexAdjacentFacesOptional(MeshType&);

// Vertex Adjacent Vertices

template<typename MeshType>
bool constexpr hasPerVertexAdjacentVertices();

template<typename MeshType>
bool isPerVertexAdjacentVerticesEnabled(const MeshType&);

template<typename MeshType>
bool enableIfPerVertexAdjacentVerticesOptional(MeshType&);

// Vertex Color

template<typename MeshType>
bool constexpr hasPerVertexColor();

template<typename MeshType>
bool isPerVertexColorEnabled(const MeshType&);

template<typename MeshType>
bool enableIfPerVertexColorOptional(MeshType&);

// Vertex Mark

template<typename MeshType>
bool constexpr hasPerVertexMark();

template<typename MeshType>
bool isPerVertexMarkEnabled(const MeshType&);

template<typename MeshType>
bool enableIfPerVertexMarkOptional(MeshType&);

// Vertex Mutable Flags

template<typename MeshType>
bool constexpr hasPerVertexMutableBitFlags();

template<typename MeshType>
bool isPerVertexMutableBitFlagsEnabled(const MeshType&);

template<typename MeshType>
bool enableIfPerVertexMutableBitFlagsOptional(MeshType&);

// Vertex Normal

template<typename MeshType>
bool constexpr hasPerVertexNormal();

template<typename MeshType>
bool isPerVertexNormalEnabled(const MeshType&);

template<typename MeshType>
bool enableIfPerVertexNormalOptional(MeshType&);

// Vertex Scalar

template<typename MeshType>
bool constexpr hasPerVertexScalar();

template<typename MeshType>
bool isPerVertexScalarEnabled(const MeshType&);

template<typename MeshType>
bool enableIfPerVertexScalarOptional(MeshType&);

// Vertex TexCoord

template<typename MeshType>
bool constexpr hasPerVertexTexCoord();

template<typename MeshType>
bool isPerVertexTexCoordEnabled(const MeshType&);

template<typename MeshType>
bool enableIfPerVertexTexCoordOptional(MeshType&);

// Vertex Custom Components

template<typename MeshType>
bool constexpr hasPerVertexCustomComponents();

/*********************
 * require functions *
 *********************/

// Vertex Adjacent Faces

template<typename MeshType>
void requirePerVertexAdjacentFaces(const MeshType& m);

// Vertex Adjacent Vertices

template<typename MeshType>
void requirePerVertexAdjacentVertices(const MeshType& m);

// Vertex Color

template<typename MeshType>
void requirePerVertexColor(const MeshType& m);

// Vertex Mark

template<typename MeshType>
void requirePerVertexMark(const MeshType& m);

// Vertex Mutable Flags

template<typename MeshType>
void requirePerVertexMutableBitFlags(const MeshType& m);

// Vertex Normal

template<typename MeshType>
void requirePerVertexNormal(const MeshType& m);

// Vertex Scalar

template<typename MeshType>
void requirePerVertexScalar(const MeshType& m);

// Vertex TexCoord

template<typename MeshType>
void requirePerVertexTexCoord(const MeshType& m);

// Vertex Custom Components

template<typename MeshType>
bool constexpr requirePerVertexCustomComponents();

template<typename MeshType>
bool requirePerVertexCustomComponents(const MeshType& m);

} // namespace vcl

#include "vertex_requirements.cpp"

#endif // VCL_MESH_VERTEX_REQUIREMENTS_H