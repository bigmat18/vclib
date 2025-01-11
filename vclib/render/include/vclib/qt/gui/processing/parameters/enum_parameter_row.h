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

#ifndef VCL_QT_GUI_PROCESSING_PARAMETERS_ENUM_PARAMETER_ROW_H
#define VCL_QT_GUI_PROCESSING_PARAMETERS_ENUM_PARAMETER_ROW_H

#include <QComboBox>

#include "parameter_row.h"

namespace vcl::qt {

class EnumParameterRow : public ParameterRow
{
    proc::EnumParameter mParam;

    QComboBox* mComboBox = nullptr;

public:
    EnumParameterRow(const proc::EnumParameter& param);

    // ParameterRow interface
    QWidget* parameterWidget() override;

    std::shared_ptr<proc::Parameter> parameterFromWidget() const override;
};

} // namespace vcl::qt

#endif // VCL_QT_GUI_PROCESSING_PARAMETERS_ENUM_PARAMETER_ROW_H
