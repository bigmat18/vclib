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

#include <vclib/processing.h>

#include <vclib/load_save.h>

int main()
{
    vcl::proc::ActionManager manager;

    manager.add(vcl::proc::vclibActions());

    auto pm0 = manager.loadMeshAction("obj")->load(VCLIB_EXAMPLE_MESHES_PATH
                                                   "/bunny.obj");

    std::vector<std::shared_ptr<vcl::proc::MeshI>> mv;

    mv.push_back(pm0);

    manager.filterMeshActionByName("Laplacian Smoothing")->applyFilter(mv);

    manager.saveMeshAction("ply")->save(
        VCLIB_RESULTS_PATH "/smoothed_bunny.ply", *mv.front());

    return 0;
}
