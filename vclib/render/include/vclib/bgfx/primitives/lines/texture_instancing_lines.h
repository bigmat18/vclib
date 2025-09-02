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

#ifndef VCL_BGFX_PRIMITIVES_LINES_TEXTURE_INSTANCING_LINES_H
#define VCL_BGFX_PRIMITIVES_LINES_TEXTURE_INSTANCING_LINES_H

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/context.h>

#include <vclib/bgfx/uniform.h>

#include <vclib/bgfx/primitives/lines/lines_utils.h>

namespace vcl::detail {

class TextureInstancingLines
{
    static inline const std::vector<float> VERTICES =
        {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};
    static inline const std::vector<uint> INDICES = {0, 1, 2, 1, 3, 2};

    uint MAX_TEXTURE_SIZE = bgfx::getCaps()->limits.maxTextureSize;

    bgfx::ProgramHandle mComputeTexturePH =
        Context::instance()
            .programManager()
            .getComputeProgram<ComputeProgram::CUSTOM_TEXTURE_INSTANCING_LINES>();
    bgfx::ProgramHandle mLinesPH =
        Context::instance()
            .programManager()
            .getProgram<VertFragProgram::CUSTOM_TEXTURE_INSTANCING_LINES>();

    Uniform mCustomIndicesUH = Uniform("u_ActiveBuffers", bgfx::UniformType::Vec4);
    Uniform mTextureSizeUH = Uniform("u_TextureSize", bgfx::UniformType::Vec4);

    VertexBuffer mVertices;
    IndexBuffer  mIndices;

    VertexBuffer mVertexCoords;
    VertexBuffer mVertexColors;
    VertexBuffer mVertexNormals;
    VertexBuffer mLineColors;   
    IndexBuffer  mLineIndices;
    
    TextureBuffer mTexture;
    uint mNumPoints = 0;

public:
    TextureInstancingLines();

    TextureInstancingLines(
        const std::vector<float>& vertCoords,
        const std::vector<float>& vertNormals = std::vector<float>(),
        const std::vector<uint>&  vertColors = std::vector<uint>(),
        const std::vector<uint>&  lineColors = std::vector<uint>());

    TextureInstancingLines(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  lineIndices,
        const std::vector<float>& vertNormals = std::vector<float>(),
        const std::vector<uint>&  vertColors = std::vector<uint>(),
        const std::vector<uint>&  lineColors = std::vector<uint>());

    TextureInstancingLines(
        const uint pointsSize,
        const VertexBuffer& vertexCoords,
        const VertexBuffer& vertexNormals = VertexBuffer(),
        const VertexBuffer& vertexColors = VertexBuffer(),
        const VertexBuffer& lineColors = VertexBuffer());

    TextureInstancingLines(
        const uint pointsSize,
        const VertexBuffer& vertexCoords,
        const IndexBuffer&  lineIndices,
        const VertexBuffer& vertexNormals = VertexBuffer(),
        const VertexBuffer& vertexColors = VertexBuffer(),
        const VertexBuffer& lineColors = VertexBuffer());

    void swap(TextureInstancingLines& other);

    friend void swap(TextureInstancingLines& a, TextureInstancingLines& b) { a.swap(b); }

    void setPoints(
        const std::vector<float>& vertCoords,
        const std::vector<float>& vertNormals = std::vector<float>(),
        const std::vector<uint>&  vertColors = std::vector<uint>(),
        const std::vector<uint>&  lineColors = std::vector<uint>());
    
    void setPoints(
        const std::vector<float>& vertCoords,
        const std::vector<uint>&  lineIndices,
        const std::vector<float>& vertNormals = std::vector<float>(),
        const std::vector<uint>&  vertColors = std::vector<uint>(),
        const std::vector<uint>&  lineColors = std::vector<uint>());

    void setPoints(
        const uint pointsSize,
        const VertexBuffer& vertexCoords,
        const VertexBuffer& vertexNormals = VertexBuffer(),
        const VertexBuffer& vertexColors = VertexBuffer(),
        const VertexBuffer& lineColors = VertexBuffer());

    void setPoints(
        const uint pointsSize,
        const VertexBuffer& vertexCoords,
        const IndexBuffer&  lineIndices,
        const VertexBuffer& vertexNormals = VertexBuffer(),
        const VertexBuffer& vertexColors = VertexBuffer(),
        const VertexBuffer& lineColors = VertexBuffer());

    void draw(uint viewId) const;

private:
    void checkCaps() const
    {
        const bgfx::Caps* caps = bgfx::getCaps();
        const bool computeSupported = bool(caps->supported & BGFX_CAPS_COMPUTE);
        const bool indirectSupported =
            bool(caps->supported & BGFX_CAPS_DRAW_INDIRECT);
        const bool instancingSupported =
            bool(caps->supported & BGFX_CAPS_INSTANCING);
        const bool textureSupported =
            bool(caps->supported & BGFX_CAPS_TEXTURE_2D_ARRAY);

        if (!(instancingSupported && computeSupported && indirectSupported &&
              textureSupported)) {
            throw std::runtime_error(
                "Instancing or compute or indirect or texture are not "
                "supported");
        }
    }

    void allocateVertexCoords(const std::vector<float>& vertCoords);

    void allocateLineIndices(const std::vector<uint>& lineIndices);
    
    void allocateVertexNormals(const std::vector<float>& vertNormals);
    
    void allocateVertexColors(const std::vector<uint>& vertColors);
    
    void allocateVertexLineColors(const std::vector<uint>& lineColors);
    
    void allocateVertexAndIndexBuffer();

    void allocateTextureBuffer();

    void generateTextureBuffer(
        const VertexBuffer& vertexCoords,
        const IndexBuffer&  lineIndices,
        const VertexBuffer& vertexNormals,
        const VertexBuffer& vertexColors,
        const VertexBuffer& lineColors);
};

} // namespace vcl::detail

#endif // VCL_BGFX_PRIMITIVES_LINES_TEXTURE_INSTANCING_LINES_H
