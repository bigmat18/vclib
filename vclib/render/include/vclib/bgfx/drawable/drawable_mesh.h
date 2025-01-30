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

#ifndef VCL_BGFX_DRAWABLE_DRAWABLE_MESH_H
#define VCL_BGFX_DRAWABLE_DRAWABLE_MESH_H

#include <vclib/algorithms/mesh/stat/bounding_box.h>
#include <vclib/render/drawable/abstract_drawable_mesh.h>

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers.h>
#include <vclib/bgfx/drawable/uniforms/drawable_mesh_uniforms.h>
#include <vclib/bgfx/drawable/uniforms/mesh_render_settings_uniforms.h>

#include <bgfx/bgfx.h>

namespace vcl {

template<MeshConcept MeshType>
class DrawableMeshBGFX : public AbstractDrawableMesh, public MeshType
{
    Box3d mBoundingBox;

    MeshRenderBuffers<MeshType> mMRB;

    bgfx::ProgramHandle mProgram =
        Context::instance().programManager().getProgram(
            VclProgram::DRAWABLE_MESH);

    DrawableMeshUniforms               mMeshUniforms;
    mutable MeshRenderSettingsUniforms mMeshRenderSettingsUniforms;

public:
    using AbstractDrawableMesh::name;

    DrawableMeshBGFX() = default;

    DrawableMeshBGFX(const MeshType& mesh) :
            AbstractDrawableMesh(mesh), MeshType(mesh)
    {
        updateBuffers();
    }

    ~DrawableMeshBGFX() = default;

    void updateBuffers() override
    {
        if constexpr (HasName<MeshType>) {
            AbstractDrawableMesh::name() = MeshType::name();
        }

        bool bbToInitialize = !vcl::HasBoundingBox<MeshType>;
        if constexpr (vcl::HasBoundingBox<MeshType>) {
            if (this->MeshType::boundingBox().isNull()) {
                bbToInitialize = true;
            }
            else {
                mBoundingBox =
                    this->MeshType::boundingBox().template cast<double>();
            }
        }

        if (bbToInitialize) {
            mBoundingBox = vcl::boundingBox(*this);
        }

        mMRB = MeshRenderBuffers<MeshType>(*this);
        mMRS.setRenderCapabilityFrom(*this);
        mMRB.setWireframeSettings(mMRS);
        mMeshRenderSettingsUniforms.updateSettings(mMRS);
        mMeshUniforms.update(*this);
    }

    void swap(DrawableMeshBGFX& other)
    {
        using std::swap;
        AbstractDrawableMesh::swap(other);
        MeshType::swap(other);
        swap(mBoundingBox, other.mBoundingBox);
        swap(mMRB, other.mMRB);
        swap(mProgram, other.mProgram);
        swap(mMeshUniforms, other.mMeshUniforms);
        swap(mMeshRenderSettingsUniforms, other.mMeshRenderSettingsUniforms);
    }

    friend void swap(DrawableMeshBGFX& a, DrawableMeshBGFX& b) { a.swap(b); }

    // DrawableObject implementation

    void init() override {}

    void draw(uint viewId) const override
    {
        if (bgfx::isValid(mProgram)) {
            uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                             BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL |
                             BGFX_STATE_BLEND_NORMAL;

            if (mMRS.isSurfaceVisible()) {
                mMRB.bindTextures(); // Bind textures before vertex buffers!!
                mMRB.bindVertexBuffers(mMRS);
                mMRB.bindIndexBuffers();
                bindUniforms(VCL_MRS_DRAWING_SURFACE);

                bgfx::setState(state);

                bgfx::submit(viewId, mProgram);
            }

            if (mMRS.isWireframeVisible()) {
                mMRB.drawWireframe(viewId);
            }

            if (mMRS.isPointVisible()) {
                mMRB.bindVertexBuffers(mMRS);
                bindUniforms(VCL_MRS_DRAWING_POINTS);

                bgfx::setState(state | BGFX_STATE_PT_POINTS);

                bgfx::submit(viewId, mProgram);
            }

            if (mMRS.isEdgesVisible()) {
                mMRB.bindVertexBuffers(mMRS);
                mMRB.bindIndexBuffers(MeshBufferId::EDGES);
                bindUniforms(VCL_MRS_DRAWING_EDGES);

                bgfx::setState(state | BGFX_STATE_PT_LINES);

                bgfx::submit(viewId, mProgram);
            }
        }
    }

    Box3d boundingBox() const override { return mBoundingBox; }

    std::shared_ptr<DrawableObject> clone() const override
    {
        return std::make_shared<DrawableMeshBGFX>(*this);
    }

    void setVisibility(bool vis) override
    {
        AbstractDrawableMesh::setVisibility(vis);
        mMeshRenderSettingsUniforms.updateSettings(mMRS);
    }

    void setRenderSettings(const MeshRenderSettings& rs) override
    {
        AbstractDrawableMesh::setRenderSettings(rs);
        mMRB.setWireframeSettings(rs); // TODO check me
        mMeshRenderSettingsUniforms.updateSettings(rs);
    }

private:
    void bindUniforms(uint primitive) const
    {
        mMeshRenderSettingsUniforms.updatePrimitive(primitive);
        mMeshRenderSettingsUniforms.bind();
        mMeshUniforms.bind();
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_MESH_H
