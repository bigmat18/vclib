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

#include <iostream>

#include <vclib/algorithms/update/topology.h>
#include <vclib/io/load_ply.h>
#include <vclib/io/save_ply.h>
#include <vclib/trimesh.h>
#include <vclib/misc/timer.h>

int main()
{
	vcl::TriMesh m;

	vcl::io::loadPly(m, "/home/alessandro/tmp/cube.ply");

	m.enablePerVertexAdjacentFaces();
	vcl::updatePerVertexAdjacentFaces(m);

	for (const vcl::TriMesh::Vertex& v : m.vertices()) {
		std::cerr << "Adj faces to vertex " << m.index(v) << ": \n\t";
		for (const vcl::TriMesh::Face* f : v.adjFaces()){
			std::cerr << m.index(f) << "; ";
		}
		std::cerr << std::endl;
	}

	m.enablePerFaceAdjacentFaces();
	vcl::updatePerFaceAdjacentFaces(m);

	std::cerr << std::endl << std::endl;

	for (const vcl::TriMesh::Face& f : m.faces()) {
		std::cerr << "Adj faces to face " << m.index(f) << ": \n\t";
		for (const vcl::TriMesh::Face* fadj : f.adjFaces()){
			if (fadj != nullptr)
				std::cerr << m.index(fadj) << "; ";
			else
				std::cerr << "-1; ";
		}
		std::cerr << std::endl;
	}

	return 0;
}
