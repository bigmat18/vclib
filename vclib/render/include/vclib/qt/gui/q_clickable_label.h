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

#ifndef VCL_QT_GUI_Q_CLICKABLE_LABEL_H
#define VCL_QT_GUI_Q_CLICKABLE_LABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>

namespace vcl::qt {

/**
 * @brief The QClickableLabel class
 *
 * A QLabel class which can be clicked (clicked event).
 *
 * Usage: just Promote a QLabel to a vcl::QClickableLabel
 *
 * @link https://wiki.qt.io/Clickable_QLabel
 */
class QClickableLabel : public QLabel
{
    Q_OBJECT

public:
    explicit QClickableLabel(
        QWidget*        parent = Q_NULLPTR,
        Qt::WindowFlags f      = Qt::WindowFlags());
    ~QClickableLabel();

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event);
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_Q_CLICKABLE_LABEL_H
