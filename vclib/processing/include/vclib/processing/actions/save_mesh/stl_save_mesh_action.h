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

#ifndef VCL_PROCESSING_ACTIONS_SAVE_MESH_STL_SAVE_MESH_ACTION_H
#define VCL_PROCESSING_ACTIONS_SAVE_MESH_STL_SAVE_MESH_ACTION_H

#include <vclib/processing/action_interfaces/save_mesh_action.h>
#include <vclib/processing/meshes.h>
#include <vclib/processing/parameters.h>

#include <vclib/load_save/stl/capability.h>
#include <vclib/load_save/stl/save.h>

namespace vcl::proc {

class StlSaveMeshAction : public SaveMeshAction
{
public:
    using SaveMeshAction::save;

    std::string name() const override { return "Save Stl Mesh"; }

    std::shared_ptr<Action> clone() const override
    {
        return std::make_shared<StlSaveMeshAction>(*this);
    }

    ParameterVector parameters() const override
    {
        ParameterVector params;

        params.pushBack(BoolParameter(
            "binary",
            true,
            "Binary",
            "If true, the file will be saved as binary."));
        params.pushBack(BoolParameter(
            "magics_mode",
            false,
            "Magics Mode",
            "Magics mode is a specific file format used by the Magics "
            "software, which includes more information than the standard STL "
            "format, like face colors. The magicsMode works only when the file "
            "is saved as binary."));

        return params;
    }

    std::vector<FileFormat> formats() const override
    {
        return {FileFormat("stl", "STL STereo Lithography interface format")};
    }

    MeshInfo formatCapability() const override { return stlFormatCapability(); }

    void save(
        const std::string&     filename,
        const MeshI&           mesh,
        const MeshInfo&        info,
        const ParameterVector& parameters,
        AbstractLogger&        log = logger()) const override
    {
        // transform saveStl to a lambda function
        auto fun = [&](auto&& m) {
            saveStl(m, filename, info, parameters, log);
        };

        callFunctionForSupportedMeshTypes(mesh, fun);
    }

private:
    void saveStl(
        const MeshConcept auto& mesh,
        const std::string&      filename,
        const MeshInfo&         info,
        const ParameterVector&  parameters,
        AbstractLogger&         log) const
    {
        vcl::SaveSettings settings;
        settings.info       = info;
        settings.binary     = parameters.get("binary")->boolValue();
        settings.magicsMode = parameters.get("magics_mode")->boolValue();
        vcl::saveStl(mesh, filename, settings, log);
    }
};

} // namespace vcl::proc

#endif // VCL_PROCESSING_ACTIONS_SAVE_MESH_STL_SAVE_MESH_ACTION_H
