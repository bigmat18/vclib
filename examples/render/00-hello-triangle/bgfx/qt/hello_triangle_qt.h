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

#ifndef HELLO_TRIANGLE_QT_H
#define HELLO_TRIANGLE_QT_H

#include <vclib/qt/canvas_widget.h>

class HelloTriangleQt : public vcl::qt::CanvasWidget
{
public:
    HelloTriangleQt();

    ~HelloTriangleQt();

    void draw() override;

private:
    bgfx::VertexBufferHandle vbh;

    bgfx::ProgramHandle program;
};

#endif // HELLO_TRIANGLE_QT_H
