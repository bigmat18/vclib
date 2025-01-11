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

#ifndef VCL_GLFW_VIEWER_WINDOW_H
#define VCL_GLFW_VIEWER_WINDOW_H

#include "event_manager_window.h"

#include <vclib/render/viewer_canvas.h>

namespace vcl::glfw {

class ViewerWindow : public EventManagerWindow, public vcl::ViewerCanvas
{
public:
    ViewerWindow(
        const std::shared_ptr<DrawableObjectVector>& v,
        const std::string& windowTitle = "Minimal Viewer",
        uint               width       = 1024,
        uint               height      = 768,
        void*              parent      = nullptr);

    ViewerWindow(
        const std::string& windowTitle = "Minimal Viewer",
        uint               width       = 1024,
        uint               height      = 768,
        void*              parent      = nullptr);

    ViewerWindow(void* parent);

    ~ViewerWindow() override = default;

    virtual void show();

protected:
    void onKeyPress(Key::Enum key) override;

    void onResize(uint width, uint height) override;
};

} // namespace vcl::glfw

#endif // VCL_GLFW_VIEWER_WINDOW_H
