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

#ifndef VCL_SPACE_SAMPLER_MESH_SAMPLER_H
#define VCL_SPACE_SAMPLER_MESH_SAMPLER_H

#include <vclib/mesh/requirements.h>

namespace vcl {

template<MeshConcept MeshType>
class MeshSampler
{
public:
	using PointType  = typename MeshType::VertexType::CoordType;
	using ScalarType = typename PointType::ScalarType;

	MeshSampler();

	const MeshType& samples() const;

	void clear();
	void reserve(uint n);

	void addPoint(const PointType& p);

	template<VertexConcept VertexType>
	void addVertex(const VertexType& v);

	template<EdgeConcept EdgeType>
	void addEdge(const EdgeType& e, double u, bool copyScalar = true);

	template<FaceConcept FaceType>
	void addFace(const FaceType& f, bool copyNormal = false, bool copyScalar = true);

	template<FaceConcept FaceType>
	void addFace(
		const FaceType&                f,
		const std::vector<ScalarType>& p,
		bool                           copyNormal = false,
		bool                           copyScalar = true);

	template<FaceConcept FaceType>
	void addFace(
		const FaceType&  f,
		const PointType& weights,
		bool             copyNormal = false,
		bool             copyScalar = true);

private:

	template<FaceConcept FaceType>
	void copyComponents(uint vi, const FaceType&  f, bool copyNormal, bool copyScalar);

	MeshType m;
};

} // namespace vcl

#include "mesh_sampler.cpp"

#endif // VCL_SPACE_SAMPLER_MESH_SAMPLER_H
