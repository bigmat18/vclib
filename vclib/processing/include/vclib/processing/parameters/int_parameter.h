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

#ifndef VCL_PROCESSING_PARAMETERS_INT_PARAMETER_H
#define VCL_PROCESSING_PARAMETERS_INT_PARAMETER_H

#include "parameter.h"

namespace vcl::proc {

class IntParameter : public Parameter
{
public:
    IntParameter(
        const std::string& name,
        int                value,
        const std::string& description = "",
        const std::string& tooltip     = "",
        const std::string& category    = "") :
            Parameter(name, value, description, tooltip, category)
    {
    }

    ParameterType::Enum type() const override { return ParameterType::INT; }

    std::shared_ptr<Parameter> clone() const override
    {
        return std::make_shared<IntParameter>(*this);
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_PARAMETERS_INT_PARAMETER_H
