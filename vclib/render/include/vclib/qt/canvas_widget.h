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

#ifndef VCL_QT_CANVAS_WIDGET_H
#define VCL_QT_CANVAS_WIDGET_H

#include "event_manager_widget.h"

#include <vclib/render/canvas.h>

#include <QVBoxLayout>

namespace vcl::qt {

class CanvasWidget : public EventManagerWidget, public vcl::Canvas
{
    using Canvas = vcl::Canvas;

public:
    explicit CanvasWidget(
        const std::string& windowTitle,
        uint               width  = 1024,
        uint               height = 768,
        QWidget*           parent = nullptr);

    explicit CanvasWidget(
        uint     width  = 1024,
        uint     height = 768,
        QWidget* parent = nullptr);

    explicit CanvasWidget(QWidget* parent);

    virtual ~CanvasWidget();

#if defined(VCLIB_RENDER_BACKEND_OPENGL2)
    void initializeGL() override;
#endif

private:
#if defined(VCLIB_RENDER_BACKEND_BGFX)
    void paintEvent(QPaintEvent* event) override;
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
    void paintGL() override;
#endif
};

} // namespace vcl::qt

#endif // VCL_QT_CANVAS_WIDGET_H
