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

#include <vclib/bgfx/primitives/lines/cpu_instancing_lines.h>

#include <vclib/bgfx/primitives/lines/lines_utils.h>

namespace vcl::detail {

CPUInstancingLines::CPUInstancingLines()
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

CPUInstancingLines::CPUInstancingLines(
    const std::vector<float>& vertCoords,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>& lineColors) 
    : CPUInstancingLines()
{
    setPoints(vertCoords, vertColors, vertNormals, lineColors);
}

CPUInstancingLines::CPUInstancingLines(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  lineIndices,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors) : CPUInstancingLines()
{
    setPoints(vertCoords, lineIndices, vertNormals, vertColors, lineColors);
}

void CPUInstancingLines::swap(CPUInstancingLines& other)
{
    using std::swap;

    swap(mVertCoords, other.mVertCoords);
    swap(mVertColors, other.mVertColors);
    swap(mVertNormals, other.mVertNormals);
    swap(mLineColors, other.mLineColors);

    swap(mVertices, other.mVertices);
    swap(mIndices, other.mIndices);
    swap(mInstanceDB, other.mInstanceDB);
}

void CPUInstancingLines::draw(uint viewId) const
{
    if ((mVertCoords.size() / 3) > 1) {

        generateInstanceDataBuffer();

        mVertices.bind(0);
        mIndices.bind();
        bgfx::setInstanceDataBuffer(&mInstanceDB);

        bgfx::setState(linesDrawState());
        bgfx::submit(viewId, mLinesPH);
    }
}

void CPUInstancingLines::setPoints(
    const std::vector<float>& vertCoords,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>& lineColors)
{
    std::vector<uint> lineIndices;
    setPoints(vertCoords, lineIndices, vertNormals, vertColors, lineColors);
}

void CPUInstancingLines::setPoints(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  lineIndices,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors)
{
    mVertCoords = vertCoords;
    mLineIndices = lineIndices;
    mVertColors = vertColors;
    mVertNormals = vertNormals;
    mLineColors = lineColors;
}

void CPUInstancingLines::generateInstanceDataBuffer() const
{
    assert(mVertCoords.size() % 3 == 0);
    assert(mLineIndices.size() % 2 == 0);

    const bool setLineIndices = mLineIndices.size() != 0;
    const bool setColors      = mVertColors.size() != 0;
    const bool setNormals     = mVertNormals.size() != 0;
    const bool setLineColors  = mLineColors.size() != 0;

    const uint nPoints =
        setLineIndices ? mLineIndices.size() : mVertCoords.size() / 3;

    assert(!setColors || mVertCoords.size() == mVertColors.size() * 3);
    assert(!setNormals || mVertCoords.size() == mVertNormals.size());
    assert(!setLineColors || mVertColors.size() == mLineColors.size() * 2);

    if (nPoints <= 1)
        return;

    const uint16_t stride = sizeof(float) * 16;
    uint           size   = (nPoints / 2);

    uint linesNum = bgfx::getAvailInstanceDataBuffer(size, stride);
    bgfx::allocInstanceDataBuffer(&mInstanceDB, size, stride);

    uint8_t* data = mInstanceDB.data;
    for (uint i = 0; i < nPoints - 1; i += 2) {
        uint index0 = setLineIndices ? mLineIndices[i] : i;
        uint index1 = setLineIndices ? mLineIndices[i + 1] : i + 1;

        float* p0 = reinterpret_cast<float*>(data);
        p0[0] = mVertCoords[((index0) * 3)];
        p0[1] = mVertCoords[((index0) * 3) + 1];
        p0[2] = mVertCoords[((index0) * 3) + 2];

        uint* color0 = reinterpret_cast<uint*>(&data[12]);
        if (setColors)  color0[0]    = mVertColors[index0];
        else            color0[0]    = 0xffffffff;

        float* p1 = reinterpret_cast<float*>(&data[16]);
        p1[0] = mVertCoords[(index1 * 3)];
        p1[1] = mVertCoords[(index1 * 3) + 1];
        p1[2] = mVertCoords[(index1 * 3) + 2];

        uint* color1 = reinterpret_cast<uint*>(&data[28]);
        if (setColors)  color1[0]    = mVertColors[index1];
        else            color1[0]    = 0xffffffff;

        float* n0 = reinterpret_cast<float*>(&data[32]);
        if (setNormals) {
            n0[0] = mVertNormals[((index0) * 3)];
            n0[1] = mVertNormals[((index0) * 3) + 1];
            n0[2] = mVertNormals[((index0) * 3) + 2];
        } else {
            n0[0] = 0.0f;
            n0[1] = 0.0f;
            n0[2] = 0.0f;
        }

        uint* lineColor = reinterpret_cast<uint*>(&data[44]);
        if (setLineColors)  lineColor[0] = mLineColors[i / 2];
        else                lineColor[0] = 0xffffffff;

        float* n1 = reinterpret_cast<float*>(&data[48]);
        if (setNormals) {
            n1[0] = mVertNormals[(index1 * 3)];
            n1[1] = mVertNormals[(index1 * 3) + 1];
            n1[2] = mVertNormals[(index1 * 3) + 2];
        } else {
            n1[0] = 0.0f;
            n1[1] = 0.0f;
            n1[2] = 0.0f;
        }
        n1[3] = 0;

        data += stride;
    }
}

} // namespace vcl
