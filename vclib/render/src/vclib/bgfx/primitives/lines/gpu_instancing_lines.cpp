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

 #include <vclib/bgfx/primitives/lines/gpu_instancing_lines.h>

namespace vcl::detail {

GPUInstancingLines::GPUInstancingLines()
{
    checkCaps();
    assert(bgfx::isValid(mComputeInstancingPH));

    mVertices.create(
        VERTICES.data(),
        VERTICES.size(),
        bgfx::Attrib::Position,
        2,
        PrimitiveType::FLOAT);

    mIndices.create(INDICES.data(), INDICES.size());
}

GPUInstancingLines::GPUInstancingLines(
    const std::vector<float>& vertCoords,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>& lineColors) : GPUInstancingLines()
{
    setPoints(vertCoords, vertColors, vertNormals, lineColors);
}

GPUInstancingLines::GPUInstancingLines(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  lineIndices,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors) : GPUInstancingLines()
{
    setPoints(vertCoords, lineIndices, vertNormals, vertColors, lineColors);
}

GPUInstancingLines::GPUInstancingLines(
    const uint pointsSize,
    const VertexBuffer& vertexCoords,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors,
    const VertexBuffer& lineColors) : GPUInstancingLines()
{
    setPoints(pointsSize, vertexCoords, vertexNormals, vertexColors, lineColors);
}

GPUInstancingLines::GPUInstancingLines(
    const uint pointsSize,
    const VertexBuffer& vertexCoords,
    const IndexBuffer&  lineIndices,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors,
    const VertexBuffer& lineColors) : GPUInstancingLines()
{
    setPoints(pointsSize, vertexCoords, lineIndices, vertexNormals, vertexColors, lineColors);
}

void GPUInstancingLines::swap(GPUInstancingLines& other)
{
    using std::swap;
    swap(mNumPoints, other.mNumPoints);
    swap(mVertices, other.mVertices);
    swap(mIndices, other.mIndices);

    swap(mVertexCoords, other.mVertexCoords);
    swap(mVertexColors, other.mVertexColors);
    swap(mVertexNormals, other.mVertexNormals);
    swap(mLineColors, other.mLineColors);
    
    swap(mInstanceData, other.mInstanceData);
    swap(mCustomIndicesUH, other.mCustomIndicesUH);
}

void GPUInstancingLines::draw(uint viewId) const
{
    mVertices.bind(0);
    mIndices.bind();

    mInstanceData.setInstance(0, mNumPoints / 2);

    bgfx::setState(linesDrawState());
    bgfx::submit(viewId, mLinesPH);
}

void GPUInstancingLines::setPoints(    
    const std::vector<float>& vertCoords,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>& lineColors)
{
    std::vector<uint> emptyIndices;
    setPoints(vertCoords, emptyIndices, vertNormals, vertColors, lineColors);
}

void GPUInstancingLines::setPoints(    
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  lineIndices,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors)
{
    assert(vertCoords.size() % 3 == 0);
    assert(lineIndices.size() % 2 == 0);

    const bool setLineIndices = lineIndices.size() != 0;
    const bool setColors      = vertColors.size() != 0;
    const bool setNormals     = vertNormals.size() != 0;
    const bool setLineColors  = lineColors.size() != 0;

    mNumPoints =
        setLineIndices ? lineIndices.size() : vertCoords.size() / 3;

    assert(!setColors || vertCoords.size() == vertColors.size() * 3);
    assert(!setNormals || vertCoords.size() == vertNormals.size());
    assert(!setLineColors || vertColors.size() == lineColors.size() * 2);

    if (mNumPoints > 1) {

        allocateVertexCoords(vertCoords);
        
        if (setLineIndices)
            allocateLineIndices(lineIndices);
        if (setNormals)
            allocateVertexNormals(vertNormals);
        if (setColors)
            allocateVertexColors(vertColors);
        if (setLineColors)
            allocateVertexLineColors(lineColors);

        allocateInstanceDataBuffer();
        generateInstanceDataBuffer(
            mVertexCoords, mLineIndices, mVertexNormals, mVertexColors, mLineColors
        );
    }
    else {
        mVertexCoords.destroy();
        mVertexColors.destroy();
        mVertexNormals.destroy();
        mLineColors.destroy();

        mInstanceData.destroy();
        mNumPoints = 0;
    }
}

void GPUInstancingLines::setPoints(
    const uint pointsSize,
    const VertexBuffer& vertexCoords,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors,
    const VertexBuffer& lineColors)
{
    IndexBuffer emptyIndices;
    setPoints(pointsSize, vertexCoords, emptyIndices, vertexNormals, vertexColors, lineColors);
}

void GPUInstancingLines::setPoints(
    const uint pointsSize,
    const VertexBuffer& vertexCoords,
    const IndexBuffer&  lineIndices,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors,
    const VertexBuffer& lineColors)
{
    mNumPoints = pointsSize;
    if (mNumPoints > 1) {

        allocateInstanceDataBuffer();
        generateInstanceDataBuffer(
            mVertexCoords, mLineIndices, mVertexNormals, mVertexColors, mLineColors
        );
    }
    else {
        mVertexCoords.destroy();
        mVertexColors.destroy();
        mVertexNormals.destroy();
        mLineColors.destroy();
        mLineIndices.destroy();

        mVertices.destroy();
        mIndices.destroy();
    }
}

void GPUInstancingLines::allocateVertexCoords(const std::vector<float>& vertCoords)
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

void GPUInstancingLines::allocateLineIndices(const std::vector<uint>& lineIndices) 
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
    
void GPUInstancingLines::allocateVertexNormals(const std::vector<float>& vertNormals)
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
    
void GPUInstancingLines::allocateVertexColors(const std::vector<uint>& vertColors) 
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
    
void GPUInstancingLines::allocateVertexLineColors(const std::vector<uint>& lineColors) 
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

void GPUInstancingLines::allocateInstanceDataBuffer() 
{
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::TexCoord0, 4, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord1, 4, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord2, 4, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord3, 4, bgfx::AttribType::Float)
        .end();

    const uint sz = (mNumPoints / 2) * 16;

    mInstanceData.create(
        bgfx::makeRef(nullptr, sizeof(float) * sz),
        layout,
        BGFX_BUFFER_COMPUTE_WRITE,
        true);
}

void GPUInstancingLines::generateInstanceDataBuffer(
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
    vertexCoords.bind(0);

    if (lineIndices.isValid())
        lineIndices.bind(1);

    if (vertexNormals.isValid())
        vertexNormals.bind(2);

    if (vertexColors.isValid())
        vertexColors.bind(3);

    if (lineColors.isValid())
        lineColors.bind(4);

    mInstanceData.bind(5, bgfx::Access::Write);

    bgfx::dispatch(0, mComputeInstancingPH, (mNumPoints / 2), 1, 1);
}

} // namespace vcl::detail
