#pragma once
#include <vclib/render/interfaces/drawable_object_i.h>
#include <vclib/bgfx/drawable/lines/lines_settings.h>
#include <bgfx/bgfx.h>

namespace vcl::lines {
    class DrawablePolylines : public vcl::DrawableObjectI {

        public:
            static std::unique_ptr<DrawablePolylines> create(const std::vector<LinesVertex> &points, const uint16_t width, const uint16_t heigth, LinesTypes type = LinesTypes::CPU_GENERATED);

            static std::unique_ptr<DrawablePolylines> create(bgfx::VertexBufferHandle vbh);

            static std::unique_ptr<DrawablePolylines> create(bgfx::VertexBufferHandle vbh, bgfx::IndexBufferHandle ivh);

            DrawablePolylines(const uint16_t width, const uint16_t heigth, const std::string& vs_name,  const std::string& fs_name);

            virtual ~DrawablePolylines();

            vcl::Box3d boundingBox() const override { 
                return vcl::Box3d(vcl::Point3d(-1,-1,-1), vcl::Point3d(1, 1, 1));
            }

            bool isVisible() const override { return m_Visible; }

            void setVisibility(bool vis) override { m_Visible = vis; }

            LinesSettings& getSettings() { return m_Settings; }

            virtual void update(const std::vector<LinesVertex> &points) = 0;

        protected: 
            bgfx::ProgramHandle m_Program;
            
            LinesSettings m_Settings;
            vcl::Box3d m_BoundingBox;
            bool m_Visible = true;
    };
}