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

#ifndef VCL_OPENGL2_VIEWER_CANVAS_H
#define VCL_OPENGL2_VIEWER_CANVAS_H

#include <vclib/render/interfaces/viewer_i.h>

#include <vclib/opengl2/canvas.h>

#include <memory>

namespace vcl {

class ViewerCanvasOpenGL2 : public vcl::CanvasOpenGL2, public ViewerI
{
public:
    ViewerCanvasOpenGL2(
        void* winId,
        uint  width     = 1024,
        uint  height    = 768,
        void* displayId = nullptr);

    ViewerCanvasOpenGL2(
        void*                                        winId,
        const std::shared_ptr<DrawableObjectVector>& v,
        uint                                         width     = 1024,
        uint                                         height    = 768,
        void*                                        displayId = nullptr);

    void init(uint width, uint height);

    void toggleAxisVisibility() override
    {
        // todo
    }

    void toggleTrackBallVisibility() override
    {
        // todo
    }

protected:
    void drawContent() override;

    // events
    void onResize(unsigned int width, unsigned int height) override;

    void onMouseDoubleClick(MouseButton::Enum button, double x, double y)
        override;

private:
    bool mReadRequested = false;
};

} // namespace vcl

#endif // VCL_OPENGL2_VIEWER_CANVAS_H
