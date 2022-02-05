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

#include <iostream>

#include <vclib/tri_mesh.h>
#include <vclib/io/load_off.h>

int main()
{
	vcl::io::FileMeshInfo loadedInfo;
	vcl::TriMesh m = vcl::io::loadOff<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/bone.off", loadedInfo);

	assert(loadedInfo.hasVertices());
	assert(m.vertexNumber() == 1872);
	assert(loadedInfo.hasVertexColors());
	assert(loadedInfo.hasFaces());
	assert(m.faceNumber() == 3022);
	assert(loadedInfo.hasFaceColors());

	m = vcl::io::loadOff<vcl::TriMesh>(VCL_TEST_MODELS_PATH "/trim-star.off", loadedInfo);

	assert(loadedInfo.hasVertices());
	assert(m.vertexNumber() == 5192);

	return 0;
}
