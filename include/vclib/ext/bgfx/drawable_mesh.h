/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_EXT_BGFX_DRAWABLE_MESH_DRAWABLE_MESH_H
#define VCL_EXT_BGFX_DRAWABLE_MESH_DRAWABLE_MESH_H

#include <bgfx/bgfx.h>

#include <vclib/render/generic_drawable_mesh.h>
#include <vclib/render/mesh_render_buffers.h>

#include "drawable_mesh_program.h"

namespace vcl::bgf {

template<MeshConcept MeshType>
class DrawableMesh : public GenericDrawableMesh
{
    MeshRenderBuffers<MeshType> mrb;

    std::vector<uint> textID;

    bgfx::VertexBufferHandle meshVBH = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle meshIBH = BGFX_INVALID_HANDLE;

    bgfx::UniformHandle meshColorUH = BGFX_INVALID_HANDLE;

    bgfx::ProgramHandle program = BGFX_INVALID_HANDLE;

public:
    DrawableMesh()
    {
        setupUniforms();
    };

    DrawableMesh(const MeshType& mesh)
    {
        if constexpr (HasName<MeshType>) {
            name() = mesh.name();
        }
        updateBuffers(mesh);
        mrs.setDefaultSettingsFromCapability();

        setupBuffers();
        setupUniforms();
    }

    ~DrawableMesh()
    {
        destroyBuffers();
        destroyUniforms();
    }

    void updateBuffers(const MeshType& m)
    {
        if constexpr (HasName<MeshType>) {
            name() = m.name();
        }
        unbindTextures();
        mrb = MeshRenderBuffers<MeshType>(m);
        mrs.setRenderCapabilityFrom(m);
        bindTextures();

        destroyBuffers();
        setupBuffers();
    }

    void setProgram(const DrawableMeshProgram& p)
    {
        program = p.program();
    }

    // DrawableObject implementation

    void init() { bindTextures(); }

    void draw() const
    {
        if (bgfx::isValid(program)) {
            bgfx::setVertexBuffer(0, meshVBH);

            bgfx::setIndexBuffer(meshIBH);

            uint64_t state = 0
                | BGFX_STATE_WRITE_RGB
                | BGFX_STATE_WRITE_A
                | BGFX_STATE_WRITE_Z
                | BGFX_STATE_DEPTH_TEST_LESS
                | BGFX_STATE_CULL_CW
                | BGFX_STATE_MSAA;

            bgfx::setState(state);

            bgfx::setUniform(meshColorUH, mrb.meshColorBufferData());

            bgfx::submit(0, program);
        }
    }

    vcl::Point3d center() const { return (mrb.bbMin() + mrb.bbMax()) / 2; }

    double radius() const { return (mrb.bbMax() - mrb.bbMin()).norm() / 2; }

    DrawableMesh* clone() const { return new DrawableMesh(*this); }

private:
    void setupBuffers()
    {
        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .end();

        meshVBH = bgfx::createVertexBuffer(
            bgfx::makeRef(mrb.vertexBufferData(), mrb.vertexBufferSize() * sizeof(float)), layout);

        meshIBH = bgfx::createIndexBuffer(
            bgfx::makeRef(mrb.triangleBufferData(), mrb.triangleBufferSize() * sizeof(uint32_t)), BGFX_BUFFER_INDEX32);
    }

    void setupUniforms()
    {
        meshColorUH = bgfx::createUniform("meshColor", bgfx::UniformType::Vec4);
    }

    void destroyBuffers()
    {
        if (bgfx::isValid(meshVBH))
            bgfx::destroy(meshVBH);

        if (bgfx::isValid(meshIBH))
            bgfx::destroy(meshIBH);
    }

    void destroyUniforms()
    {
        if (bgfx::isValid(meshColorUH))
            bgfx::destroy(meshColorUH);
    }

    void bindTextures()
    {
        // todo
    }

    void unbindTextures()
    {
        // todo
    }
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_DRAWABLE_MESH_DRAWABLE_MESH_H
