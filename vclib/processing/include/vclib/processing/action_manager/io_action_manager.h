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

#ifndef VCL_PROCESSING_ACTION_MANAGER_IO_ACTION_MANAGER_H
#define VCL_PROCESSING_ACTION_MANAGER_IO_ACTION_MANAGER_H

#include <vclib/io/file_format.h>
#include <vclib/processing/action_interfaces/action.h>
#include <vclib/space/core/vector/polymorphic_object_vector.h>

#include <map>

namespace vcl::proc {

template<typename IOAction>
class IOActionManager
{
    vcl::PolymorphicObjectVector<Action> mActions;

    std::map<FileFormat, std::shared_ptr<IOAction>> mFormatMap;

public:
    IOActionManager() = default;

    void add(std::shared_ptr<IOAction> action)
    {
        if (!action) {
            throw std::runtime_error("Action is nullptr.");
        }
        std::vector<FileFormat> formats = action->formats();

        for (const auto& format : formats) {
            checkFormatDoesNotExist(format);
            mActions.pushBack(action);
            mFormatMap[format] =
                std::dynamic_pointer_cast<IOAction>(mActions.back());
        }
    }

    std::shared_ptr<IOAction> get(const FileFormat& format) const
    {
        checkFormatExists(format);
        return mFormatMap.at(format);
    }

    std::vector<FileFormat> formats() const
    {
        std::vector<FileFormat> formats;
        for (const auto& [format, _] : mFormatMap) {
            formats.push_back(format);
        }
        return formats;
    }

private:
    void checkFormatDoesNotExist(const FileFormat& format) const
    {
        if (mFormatMap.find(format) != mFormatMap.end()) {
            throw std::runtime_error("Format already registered.");
        }
    }

    void checkFormatExists(const FileFormat& format) const
    {
        if (mFormatMap.find(format) == mFormatMap.end()) {
            throw std::runtime_error("Format not registered.");
        }
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTION_MANAGER_IO_ACTION_MANAGER_H
