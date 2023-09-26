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

#ifndef VCL_IO_PLY_EXTRA_H
#define VCL_IO_PLY_EXTRA_H

#include <fstream>

#include <vclib/mesh/requirements.h>

#include "ply_header.h"
#include "../internal/io_read.h"

namespace vcl::io::ply {

/******************************************************************************
 *                                Declarations                                *
 ******************************************************************************/

template<MeshConcept MeshType>
void loadTextures(const PlyHeader& header, MeshType& mesh);

template<MeshConcept MeshType>
void saveTextures(PlyHeader& header, const MeshType& mesh);

void readUnknownElements(std::ifstream& file, const PlyHeader& header, Element el);

/******************************************************************************
 *                                Definitions                                 *
 ******************************************************************************/

template<MeshConcept MeshType>
void loadTextures(const PlyHeader& header, MeshType& mesh)
{
	if constexpr (vcl::HasTexturePaths<MeshType>) {
		for (const std::string& str : header.textureFileNames()) {
			mesh.pushTexturePath(str);
		}
	}
}

template<MeshConcept MeshType>
void saveTextures(PlyHeader& header, const MeshType& mesh)
{
	if constexpr (vcl::HasTexturePaths<MeshType>) {
		for (const std::string& str : mesh.texturePaths()) {
			header.pushTextureFileName(str);
		}
	}
}

void readUnknownElements(std::ifstream& file, const PlyHeader& header, ply::Element el)
{
	if (header.format() == ply::ASCII) {
		for (uint i = 0; i < el.numberElements; ++i) {
			io::internal::nextNonEmptyTokenizedLine(file);
		}
	}
	else {
		for (uint i = 0; i < el.numberElements; ++i) {
			for (const Property& p : el.properties) {
				if (p.list) {
					uint s = io::internal::readProperty<int>(file, p.listSizeType);
					for (uint i = 0; i < s; ++i)
						io::internal::readProperty<int>(file, p.type);
				}
				else {
					io::internal::readProperty<int>(file, p.type);
				}
			}
		}
	}
}

} // namespace vcl::ply

#endif // VCL_IO_PLY_EXTRA_H
