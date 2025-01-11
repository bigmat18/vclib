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

#include <vclib/bgfx/viewer_canvas.h>

#include <vclib/math/min_max.h>
#include <vclib/render/interfaces/drawable_mesh_i.h>
#include <vclib/space/core/box.h>

namespace vcl {

ViewerCanvasBGFX::ViewerCanvasBGFX(
    void* winId,
    uint  width,
    uint  height,
    void* displayId) :
        CanvasBGFX(winId, width, height, displayId), ViewerI(width, height)
{
    CanvasBGFX::setDefaultClearColor(Color::White);
    mCameraUniforms.updateCamera(DTB::camera());
    mDirectionalLightUniforms.updateLight(DTB::light());
}

ViewerCanvasBGFX::ViewerCanvasBGFX(
    void*                                        winId,
    const std::shared_ptr<DrawableObjectVector>& v,
    uint                                         width,
    uint                                         height,
    void* displayId) : ViewerCanvasBGFX(winId, width, height, displayId)
{
    setDrawableObjectVector(v);
}

void ViewerCanvasBGFX::draw()
{
    drawContent();

    if (mAxis.isVisible()) {
        mAxis.draw(viewId());
    }

    if (mDirectionalLight.isVisible()) {
        mDirectionalLight.draw(viewId());
    }

    if (mDrawTrackBall.isVisible()) {
        mDrawTrackBall.draw(viewId());
    }
}

void ViewerCanvasBGFX::drawContent()
{
    setDirectionalLightVisibility(
        currentMotion() == DTB::TrackBallType::DIR_LIGHT_ARC);
    updateDirectionalLight();
    updateDrawableTrackball();

    bgfx::setViewTransform(
        viewId(), viewMatrix().data(), projectionMatrix().data());

    mCameraUniforms.updateCamera(DTB::camera());
    mCameraUniforms.bind();

    mDirectionalLightUniforms.bind();

    for (auto obj : drawableObjectVector())
        obj->draw(viewId());
}

void ViewerCanvasBGFX::onResize(unsigned int width, unsigned int height)
{
    CanvasBGFX::onResize(width, height);
    DTB::resizeViewer(width, height);
    update();
}

void ViewerCanvasBGFX::onKeyPress(Key::Enum key)
{
    ViewerI::onKeyPress(key);
    CanvasBGFX::onKeyPress(key);
    update();
}

void ViewerCanvasBGFX::onKeyRelease(Key::Enum key)
{
    ViewerI::onKeyRelease(key);
    update();
}

void ViewerCanvasBGFX::onMouseMove(double x, double y)
{
    ViewerI::onMouseMove(x, y);
    update();
}

void ViewerCanvasBGFX::onMousePress(MouseButton::Enum button)
{
    ViewerI::onMousePress(button);
    update();
}

void ViewerCanvasBGFX::onMouseRelease(MouseButton::Enum button)
{
    ViewerI::onMouseRelease(button);
    update();
}

void ViewerCanvasBGFX::onMouseScroll(double dx, double dy)
{
    ViewerI::onMouseScroll(dx, dy);
    update();
}

void ViewerCanvasBGFX::onMouseDoubleClick(
    MouseButton::Enum button,
    double            x,
    double            y)
{
    // FIXME: code duplication for both OpenGL2 and BGFX
    if (mReadRequested)
        return;

    // get point
    const Point2d p(x, y);

    // get the homogeneous NDC flag
    const bool homogeneousNDC =
        Context::instance().capabilites().homogeneousDepth;

    // matrices
    const auto proj = projectionMatrix();
    const auto view = viewMatrix();
    // viewport
    const Point4f vp = {.0f, .0f, float(size().x()), float(size().y())};
    // create the callback
    auto callback = [=, this](const ReadData& data) {
        assert(std::holds_alternative<std::vector<float>>(data));
        const auto& d  = std::get<std::vector<float>>(data);
        mReadRequested = false;

        // if the depth is 1.0, the point is not in the scene
        const float depth = d[0];
        if (depth == 1.0f) {
            return;
        }

        // unproject the point
        const Point3f p2d(p.x(), vp[3] - p.y(), depth);
        const auto    unproj = unproject(
            p2d, Matrix44<ScalarType>(proj * view), vp, homogeneousNDC);

        this->focus(unproj);
        this->update();
    };

    mReadRequested = this->readDepth(Point2i(p.x(), p.y()), callback);
    if (mReadRequested)
        update();
}

} // namespace vcl
