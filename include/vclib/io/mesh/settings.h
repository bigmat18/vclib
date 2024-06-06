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

#ifndef VCL_IO_MESH_SETTINGS_H
#define VCL_IO_MESH_SETTINGS_H

namespace vcl {

/**
 * @brief The LoadSettings structure contains the settings that can be used to
 * load a mesh from a stream/file.
 */
struct LoadSettings
{
    /**
     * @brief If true, some eventual optional components of the mesh that were
     * not enabled and that can be loaded from the stream/file, will be enabled
     * before loading the stream/file.
     *
     * It applies to all the loading functions.
     */
    bool enableOptionalComponents = true;

    /**
     * @brief If true, and if the Mesh has the TextureImages component, the
     * loading function will try to load the texture images from the paths
     * stored in the TextureImages component. Otherwise, only the paths of the
     * textures will be loaded.
     *
     * It applies to all the loading functions that load from a file format that
     * supports textures.
     */
    bool loadTextureImages = false;
};

/**
 * @brief The SaveSettings structure contains the settings that can be used to
 * save a mesh to a stream/file.
 */
struct SaveSettings
{
    /**
     * @brief If true, the mesh will be saved in binary format. Otherwise, it
     * will be saved in ASCII format.
     *
     * It applies to all the saving functions that save to a file format that
     * supports both binary and ASCII format.
     */
    bool binary = true;

    /**
     * @brief If true, and if the Mesh has the TextureImages component, the
     * saving function will save the texture images to the paths stored in the
     * TextureImages component. Otherwise, only the paths of the textures will
     * be saved.
     *
     * It applies to all the saving functions that save to a file format that
     * supports textures.
     */
    bool saveTextureImages = false;

    /**
     * @brief Applied only to STL binary files. Magics mode is a specific file
     * format used by the Magics software, which includes more information than
     * the standard STL format, like face colors. The magicsMode works only when
     * binary flag is set to `true`.
     */
    bool magicsMode = false;
};

} // namespace vcl

#endif // VCL_IO_MESH_SETTINGS_H
