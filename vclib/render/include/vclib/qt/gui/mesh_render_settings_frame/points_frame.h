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

#ifndef VCL_QT_GUI_MESH_RENDER_SETTINGS_FRAME_POINTS_FRAME_H
#define VCL_QT_GUI_MESH_RENDER_SETTINGS_FRAME_POINTS_FRAME_H

#include "generic_mesh_render_settings_frame.h"

#include <vclib/render/drawable/mesh/mesh_render_settings.h>

#include <QFrame>

namespace vcl::qt {

namespace Ui {
class PointsFrame;
} // namespace Ui

class PointsFrame : public GenericMeshRenderSettingsFrame
{
    Q_OBJECT

    Ui::PointsFrame* mUI;

public:
    explicit PointsFrame(
        MeshRenderSettings& settings,
        QWidget*            parent = nullptr);
    ~PointsFrame();

    void updateFrameFromSettings() override;

private:
    enum POINT_COLOR { P_VERT = 0, P_MESH, P_USER };

    void updateColorComboBoxFromSettings();

private slots:
    void onVisibilityChanged(int arg1);
    void onShapeCircleToggled(bool checked);
    void onShapePixelToggled(bool checked);
    void onShadingVertexToggled(bool checked);
    void onShadingNoneToggled(bool checked);
    void onColorComboBoxChanged(int index);
    void onColorDialogButtonClicked();
    void onSizeChanged(int value);
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_MESH_RENDER_SETTINGS_FRAME_POINTS_FRAME_H
