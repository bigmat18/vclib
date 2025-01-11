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

#include "hello_triangle_glfw.h"

#include "../common.h"

#include <iostream>

HelloTriangleGLFW::HelloTriangleGLFW() :
        vcl::glfw::CanvasWindow("Hello Triangle GLFW")
{
}

HelloTriangleGLFW::~HelloTriangleGLFW()
{
}

void HelloTriangleGLFW::drawContent()
{
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    glColor4ubv((GLubyte*) &(vertices[0].abgr));
    glVertex2fv((GLfloat*) &(vertices[0].pos));
    glColor4ubv((GLubyte*) &(vertices[1].abgr));
    glVertex2fv((GLfloat*) &(vertices[1].pos));
    glColor4ubv((GLubyte*) &(vertices[2].abgr));
    glVertex2fv((GLfloat*) &(vertices[2].pos));
    glEnd();
}
