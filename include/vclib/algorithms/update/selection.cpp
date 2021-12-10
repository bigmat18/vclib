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

#include "selection.h"

#include "../clean.h"

namespace vcl {

template<typename MeshType>
void clearVertexSelection(MeshType& m)
{
	vcl::requireVertices<MeshType>();

	using VertexType = typename MeshType::VertexType;

	for (VertexType& v : m.vertices()) {
		v.unsetSelected();
	}
}

template<typename MeshType>
void clearFaceSelection(MeshType& m)
{
	vcl::requireFaces<MeshType>();

	using FaceType = typename MeshType::FaceType;

	for (FaceType& f : m.faces()) {
		f.unsetSelected();
	}
}

template<typename MeshType>
void selectNonManifoldVertices(MeshType& m, bool clearSelectionFirst)
{
	std::vector<bool> nonManifoldVertices = internal::nonManifoldVerticesVectorBool(m);

	using VertexType = typename MeshType::VertexType;

	for (VertexType& v : m.vertices()) {
		if (nonManifoldVertices[m.index(v)]) {
			v.setSelected();
		}
		else if (clearSelectionFirst) {
			v.unsetSelected();
		}
	}
}

} // namespace vcl
