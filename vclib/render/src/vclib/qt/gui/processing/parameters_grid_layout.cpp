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

#include <vclib/qt/gui/processing/parameters_grid_layout.h>

#include <vclib/qt/gui/processing/parameters/bool_parameter_row.h>
#include <vclib/qt/gui/processing/parameters/enum_parameter_row.h>
#include <vclib/qt/gui/processing/parameters/scalar_parameter_row.h>
#include <vclib/qt/gui/processing/parameters/uint_parameter_row.h>
#include <vclib/qt/gui/processing/parameters/uscalar_parameter_row.h>

namespace vcl::qt {

ParametersGridLayout::ParametersGridLayout(QWidget* parent) :
        QGridLayout(parent)
{
}

void ParametersGridLayout::setParameters(
    const proc::ParameterVector& parameters)
{
    clear();
    mParameters = parameters;

    uint row = 0;
    for (const std::shared_ptr<const proc::Parameter>& par : parameters) {
        if (par) {
            bool                          paramInserted = false;
            std::shared_ptr<ParameterRow> parRow;

            switch (par->type()) {
            case proc::ParameterType::BOOL: {
                std::shared_ptr<const proc::BoolParameter> bp =
                    std::dynamic_pointer_cast<const proc::BoolParameter>(par);
                parRow = std::make_shared<BoolParameterRow>(*bp);

                paramInserted = true;

            } break;
            case proc::ParameterType::ENUM: {
                std::shared_ptr<const proc::EnumParameter> ep =
                    std::dynamic_pointer_cast<const proc::EnumParameter>(par);
                parRow = std::make_shared<EnumParameterRow>(*ep);

                paramInserted = true;
            } break;
            case proc::ParameterType::SCALAR: {
                std::shared_ptr<const proc::ScalarParameter> sp =
                    std::dynamic_pointer_cast<const proc::ScalarParameter>(par);
                parRow = std::make_shared<ScalarParameterRow>(*sp);

                paramInserted = true;
            } break;
            case proc::ParameterType::UINT: {
                std::shared_ptr<const proc::UintParameter> up =
                    std::dynamic_pointer_cast<const proc::UintParameter>(par);
                parRow = std::make_shared<UintParameterRow>(*up);

                paramInserted = true;
            } break;
            case proc::ParameterType::USCALAR: {
                std::shared_ptr<const proc::UscalarParameter> usp =
                    std::dynamic_pointer_cast<const proc::UscalarParameter>(
                        par);
                parRow = std::make_shared<UscalarParameterRow>(*usp);

                paramInserted = true;
            } break;
            default: break;
            }
            if (paramInserted) {
                parRow->addRowToGridLayout(this, row);

                mRows.push_back(parRow);
                ++row;
            }
        }
    }
}

proc::ParameterVector ParametersGridLayout::parameters() const
{
    proc::ParameterVector pars = mParameters;
    for (const std::shared_ptr<ParameterRow>& row : mRows) {
        if (row->hasBeenModified()) {
            auto par = row->parameterFromWidget();
            pars.get(par->name())->setValue(*par);
        }
    }
    return pars;
}

void ParametersGridLayout::setHelpVisible(bool b)
{
    for (const std::shared_ptr<ParameterRow>& row : mRows)
        row->setHelpVisible(b);
}

void ParametersGridLayout::clear()
{
    clear(this);
    mParameters.clear();
    mRows.clear();
}

// https://stackoverflow.com/a/7077340/5851101
void ParametersGridLayout::clear(QLayout* layout)
{
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QWidget* widget = item->widget())
            widget->deleteLater();
        if (QLayout* childLayout = item->layout())
            clear(childLayout);
        delete item;
    }
}

} // namespace vcl::qt
