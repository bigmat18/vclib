#pragma once
#include <vclib/bgfx/drawable/lines/drawable_polylines.h>

namespace vcl::lines {
    class CPUGeneratedPolylines : public DrawablePolylines {

        bgfx::ProgramHandle mLinesPH = Context::instance().programManager().getProgram(
                                            VclProgram::POLYLINES_CPU_GENERATED_VSFS);

        uint32_t                    mPointsSize;
        std::vector<float>          mVertices;
        std::vector<uint32_t>       mSegmentsIndexes;
        std::vector<uint32_t>       mJoinsIndexes;

        bgfx::DynamicVertexBufferHandle    mVerticesBH          = BGFX_INVALID_HANDLE;
        bgfx::DynamicIndexBufferHandle     mSegmentsIndexesBH   = BGFX_INVALID_HANDLE;
        bgfx::DynamicIndexBufferHandle     mJoinsIndexesBH      = BGFX_INVALID_HANDLE;

        public:
            CPUGeneratedPolylines() = default;

            CPUGeneratedPolylines(const std::vector<LinesVertex> &points);

            CPUGeneratedPolylines(const CPUGeneratedPolylines& other);

            CPUGeneratedPolylines(CPUGeneratedPolylines&& other);

            ~CPUGeneratedPolylines();

            CPUGeneratedPolylines& operator=(CPUGeneratedPolylines other);

            void swap(CPUGeneratedPolylines& other);

            std::shared_ptr<vcl::DrawableObject> clone() const override;

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:

            void generateBuffers(const std::vector<LinesVertex> points);

            void allocateVertexBuffer();

            void allocateIndexesBuffer();
    };
}