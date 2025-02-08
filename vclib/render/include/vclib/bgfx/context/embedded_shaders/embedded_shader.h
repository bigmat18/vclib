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

#ifndef VCL_BGFX_CONTEXT_EMBEDDED_SHADERS_EMBEDDED_SHADER_H
#define VCL_BGFX_CONTEXT_EMBEDDED_SHADERS_EMBEDDED_SHADER_H

#include <bgfx/embedded_shader.h>

#include <array>
#include <string>

namespace vcl {

struct VclProgram
{
    enum Enum {
        DRAWABLE_AXIS,
        DRAWABLE_DIRECTIONAL_LIGHT,
        DRAWABLE_MESH,
        DRAWABLE_TRACKBALL,

        FONT_BASIC,
        FONT_DISTANCE_FIELD,
        FONT_DISTANCE_FIELD_DROP_SHADOW,
        FONT_DISTANCE_FIELD_DROP_SHADOW_IMAGE,
        FONT_DISTANCE_FIELD_OUTLINE,
        FONT_DISTANCE_FIELD_OUTLINE_DROP_SHADOW_IMAGE,
        FONT_DISTANCE_FIELD_OUTLINE_IMAGE,
        FONT_DISTANCE_FIELD_SUBPIXEL,

        LINES_CPU_GENERATED_VSFS,
        LINES_GPU_GENERATED_CS,
        LINES_INDIRECT_BASED_CS,
        LINES_INDIRECT_BASED_VSFS,
        LINES_INSTANCING_BASED_VSFS,
        LINES_TEXTURE_BASED_CS,
        LINES_TEXTURE_BASED_VSFS,

        POLYLINES_CPU_GENERATED_VSFS,
        POLYLINES_GPU_GENERATED_CS,
        POLYLINES_INDIRECT_BASED_CS,
        POLYLINES_INDIRECT_BASED_JOINTS_VSFS,
        POLYLINES_INDIRECT_BASED_VSFS,
        POLYLINES_INSTANCING_BASED_VSFS,
        POLYLINES_INSTANCING_BASED_JOINTS_VSFS,
        POLYLINES_TEXTURE_BASED_CS,
        POLYLINES_TEXTURE_BASED_JOINTS_VSFS,
        POLYLINES_TEXTURE_BASED_VSFS,


        COUNT
    };

    static inline const std::array<std::string, VclProgram::COUNT>
        programNames = {
            "DrawableAxis",
            "DrawableDirectionalLight",
            "DrawableMesh",
            "DrawableTrackball",

            "FontBasic",
            "FontDistanceField",
            "FontDistanceFieldDropShadow",
            "FontDistanceFieldDropShadowImage",
            "FontDistanceFieldOutline",
            "FontDistanceFieldOutlineDropShadowImage",
            "FontDistanceFieldOutlineImage",
            "FontDistanceFieldSubpixel",

            "LinesCPUGenerated",
            "LinesGPUGenerated",
            "LinesIndirectBasedCS",
            "LinesIndirectBasedVSFS",
            "LinesInstancingBased",
            "LinesTextureBasedCS",
            "LinesTextureBasedVSFS",

            "PolylinesLinesCPUGenerated",
            "PolylinesLinesGPUGenerated",
            "PolylinesLinesIndirectBasedCS",
            "PolylinesLinesIndirectBasedJoints",
            "PolylinesLinesIndirectBasedVSFS",
            "PolylinesLinesInstancingBased",
            "PolylinesLinesInstancingBasedJoints",
            "PolylinesLinesTextureBasedCS",
            "PolylinesLinesTextureBasedJoints",
            "PolylinesLinesTextureBasedVSFS"
    };
};

template<VclProgram::Enum PROGRAM = VclProgram::COUNT>
struct EmbeddedShader
{
    static bgfx::EmbeddedShader::Data vertexEmbeddedShader(
        bgfx::RendererType::Enum type)
    {
        static_assert(true, "Invalid program");
        switch (type) {
        case bgfx::RendererType::OpenGLES: return {type, nullptr, 0};
        case bgfx::RendererType::OpenGL: return {type, nullptr, 0};
        case bgfx::RendererType::Vulkan: return {type, nullptr, 0};
#ifdef _WIN32
        case bgfx::RendererType::Direct3D11: return {type, nullptr, 0};
        case bgfx::RendererType::Direct3D12: return {type, nullptr, 0};
#endif
#ifdef __APPLE__
        case bgfx::RendererType::Metal: return {type, nullptr, 0};
#endif
        default: return {type, nullptr, 0};
        }
    }

    static bgfx::EmbeddedShader::Data fragmentEmbeddedShader(
        bgfx::RendererType::Enum type)
    {
        static_assert(true, "Invalid program");
        switch (type) {
        case bgfx::RendererType::OpenGLES: return {type, nullptr, 0};
        case bgfx::RendererType::OpenGL: return {type, nullptr, 0};
        case bgfx::RendererType::Vulkan: return {type, nullptr, 0};
#ifdef _WIN32
        case bgfx::RendererType::Direct3D11: return {type, nullptr, 0};
        case bgfx::RendererType::Direct3D12: return {type, nullptr, 0};
#endif
#ifdef __APPLE__
        case bgfx::RendererType::Metal: return {type, nullptr, 0};
#endif
        default: return {type, nullptr, 0};
        }
    }

    static bgfx::EmbeddedShader::Data computeEmbeddedShader(
        bgfx::RendererType::Enum type)
    {
        static_assert(true, "Invalid program");
        switch (type) {
        case bgfx::RendererType::OpenGLES: return {type, nullptr, 0};
        case bgfx::RendererType::OpenGL: return {type, nullptr, 0};
        case bgfx::RendererType::Vulkan: return {type, nullptr, 0};
#ifdef _WIN32
        case bgfx::RendererType::Direct3D11: return {type, nullptr, 0};
        case bgfx::RendererType::Direct3D12: return {type, nullptr, 0};
#endif
#ifdef __APPLE__
        case bgfx::RendererType::Metal: return {type, nullptr, 0};
#endif
        default: return {type, nullptr, 0};
        }
    }
};

} // namespace vcl

#endif // VCL_BGFX_CONTEXT_EMBEDDED_SHADERS_EMBEDDED_SHADER_H
