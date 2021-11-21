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

#include "flag.h"

#include <vclib/algorithms/internal/edge_sorterer.h>
#include <vclib/mesh/requirements.h>

namespace vcl {

/**
 * @brief Computes per-face border flags without requiring any kind of
 * topology info.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *   - Faces
 *
 * Complexity: O(NF log (NF))
 *
 * @param m: the mesh on which the border flags will be updated
 */
template<typename MeshType>
void updateBorder(MeshType& m)
{
	vcl::requireVertices(m);
	vcl::requireFaces(m);

	using VertexType    = typename MeshType::Vertex;
	using FaceType      = typename MeshType::Face;

	for (FaceType& f : m.faces())
		f.clearAllEdgeOnBorder();

	if (m.faceNumber() == 0)
		return;

	std::vector<internal::EdgeSorterer<MeshType>> e = internal::fillAndSortEdgeVector(m);

	typename std::vector<internal::EdgeSorterer<MeshType>>::iterator pe, ps;
	ps = e.begin();
	pe = e.begin();
	do {
		if (pe == e.end() || *pe != *ps) { // Trovo blocco di edge uguali
			if (pe - ps == 1) {
				ps->f->setEdgeOnBorder(ps->z);
			}
			ps = pe;
		}
		if (pe != e.end())
			++pe;
	} while (pe != e.end());
}

} // namespace vcl