/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#include "vclib/algorithms/polygon/geometry.h"
#include <iostream>

#include <vclib/meshes.h>
#include <vclib/load_save.h>

int main()
{
	vcl::FileMeshInfo loadedInfo;
	vcl::TriMesh m = vcl::io::loadPly<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/brain.ply", loadedInfo);

	assert(loadedInfo.hasVertices());
	assert(m.vertexNumber() == 18844);
	assert(loadedInfo.hasFaces());
	assert(loadedInfo.hasFaceVRefs());
	assert(m.faceNumber() == 36752);

	m = vcl::io::loadPly<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/bunny_textured.ply", loadedInfo);

	assert(loadedInfo.hasVertices());
	assert(m.vertexNumber() == 5051);
	assert(loadedInfo.hasFaces());
	assert(loadedInfo.hasFaceVRefs());
	assert(m.faceNumber() == 9999);
	assert(loadedInfo.hasTextures());
	assert(m.textureNumber() == 1);
	assert(loadedInfo.hasFaceWedgeTexCoords());
	assert(m.isPerFaceWedgeTexCoordsEnabled());

	m =  vcl::io::loadPly<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/TextureDouble.ply", loadedInfo);

	assert(loadedInfo.hasVertices());
	assert(m.vertexNumber() == 8);
	assert(loadedInfo.hasFaces());
	assert(loadedInfo.hasFaceVRefs());
	assert(m.faceNumber() == 4);
	assert(loadedInfo.hasTextures());
	assert(m.textureNumber() == 2);

	// try to load a polygonal mesh into a trimesh
	m = vcl::io::loadPly<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/cube_poly.ply", loadedInfo);
	for (const vcl::TriMesh::Face& f : m.faces()){
		assert(!f.isEdgeFaux(0));
		assert(!f.isEdgeFaux(1));
		assert(f.isEdgeFaux(2));
	}

	m.addPerFaceCustomComponent<double>("area");
	auto areaVec = m.perFaceCustomComponentVectorHandle<double>("area");

	for(const auto& f : m.faces()) {
		areaVec[m.index(f)] = vcl::faceArea(f);
		std::cerr << "area " << m.index(f) << ": " << areaVec[m.index(f)] << "\n";
	}

	// save again the mesh
	vcl::io::savePly(m, VCL_TEST_RESULTS_PATH "/triangulated_cube.ply");

	return 0;
}