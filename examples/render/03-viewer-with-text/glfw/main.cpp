/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#include "get_drawable_mesh.h"

#include <vclib/glfw/viewer_window.h>

int main(int argc, char** argv)
{
    vcl::glfw::ViewerWindow tw("Viewer GLFW");

    // load and set up a drawable mesh
    vcl::DrawableMesh<vcl::TriMesh> m =
        getDrawableMesh<vcl::TriMesh>("greek_helmet.obj");

    // add the drawable mesh to the scene
    // the viewer will own **a copy** of the drawable mesh
    tw.pushDrawableObject(m);

    tw.enableText();

    tw.setTextFont(vcl::VclFont::DROID_SANS, 20);
    tw.appendStaticText(
        {5, 5}, "Vertices: " + std::to_string(m.vertexNumber()));
    tw.appendStaticText({5, 30}, "Faces: " + std::to_string(m.faceNumber()));

    tw.fitScene();

    tw.show();

    return 0;
}