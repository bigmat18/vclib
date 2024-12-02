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

#include <vclib/render_opengl2/canvas.h>

#include <vclib/io/image.h>

namespace vcl {

Canvas::Canvas(void*, uint width, uint height, void*)
{
    init(width, height);
}

void Canvas::init(uint width, uint height)
{
    mSize = {width, height};
    glViewport(0, 0, width, height);
    glClearColor(1.f, 1.f, 1.f, 1.0f);
}

bool Canvas::screenshot(const std::string& filename, uint width, uint height)
{
    (void)width;
    (void)height;

    std::vector<std::uint8_t> buffer(mSize.x() * mSize.y() * 4);
    // read pixels
    glReadPixels(
        0,
        0,
        GLsizei(mSize.x()),
        GLsizei(mSize.y()),
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        buffer.data());

    // write image using stb
    bool ret = true;
    stbi_flip_vertically_on_write(1);
    try {
        saveImageData(filename, mSize.x(), mSize.y(), buffer.data());
    }
    catch (const std::exception& e) {
        ret = false;
    }
    stbi_flip_vertically_on_write(0);

    return ret;
}

bool Canvas::readDepth(
    const Point2i& point,
    CallbackReadBuffer callback)
{
    if (point.x() < 0 || point.y() < 0 ||// point out of bounds
        point.x() >= mSize.x() || point.y() >= mSize.y()) {
        return false;
    }

    mReadDepthPoint = point;
    mReadBufferCallback = callback;
    return true;
}

void Canvas::onResize(uint width, uint height)
{
    mSize = {width, height};
    glViewport(0, 0, width, height);
}

void Canvas::frame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // if depth requested, read it
    if (mReadBufferCallback)
    {
        drawContent();
        readDepthData();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    draw();
}

void Canvas::readDepthData()
{
     // get depth range
    std::array<GLfloat,2> depthRange = {0,0};
    glGetFloatv(GL_DEPTH_RANGE, depthRange.data());

    // get viewport heigth only
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    // read depth
    GLfloat depth = depthRange[1];
    glReadPixels(
        GLint(mReadDepthPoint.x()),
        GLint(viewport[3] - mReadDepthPoint.y() - 1),
        1, 1,
        GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    
    // normalize depth into [0,1] interval
    depth = (depth - depthRange[0]) / (depthRange[1] - depthRange[0]);

    // callback
    mReadBufferCallback({depth});
    
    // cleanup
    mReadDepthPoint = {-1,-1};
    mReadBufferCallback = nullptr;
}

} // namespace vcl
