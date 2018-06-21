#ifndef ROUNDED_RECT_MESH_AEX_GUI_HEADER
#define ROUNDED_RECT_MESH_AEX_GUI_HEADER

#include "Mesh.hpp"
#include "Color.hpp"
#include "ShaderManager.hpp"

namespace aex {
    namespace gui {

        class AEX_PUBLIC_API RoundedRectMesh : public Mesh {
        public:
            static const char* ShaderName;
            static const char* Attribs;
            static const char* VSH;
            static const char* FSH;

            RoundedRectMesh(const Color& color, const TriangleBatch& batch, const Area& area, float circleRadius);
            RoundedRectMesh(RoundedRectMesh& cm) = delete;
            RoundedRectMesh(RoundedRectMesh&& cm) = delete;
            virtual ~RoundedRectMesh();

            void setColor(const Color& color);
            const Color& getColor() const;
            void setCircleRadius(float circleRadius);
            float getCircleRadius() const;
            void setAreaForUniforms(const Area& area);
            const Area& getAreaForUniforms() const;
            void setBlurrPercent(float percent0_to_1);
            float getBlurrPercent() const;

            virtual void batchChanged();

            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);
            virtual void needsUpdate();

        protected:
            void calcAndSetUniforms();

        protected:
            Color m_color;
            UniformFloat4 m_uColor;

            Area m_area;
            UniformFloat2 m_uCenter;

            float m_circleRadius;
            UniformFloat m_uCircleRadius;
            UniformFloat m_uMaxDistFromCenter;
            UniformFloat2 m_uCenter2CircCenterDelta;
            UniformFloat2 m_uCenter2CornerDelta;

            float m_blurrPercent;
            UniformFloat m_uBlurrPercent;

            ShaderManager::handle_t m_shaderHandle;
            ShaderDrw* m_shader;
        };
    }
}

#endif
