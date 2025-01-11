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

#ifndef VCL_PROCESSING_ACTION_INTERFACES_LOAD_IMAGE_ACTION_H
#define VCL_PROCESSING_ACTION_INTERFACES_LOAD_IMAGE_ACTION_H

#include "action.h"

#include <vclib/io/file_format.h>
#include <vclib/space/core/image.h>

#include <vector>

namespace vcl::proc {

class LoadImageAction : public Action
{
public:
    uint type() const final { return ActionType::LOAD_IMAGE_ACTION; }

    virtual std::vector<FileFormat> formats() const = 0;

    virtual Image load(
        const std::string& filename,
        AbstractLogger&    log = logger()) const = 0;
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTION_INTERFACES_LOAD_IMAGE_ACTION_H
