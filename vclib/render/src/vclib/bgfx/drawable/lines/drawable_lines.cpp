#include <vclib/bgfx/drawable/lines/drawable_lines.h>
#include <vclib/bgfx/drawable/lines/lines/cpu_generated_lines.h>
#include <vclib/bgfx/drawable/lines/lines/gpu_generated_lines.h>
#include <vclib/bgfx/drawable/lines/lines/instancing_based_lines.h>
#include <vclib/bgfx/drawable/lines/lines/indirect_based_lines.h>
#include <vclib/bgfx/drawable/lines/lines/texture_based_lines.h>

#include <vclib/bgfx/context.h>


namespace vcl::lines {

    std::unique_ptr<DrawableLines> DrawableLines::create(const std::vector<LinesVertex> &points, const uint16_t width, const uint16_t heigth, LinesTypes type) {
        const bgfx::Caps* caps = bgfx::getCaps();

        switch (type) {
            case LinesTypes::CPU_GENERATED: {
                return std::make_unique<CPUGeneratedLines>(points, width, heigth);
            }

            case LinesTypes::GPU_GENERATED: {
                const bool computeSupported  = !!(caps->supported & BGFX_CAPS_COMPUTE);
                assert((void("GPU compute not supported"), computeSupported));
                return std::make_unique<GPUGeneratedLines>(points, width, heigth);
            }

            case LinesTypes::INSTANCING_BASED: {
                const bool instancingSupported = !!(caps->supported & BGFX_CAPS_INSTANCING);
                assert((void("Instancing not supported"), instancingSupported));
                return std::make_unique<InstancingBasedLines>(points, width, heigth);
            }

            case LinesTypes::INDIRECT_BASED: {
                const bool computeSupported  = !!(caps->supported & BGFX_CAPS_COMPUTE);
                const bool indirectSupported = !!(caps->supported & BGFX_CAPS_DRAW_INDIRECT);
                const bool instancingSupported = !!(caps->supported & BGFX_CAPS_INSTANCING);

                assert((void("Instancing or compute are not supported"), instancingSupported && computeSupported && indirectSupported));
                return std::make_unique<IndirectBasedLines>(points, width, heigth);
            }

            case LinesTypes::TEXTURE_BASED: {
                const bool computeSupported  = !!(caps->supported & BGFX_CAPS_COMPUTE);
                const bool indirectSupported = !!(caps->supported & BGFX_CAPS_DRAW_INDIRECT);
                const bool instancingSupported = !!(caps->supported & BGFX_CAPS_INSTANCING);
                const bool textureSupported = !!(caps->supported & BGFX_CAPS_TEXTURE_2D_ARRAY);

                assert((void("Instancing or compute or indirect or texture are not supported"), instancingSupported && computeSupported && indirectSupported && textureSupported));
                return std::make_unique<TextureBasedLines>(points, width, heigth, caps->limits.maxTextureSize);
            }
        }

        assert((void("Lines type is incorrect"), true));
        return nullptr;
    }

    std::unique_ptr<DrawableLines> DrawableLines::create(bgfx::VertexBufferHandle vbh) {
        /** ... */
        return nullptr;
    }

    std::unique_ptr<DrawableLines> DrawableLines::create(bgfx::VertexBufferHandle vbh, bgfx::IndexBufferHandle ivh) {
        /** ... */
        return nullptr;
    }

    DrawableLines::DrawableLines(const uint16_t width, const uint16_t heigth, const std::string& vs_name,  const std::string& fs_name) {
        m_Settings = LinesSettings(width, heigth);
        std::string name = "CPUGeneratedLines";
        m_Program = Context::instance().programManager().loadProgram(name, vs_name, fs_name);
    }

    DrawableLines::~DrawableLines() {
        bgfx::destroy(m_Program);
    } 
}