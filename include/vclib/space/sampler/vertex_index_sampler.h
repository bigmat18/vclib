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

#ifndef VCL_SPACE_SAMPLER_VERTEX_INDEX_SAMPLER_H
#define VCL_SPACE_SAMPLER_VERTEX_INDEX_SAMPLER_H

#include <vclib/misc/types.h>

#include "sampler_concept.h"

namespace vcl {

class VertexIndexSampler
{
public:
	VertexIndexSampler() = default;

	const std::vector<uint> samples() const;

	void clear();
	void reserve(uint n);

	template<MeshConcept MeshType>
	void addVertex(const typename MeshType::VertexType& v, const MeshType& m);

private:
	std::vector<uint> samplesVec;
};

// makes sure that the VertexIndexSampler satisfies SamplerConcept
static_assert(SamplerConcept<VertexIndexSampler>, "VertexIndexSampler is not a valid Sampler");

} // namespace vcl

#include "vertex_index_sampler.cpp"

#endif // VCL_SPACE_SAMPLER_VERTEX_INDEX_SAMPLER_H