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

#ifndef COMMON_H
#define COMMON_H

#include <vclib/algorithms/mesh/update/color.h>
#include <vclib/algorithms/mesh/update/normal.h>
#include <vclib/load_save.h>
#include <vclib/meshes.h>

#include <vclib/render/drawable/drawable_mesh.h>

template<vcl::MeshConcept MeshType>
vcl::DrawableMesh<MeshType> getDrawableMesh()
{
    vcl::LoadSettings s;
    s.loadTextureImages = true;
    // load a mesh:
    // MeshType m =
    //     vcl::load<MeshType>("/home/alessandro/tmp/chimera/chimera.ply");
    // MeshType m = vcl::load<MeshType>(VCLIB_EXAMPLE_MESHES_PATH
    // "/greek_helmet.obj"); MeshType m =
    // vcl::load<MeshType>(VCLIB_EXAMPLE_MESHES_PATH
    // "/bunny_textured.ply");
    // MeshType m = vcl::load<MeshType>(VCLIB_EXAMPLE_MESHES_PATH "/bimba.obj");
    MeshType m =
        vcl::load<MeshType>(VCLIB_EXAMPLE_MESHES_PATH "/TextureDouble.ply", s);
    vcl::updatePerVertexAndFaceNormals(m);

    // enable the vertex color of the mesh and set it to gray
    m.enablePerVertexColor();
    vcl::setPerVertexColor(m, vcl::Color::Gray);
    m.enablePerFaceColor();
    for (auto& f : m.faces()) {
        if (f.index() % 3 == 0)
            f.color() = vcl::Color::Red;
        else if (f.index() % 3 == 1)
            f.color() = vcl::Color::Green;
        else
            f.color() = vcl::Color::Blue;
    }

    // create a MeshRenderSettings object, that allows to set the rendering
    // options of the mesh
    // default is what we want: color per vertex, smooth shading, no wireframe
    vcl::MeshRenderSettings settings(m);

    // create a DrawableMesh object from the mesh
    vcl::DrawableMesh<MeshType> drawable(m);

    // set the settings to the drawable mesh
    drawable.setRenderSettings(settings);

    return drawable;
}

#endif // COMMON_H
