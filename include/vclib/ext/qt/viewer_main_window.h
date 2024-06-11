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

#ifndef VCL_EXT_QT_VIEWER_MAIN_WINDOW_H
#define VCL_EXT_QT_VIEWER_MAIN_WINDOW_H

#include <QMainWindow>

#include <vclib/processing/action_manager.h>
#include <vclib/render/drawable/drawable_object_vector.h>

#include <vclib/ext/qt/gui/text_edit_logger.h>

namespace vcl::qt {

namespace Ui {
class ViewerMainWindow;
} // namespace Ui

class ViewerMainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::ViewerMainWindow*                      mUI;
    std::shared_ptr<vcl::DrawableObjectVector> mDrawVector;
    proc::ActionManager                        mActionManager;

public:
    explicit ViewerMainWindow(QWidget* parent = nullptr);

    ~ViewerMainWindow();

    void setDrawableObjectVector(
        const std::shared_ptr<vcl::DrawableObjectVector>& v);

    TextEditLogger& logger();

public slots:
    void visibilityDrawableObjectChanged();

    void selectedDrawableObjectChanged(uint i);

    void renderSettingsUpdated();

private slots:
    void on_actionSave_triggered();
    void on_actionShow_Right_Bar_triggered(bool checked);
    void on_actionShow_Logger_triggered(bool checked);
    void on_actionShow_Mesh_Render_Settings_triggered(bool checked);
    void on_actionShow_Mesh_List_triggered(bool checked);
};

} // namespace vcl::qt

#endif // VCL_EXT_QT_VIEWER_MAIN_WINDOW_H
