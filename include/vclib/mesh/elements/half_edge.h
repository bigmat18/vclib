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

#ifndef VCL_MESH_ELEMENTS_HALF_EDGE_H
#define VCL_MESH_ELEMENTS_HALF_EDGE_H

#include "half_edge_concept.h"

namespace vcl::mesh {

// EdgeContainer class declaration
template<HalfEdgeConcept>
class HalfEdgeContainer;

template<typename>
class ElementContainer;

} // namespace vcl::mesh

namespace vcl {

template<typename MeshType, typename... Args>
class HalfEdge : public Args...
{
	template<HalfEdgeConcept>
	friend class mesh::HalfEdgeContainer;

	template<typename>
	friend class mesh::ElementContainer;

public:
	using ParentMeshType = MeshType;

	HalfEdge();

	template<typename Element>
	void importFrom(const Element& e);

	constexpr MeshType* parentMesh();
	constexpr const MeshType* parentMesh() const;

private:
	MeshType* parent = nullptr;

	void setParentMesh(void* parentMesh);
};

} // namespace vcl

#include "half_edge.cpp"

#endif // VCL_MESH_ELEMENTS_HALF_EDGE_H
