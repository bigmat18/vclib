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

#include <vclib/bgfx/primitives/lines/texture_instancing_lines.h>

namespace vcl::detail {

TextureInstancingLines::TextureInstancingLines()
{
    checkCaps();

    mVertices.create(
        VERTICES.data(),
        VERTICES.size(),
        bgfx::Attrib::Position,
        2,
        PrimitiveType::FLOAT);

    mIndices.create(INDICES.data(), INDICES.size());
}

TextureInstancingLines::TextureInstancingLines(
    const std::vector<float>& vertCoords,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>& lineColors) : TextureInstancingLines()
{
    setPoints(vertCoords, vertColors, vertNormals, lineColors);
}

TextureInstancingLines::TextureInstancingLines(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  lineIndices,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors) : TextureInstancingLines()
{
    setPoints(vertCoords, lineIndices, vertNormals, vertColors, lineColors);
}

TextureInstancingLines::TextureInstancingLines(
    const uint nVertices, 
    const VertexBuffer& vertCoords,
    const VertexBuffer& vertNormals,
    const VertexBuffer& vertColors,
    const VertexBuffer& lineColors) : TextureInstancingLines()
{
    setPoints(nVertices, vertCoords, vertColors, vertNormals, lineColors);
}

TextureInstancingLines::TextureInstancingLines(
    const uint nVertices,
    const VertexBuffer& vertCoords,
    const IndexBuffer&  lineIndices,
    const VertexBuffer& vertNormals,
    const VertexBuffer& vertColors,
    const VertexBuffer& lineColors) : TextureInstancingLines()
{
    setPoints(nVertices, vertCoords, lineIndices, vertNormals, vertColors, lineColors);
}

void TextureInstancingLines::swap(TextureInstancingLines& other)
{
    using std::swap;

    swap(mVertices, other.mVertices);
    swap(mIndices, other.mIndices);

    swap(mVertexCoords, other.mVertexCoords);
    swap(mVertexColors, other.mVertexColors);
    swap(mVertexNormals, other.mVertexNormals);
    swap(mLineColors, other.mLineColors);
    swap(mLineIndices, other.mLineIndices);

    swap(mTexture, other.mTexture);
    swap(mNumPoints, other.mNumPoints);

    swap(mCustomIndicesUH, other.mCustomIndicesUH);
    swap(mTextureSizeUH, other.mTextureSizeUH);
}

void TextureInstancingLines::draw(uint viewId) const
{
    mVertices.bind(0);
    mIndices.bind();
    mTexture.bind(0, bgfx::Access::Read);

    bgfx::setInstanceCount(mNumPoints / 2);
    bgfx::setState(linesDrawState());
    bgfx::submit(viewId, mLinesPH);
}

void TextureInstancingLines::setPoints(    
    const std::vector<float>& vertCoords,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>& lineColors)
{
    std::vector<uint> emptyIndices;
    setPoints(vertCoords, emptyIndices, vertNormals, vertColors, lineColors);
}

void TextureInstancingLines::setPoints(    
    const std::vector<float>& vertCoords,
    const std::vector<uint>& lineIndices,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors)
{
    mNumPoints = static_cast<uint>(vertCoords.size() / 3);
    if (mNumPoints > 1) {
        const bool setNormals = !vertNormals.empty();
        const bool setVertColors = !vertColors.empty();
        const bool setLineColors = !lineColors.empty();

        allocateVertexCoords(vertCoords);
        if (!lineIndices.empty())
            allocateLineIndices(lineIndices);
        if (setNormals)
            allocateVertexNormals(vertNormals);
        if (setVertColors)
            allocateVertexColors(vertColors);
        if (setLineColors)
            allocateVertexLineColors(lineColors);

        allocateTextureBuffer();
        generateTextureBuffer(
            mVertexCoords, mLineIndices, mVertexNormals, mVertexColors, mLineColors
        );
    }
    else {
        mVertexCoords.destroy();
        mVertexColors.destroy();
        mVertexNormals.destroy();
        mLineColors.destroy();
        mLineIndices.destroy();

        mTexture.destroy();
        mNumPoints = 0;
    }
}

void TextureInstancingLines::setPoints(
    const uint pointsSize,
    const VertexBuffer& vertexCoords,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors,
    const VertexBuffer& lineColors)
{
    IndexBuffer emptyIndices;
    setPoints(pointsSize, vertexCoords, emptyIndices, vertexNormals, vertexColors, lineColors);
}

void TextureInstancingLines::setPoints(
    const uint pointsSize,
    const VertexBuffer& vertexCoords,
    const IndexBuffer&  lineIndices,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors,
    const VertexBuffer& lineColors)
{
    mNumPoints = pointsSize;
    if (mNumPoints > 1) {

        allocateTextureBuffer();
        generateTextureBuffer(
            vertexCoords, lineIndices, vertexNormals, vertexColors, lineColors
        );
    }
    else {
        mVertexCoords.destroy();
        mVertexColors.destroy();
        mVertexNormals.destroy();
        mLineColors.destroy();
        mLineIndices.destroy();

        mTexture.destroy();
        mNumPoints = 0;
    }
}

void TextureInstancingLines::allocateVertexCoords(const std::vector<float>& vertCoords)
{
    auto [buffer, releaseFn] =
        linesGetAllocatedBufferAndReleaseFn<float>(vertCoords.size());

    std::copy(vertCoords.begin(), vertCoords.end(), buffer);

    mVertexCoords.createForCompute(
        buffer,
        vertCoords.size() / 3,
        bgfx::Attrib::Position,
        3,
        PrimitiveType::FLOAT,
        false,
        bgfx::Access::Read,
        releaseFn
    );
}

void TextureInstancingLines::allocateLineIndices(const std::vector<uint>& lineIndices) 
{
    auto [buffer, releaseFn] =
        linesGetAllocatedBufferAndReleaseFn<uint>(lineIndices.size());

    mLineIndices.createForCompute(
        buffer,
        lineIndices.size(),
        PrimitiveType::UINT,
        bgfx::Access::Read,
        releaseFn
    );
}
    
void TextureInstancingLines::allocateVertexNormals(const std::vector<float>& vertNormals)
{
    auto [buffer, releaseFn] =
        linesGetAllocatedBufferAndReleaseFn<float>(vertNormals.size());

    std::copy(vertNormals.begin(), vertNormals.end(), buffer);

    mVertexNormals.createForCompute(
        buffer,
        vertNormals.size() / 3,
        bgfx::Attrib::Normal,
        3,
        PrimitiveType::FLOAT,
        false,
        bgfx::Access::Read,
        releaseFn
    );
}
    
void TextureInstancingLines::allocateVertexColors(const std::vector<uint>& vertColors) 
{
    auto [buffer, releaseFn] =
        linesGetAllocatedBufferAndReleaseFn<uint>(vertColors.size());

    std::copy(vertColors.begin(), vertColors.end(), buffer);

    mVertexColors.createForCompute(
        buffer,
        vertColors.size(),
        bgfx::Attrib::Color0,
        4,
        PrimitiveType::UCHAR,
        true,
        bgfx::Access::Read,
        releaseFn
    );
}
    
void TextureInstancingLines::allocateVertexLineColors(const std::vector<uint>& lineColors) 
{
    auto [buffer, releaseFn] =
        linesGetAllocatedBufferAndReleaseFn<uint>(lineColors.size());

    std::copy(lineColors.begin(), lineColors.end(), buffer);

    mLineColors.createForCompute(
        buffer,
        lineColors.size(),
        bgfx::Attrib::Color1,
        4,
        PrimitiveType::UCHAR,
        true,
        bgfx::Access::Read,
        releaseFn
    );
}

void TextureInstancingLines::allocateTextureBuffer()
{
    uint16_t Y = (mNumPoints * 3) / (MAX_TEXTURE_SIZE + 1);
    uint16_t X = Y == 0 ? (mNumPoints * 3) : MAX_TEXTURE_SIZE;

    mTexture.create(
        X, Y + 1, bgfx::TextureFormat::RGBA32F, BGFX_TEXTURE_COMPUTE_WRITE);
}

void TextureInstancingLines::generateTextureBuffer(
    const VertexBuffer& vertexCoords,
    const IndexBuffer&  lineIndices,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors,
    const VertexBuffer& lineColors)
{
    float data[] = {static_cast<float>(lineIndices.isValid()), 
                   static_cast<float>(vertexNormals.isValid()), 
                   static_cast<float>(vertexColors.isValid()), 
                   static_cast<float>(lineColors.isValid())};

    mCustomIndicesUH.bind(data);
    
    float dataSize[] = {
        static_cast<float>(MAX_TEXTURE_SIZE),
        0.0f, 0.0f, 0.0f
    };
    mTextureSizeUH.bind(dataSize);
    
    vertexCoords.bind(0);

    if (lineIndices.isValid())
        lineIndices.bind(1);

    if (vertexNormals.isValid())
        vertexNormals.bind(2);

    if (vertexColors.isValid())
        vertexColors.bind(3);

    if (lineColors.isValid())
        lineColors.bind(4);

    mTexture.bind(5, bgfx::Access::Write);
    bgfx::dispatch(0, mComputeTexturePH, (mNumPoints / 2), 1, 1);
}

} // namespace vcl::detail
