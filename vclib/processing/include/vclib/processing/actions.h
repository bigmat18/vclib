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

#ifndef VCL_PROCESSING_ACTIONS_H
#define VCL_PROCESSING_ACTIONS_H

#include "actions/filter_mesh.h"
#include "actions/load_image.h"
#include "actions/load_mesh.h"
#include "actions/save_image.h"
#include "actions/save_mesh.h"

namespace vcl::proc {

std::vector<std::shared_ptr<Action>> vclibActions()
{
    std::vector<std::shared_ptr<Action>> vec;

    // Load actions
    auto loadImageActions = vclibLoadImageActions();
    vec.insert(vec.end(), loadImageActions.begin(), loadImageActions.end());

    auto loadMeshActions = vclibLoadMeshActions();
    vec.insert(vec.end(), loadMeshActions.begin(), loadMeshActions.end());

    // Save actions
    auto saveImageActions = vclibSaveImageActions();
    vec.insert(vec.end(), saveImageActions.begin(), saveImageActions.end());

    auto saveMeshActions = vclibSaveMeshActions();
    vec.insert(vec.end(), saveMeshActions.begin(), saveMeshActions.end());

    // Filter actions
    auto filterMeshActions = vclibFilterMeshActions();
    vec.insert(vec.end(), filterMeshActions.begin(), filterMeshActions.end());

    return vec;
}

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTIONS_H
