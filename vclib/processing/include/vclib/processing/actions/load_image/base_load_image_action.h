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

#ifndef VCL_PROCESSING_ACTIONS_LOAD_IMAGE_BASE_LOAD_IMAGE_ACTION_H
#define VCL_PROCESSING_ACTIONS_LOAD_IMAGE_BASE_LOAD_IMAGE_ACTION_H

#include <vclib/processing/action_interfaces/load_image_action.h>

namespace vcl::proc {

class BaseLoadImageAction : public LoadImageAction
{
public:
    BaseLoadImageAction()  = default;
    ~BaseLoadImageAction() = default;

    std::string name() const override { return "Base Load Image"; }

    std::shared_ptr<Action> clone() const override
    {
        return std::make_shared<BaseLoadImageAction>(*this);
    }

    std::vector<FileFormat> formats() const override
    {
        std::vector<FileFormat> formats;
        formats.push_back(FileFormat("png", "Portable Network Graphics"));
        formats.push_back(FileFormat("bmp", "Bitmap"));
        formats.push_back(FileFormat("tga", "Truevision TGA"));
        formats.push_back(FileFormat(
            std::vector<std::string> {"jpg", "jpeg"},
            "Joint Photographic Experts Group"));

        return formats;
    }

    Image load(const std::string& filename, AbstractLogger& log = logger())
        const override
    {
        return Image(filename);
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTIONS_LOAD_IMAGE_BASE_LOAD_IMAGE_ACTION_H
