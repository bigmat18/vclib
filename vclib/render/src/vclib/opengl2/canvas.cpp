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

#include <vclib/opengl2/canvas.h>

#include <vclib/io/image.h>

namespace vcl {

CanvasOpenGL2::CanvasOpenGL2(void*, uint width, uint height, void*)
{
    init(width, height);
}

void CanvasOpenGL2::init(uint width, uint height)
{
    mSize = {width, height};
    glViewport(0, 0, width, height);
    glClearColor(
        mDefaultClearColor.redF(),
        mDefaultClearColor.greenF(),
        mDefaultClearColor.blueF(),
        mDefaultClearColor.alphaF());
}

void CanvasOpenGL2::setDefaultClearColor(const Color& color)
{
    mDefaultClearColor = color;
    glClearColor(
        mDefaultClearColor.redF(),
        mDefaultClearColor.greenF(),
        mDefaultClearColor.blueF(),
        mDefaultClearColor.alphaF());
}

bool CanvasOpenGL2::screenshot(
    const std::string& filename,
    uint               width,
    uint               height)
{
    (void) width;
    (void) height;

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

bool CanvasOpenGL2::readDepth(const Point2i& point, CallbackReadBuffer callback)
{
    if (point.x() < 0 || point.y() < 0 || // point out of bounds
        point.x() >= mSize.x() || point.y() >= mSize.y()) {
        return false;
    }

    mReadDepthPoint     = point;
    mReadBufferCallback = callback;
    return true;
}

void CanvasOpenGL2::onResize(uint width, uint height)
{
    mSize = {width, height};
    glViewport(0, 0, width, height);
}

void CanvasOpenGL2::frame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // if depth requested, read it
    if (mReadBufferCallback) {
        drawContent();
        readDepthData();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    draw();
}

void CanvasOpenGL2::readDepthData()
{
    // get depth range
    std::array<GLfloat, 2> depthRange = {0, 0};
    glGetFloatv(GL_DEPTH_RANGE, depthRange.data());

    // get viewport heigth only
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    // read depth
    GLfloat depth = depthRange[1];
    glReadPixels(
        GLint(mReadDepthPoint.x()),
        GLint(viewport[3] - mReadDepthPoint.y() - 1),
        1,
        1,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
        &depth);

    // normalize depth into [0,1] interval
    depth = (depth - depthRange[0]) / (depthRange[1] - depthRange[0]);

    ReadData rd = FloatData({depth});

    // callback
    mReadBufferCallback(rd);

    // cleanup
    mReadDepthPoint     = {-1, -1};
    mReadBufferCallback = nullptr;
}

} // namespace vcl
