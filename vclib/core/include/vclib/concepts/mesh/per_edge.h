/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_CONCEPTS_MESH_PER_EDGE_H
#define VCL_CONCEPTS_MESH_PER_EDGE_H

#include "containers/edge_container.h"
#include "elements/edge.h"

/**
 * @defgroup edge_mesh_concepts EdgeMesh Concepts
 * @ingroup mesh_concepts
 *
 * @brief List of concepts for types related to Meshes having Edges.
 * They allow to check the presence of specific components for Edges.
 */

namespace vcl {

/**
 * @brief Concept that checks if a Mesh has the per Edge AdjacentEdges
 * component.
 *
 * Evaluates to true if AdjacentEdges is part of the Edge element, whether it is
 * horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup edge_mesh_concepts
 */
template<typename MeshType>
concept HasPerEdgeAdjacentEdges =
    HasEdges<MeshType> &&
    edge::HasAdjacentEdges<typename RemoveRef<MeshType>::EdgeType>;

/**
 * @brief Concept that checks if a Mesh has the per Edge AdjacentFaces
 * component.
 *
 * Evaluates to true if AdjacentFaces is part of the Edge element, whether it is
 * horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup edge_mesh_concepts
 */
template<typename MeshType>
concept HasPerEdgeAdjacentFaces =
    HasEdges<MeshType> &&
    edge::HasAdjacentFaces<typename RemoveRef<MeshType>::EdgeType>;

/**
 * @brief Concept that checks if a Mesh has the per Edge BitFlags component.
 *
 * Evaluates to true if BitFlags is part of the Edge element, whether it is
 * horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup edge_mesh_concepts
 */
template<typename MeshType>
concept HasPerEdgeBitFlags =
    HasEdges<MeshType> &&
    edge::HasBitFlags<typename RemoveRef<MeshType>::EdgeType>;

/**
 * @brief Concept that checks if a Mesh has the per Edge Color component.
 *
 * Evaluates to true if Color is part of the Edge element, whether it is
 * horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup edge_mesh_concepts
 */
template<typename MeshType>
concept HasPerEdgeColor =
    HasEdges<MeshType> &&
    edge::HasColor<typename RemoveRef<MeshType>::EdgeType>;

/**
 * @brief Concept that checks if a Mesh has the per Edge CustomComponents
 * component.
 *
 * Evaluates to true if CustomComponents is part of the Edge element, whether it
 * is horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup edge_mesh_concepts
 */
template<typename MeshType>
concept HasPerEdgeCustomComponents =
    HasEdges<MeshType> &&
    edge::HasCustomComponents<typename RemoveRef<MeshType>::EdgeType>;

/**
 * @brief Concept that checks if a Mesh has the per Edge Mark component.
 *
 * Evaluates to true if Mark is part of the Edge element, whether it is
 * horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup edge_mesh_concepts
 */
template<typename MeshType>
concept HasPerEdgeMark =
    HasEdges<MeshType> && edge::HasMark<typename RemoveRef<MeshType>::EdgeType>;

/**
 * @brief Concept that checks if a Mesh has the per Edge Normal component.
 *
 * Evaluates to true if Normal is part of the Edge element, whether it is
 * horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup edge_mesh_concepts
 */
template<typename MeshType>
concept HasPerEdgeNormal =
    HasEdges<MeshType> &&
    edge::HasNormal<typename RemoveRef<MeshType>::EdgeType>;

/**
 * @brief Concept that checks if a Mesh has the per Edge Quality component.
 *
 * Evaluates to true if Quality is part of the Edge element, whether it is
 * horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup edge_mesh_concepts
 */
template<typename MeshType>
concept HasPerEdgeQuality =
    HasEdges<MeshType> &&
    edge::HasQuality<typename RemoveRef<MeshType>::EdgeType>;

/**
 * @brief Concept that checks if a Mesh has the per Edge VertexReferences
 * component (Pointers or Indices).
 *
 * Evaluates to true if VertexPointers is part of the Edge element, whether it
 * is horizontal, vertical or optional.
 *
 * @tparam MeshType: mesh type to check.
 *
 * @ingroup edge_mesh_concepts
 */
template<typename MeshType>
concept HasPerEdgeVertexReferences =
    HasEdges<MeshType> &&
    edge::HasVertexReferences<typename RemoveRef<MeshType>::EdgeType>;

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_PER_EDGE_H
