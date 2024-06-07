/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_IO_STL_SAVE_H
#define VCL_IO_STL_SAVE_H

#include <vclib/algorithms/core/polygon.h>
#include <vclib/io/mesh/settings.h>
#include <vclib/io/write.h>
#include <vclib/mesh/utils/mesh_info.h>
#include <vclib/misc/logger.h>

namespace vcl {

namespace detail {

inline void writeStlHeader(std::ostream& fp, const SaveSettings& settings)
{
    std::string header = "solid STL generated by VCLib";
    if (settings.binary) {
        if (settings.magicsMode) {
            std::string p;
            for (uint i = 0; i < 3; i++)
                p.push_back((char) 0x7f);
            header = "COLOR=" + p + " MATERIAL=" + p + " " + p + " " + p;
        }
        while (header.size() < 80)
            header.push_back(' ');
    }

    fp << header;
    if (!settings.binary)
        fp << std::endl;
}

template<Point3Concept PointType, Point3Concept NormalType>
void writeSTLTriangle(
    std::ostream&       fp,
    const PointType&    p0,
    const PointType&    p1,
    const PointType&    p2,
    const NormalType&   n,
    uint                attributes,
    const SaveSettings& settings)
{
    if (settings.binary) {
        for (uint i = 0; i < 3; ++i)
            io::writeFloat(fp, n[i]);

        for (uint i = 0; i < 3; ++i)
            io::writeFloat(fp, p0[i]);

        for (uint i = 0; i < 3; ++i)
            io::writeFloat(fp, p1[i]);

        for (uint i = 0; i < 3; ++i)
            io::writeFloat(fp, p2[i]);

        io::writeUShort(fp, attributes);
    }
    else {
        fp << "  facet normal " << n.x() << " " << n.y() << " " << n.z()
           << std::endl;
        fp << "    outer loop" << std::endl;

        fp << "      vertex " << p0.x() << " " << p0.y() << " " << p0.z()
           << std::endl;
        fp << "      vertex " << p1.x() << " " << p1.y() << " " << p1.z()
           << std::endl;
        fp << "      vertex " << p2.x() << " " << p2.y() << " " << p2.z()
           << std::endl;

        fp << "    endloop" << std::endl;
        fp << "  endfacet" << std::endl;
    }
}

} // namespace detail

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveStl(
    const MeshType&     m,
    std::ostream&       fp,
    LogType&            log      = nullLogger,
    const SaveSettings& settings = SaveSettings())
{
    MeshInfo meshInfo(m);

    // make sure that the given info contains only components that are actually
    // available in the mesh. meshInfo will contain the intersection between the
    // components that the user wants to save and the components that are
    // available in the mesh.
    if (!settings.info.isEmpty())
        meshInfo = settings.info.intersect(meshInfo);

    log.log(0, "Saving STL file");

    detail::writeStlHeader(fp, settings);

    if constexpr (HasFaces<MeshType>) {
        using FaceType = MeshType::FaceType;

        if (settings.binary) {
            io::writeInt(fp, m.faceNumber());
        }

        log.startProgress("Saving STL file", m.faceNumber());

        for (const FaceType& f : m.faces()) {
            // For each triangle write the normal, the three coords and a short
            auto n = vcl::faceNormal(f);

            unsigned short attributes = 0;

            if constexpr (HasPerFaceColor<MeshType>) {
                if (meshInfo.hasFaceColors()) {
                    if (settings.magicsMode)
                        attributes = 32768 | f.color().bgr5();
                    else
                        attributes = 32768 | f.color().rgb5();
                }
            }

            if (f.vertexNumber() == 3) {
                detail::writeSTLTriangle(
                    fp,
                    f.vertex(0)->coord(),
                    f.vertex(1)->coord(),
                    f.vertex(2)->coord(),
                    n,
                    attributes,
                    settings);
            }
            else {
                std::vector<uint> tris = vcl::earCut(f);
                for (uint i = 0; i < tris.size(); i += 3) {
                    detail::writeSTLTriangle(
                        fp,
                        f.vertex(tris[i])->coord(),
                        f.vertex(tris[i + 1])->coord(),
                        f.vertex(tris[i + 2])->coord(),
                        n,
                        attributes,
                        settings);
                }
            }

            log.progress(m.index(f));
        }

        log.endProgress();
    }

    if (!settings.binary) {
        fp << "endsolid VCLib" << std::endl;
    }
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveStl(
    const MeshType&     m,
    std::ostream&       fp,
    const SaveSettings& settings,
    LogType&            log = nullLogger)
{
    saveStl(m, fp, log, settings);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveStl(
    const MeshType&     m,
    const std::string&  filename,
    LogType&            log      = nullLogger,
    const SaveSettings& settings = SaveSettings())
{
    std::ofstream fp = openOutputFileStream(filename, "stl");

    saveStl(m, fp, log, settings);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveStl(
    const MeshType&     m,
    const std::string&  filename,
    const SaveSettings& settings,
    LogType&            log = nullLogger)
{
    saveStl(m, filename, log, settings);
}

} // namespace vcl

#endif // VCL_IO_STL_SAVE_H
