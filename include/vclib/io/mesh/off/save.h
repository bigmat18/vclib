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

#ifndef VCL_IO_OFF_SAVE_H
#define VCL_IO_OFF_SAVE_H

#include <vclib/exceptions/io_exceptions.h>
#include <vclib/io/utils.h>
#include <vclib/io/write.h>
#include <vclib/misc/logger.h>
#include <vclib/misc/mesh_info.h>

namespace vcl {

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveOff(
    const MeshType&    m,
    const std::string& filename,
    const MeshInfo&    info,
    LogType&           log = nullLogger)
{
    MeshInfo meshInfo(m);

    // make sure that the given info contains only components that are actually
    // available in the mesh. meshInfo will contain the intersection between the
    // components that the user wants to save and the components that are
    // available in the mesh.
    meshInfo         = info.intersect(meshInfo);
    std::ofstream fp = openOutputFileStream(filename, "off");
    if (meshInfo.hasVertexNormals())
        fp << "N";
    if (meshInfo.hasVertexColors())
        fp << "C";
    if (meshInfo.hasVertexTexCoords())
        fp << "ST";
    fp << "OFF" << std::endl;

    uint vn = 0;
    uint fn = 0;
    uint en = 0;
    if constexpr (vcl::HasVertices<MeshType>) {
        vn = m.vertexNumber();
    }
    if constexpr (vcl::HasFaces<MeshType>) {
        fn = m.faceNumber();
    }
    if constexpr (vcl::HasEdges<MeshType>) {
        en = m.edgeNumber();
    }

    io::writeInt(fp, vn, false);
    io::writeInt(fp, fn, false);
    io::writeInt(fp, en, false);
    fp << std::endl;

    // vertices
    if constexpr (vcl::HasVertices<MeshType>) {
        using VertexType = MeshType::VertexType;
        for (const VertexType& v : m.vertices()) {
            io::writeDouble(fp, v.coord().x(), false);
            io::writeDouble(fp, v.coord().y(), false);
            io::writeDouble(fp, v.coord().z(), false);

            if constexpr (vcl::HasPerVertexColor<MeshType>) {
                if (meshInfo.hasVertexColors()) {
                    io::writeInt(fp, v.color().red(), false);
                    io::writeInt(fp, v.color().green(), false);
                    io::writeInt(fp, v.color().blue(), false);
                    io::writeInt(fp, v.color().alpha(), false);
                }
            }
            if constexpr (vcl::HasPerVertexNormal<MeshType>) {
                if (meshInfo.hasVertexNormals()) {
                    io::writeDouble(fp, v.normal().x(), false);
                    io::writeDouble(fp, v.normal().y(), false);
                    io::writeDouble(fp, v.normal().z(), false);
                }
            }
            if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
                if (meshInfo.hasVertexTexCoords()) {
                    io::writeDouble(fp, v.texCoord().u(), false);
                    io::writeDouble(fp, v.texCoord().v(), false);
                }
            }

            fp << std::endl;
        }
    }

    // faces
    if constexpr (vcl::HasFaces<MeshType>) {
        using VertexType = MeshType::VertexType;
        using FaceType   = MeshType::FaceType;

        // indices of vertices that do not consider deleted vertices
        std::vector<uint> vIndices = m.vertexCompactIndices();

        for (const FaceType& f : m.faces()) {
            io::writeInt(fp, f.vertexNumber(), false);
            for (const VertexType* v : f.vertices()) {
                io::writeInt(fp, vIndices[m.index(v)], false);
            }
            if constexpr (vcl::HasPerFaceColor<MeshType>) {
                if (meshInfo.hasFaceColors()) {
                    io::writeInt(fp, f.color().red(), false);
                    io::writeInt(fp, f.color().green(), false);
                    io::writeInt(fp, f.color().blue(), false);
                    io::writeInt(fp, f.color().alpha(), false);
                }
            }

            fp << std::endl;
        }
    }
    fp.close();
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveOff(
    const MeshType&    m,
    const std::string& filename,
    LogType&           log = nullLogger)
{
    MeshInfo info(m);
    saveOff(m, filename, info, log);
}

} // namespace vcl

#endif // VCL_IO_OFF_SAVE_H
