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

#include <vclib/render_bgfx/viewer_canvas.h>

#include <vclib/math/min_max.h>
#include <vclib/render/interfaces/drawable_mesh_i.h>
#include <vclib/space/core/box.h>

namespace vcl {

ViewerCanvas::ViewerCanvas(
    void* winId,
    uint  width,
    uint  height,
    void* displayId) :
        Canvas(winId, width, height, displayId), ViewerI(width, height)
{
    mCameraUniforms.updateCamera(DTB::camera());
    mDirectionalLightUniforms.updateLight(DTB::light());
}

ViewerCanvas::ViewerCanvas(
    void*                                        winId,
    const std::shared_ptr<DrawableObjectVector>& v,
    uint                                         width,
    uint                                         height,
    void* displayId) : ViewerCanvas(winId, width, height, displayId)
{
    setDrawableObjectVector(v);
}

void ViewerCanvas::draw()
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

void ViewerCanvas::onResize(unsigned int width, unsigned int height)
{
    Canvas::onResize(width, height);
    DTB::resizeViewer(width, height);
    update();
}

void ViewerCanvas::onKeyPress(Key::Enum key)
{
    ViewerI::onKeyPress(key);
    Canvas::onKeyPress(key);
    update();
}

void ViewerCanvas::onKeyRelease(Key::Enum key)
{
    ViewerI::onKeyRelease(key);
    update();
}

void ViewerCanvas::onMouseMove(double x, double y)
{
    ViewerI::onMouseMove(x, y);
    update();
}

void ViewerCanvas::onMousePress(MouseButton::Enum button)
{
    ViewerI::onMousePress(button);
    update();
}

void ViewerCanvas::onMouseRelease(MouseButton::Enum button)
{
    ViewerI::onMouseRelease(button);
    update();
}

void ViewerCanvas::onMouseScroll(double dx, double dy)
{
    ViewerI::onMouseScroll(dx, dy);
    update();
}

} // namespace vcl
