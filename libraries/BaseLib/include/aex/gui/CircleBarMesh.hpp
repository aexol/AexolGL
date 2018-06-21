#ifndef CIRCLE_BAR_MESH_AEX_GUI_HEADER
#define CIRCLE_BAR_MESH_AEX_GUI_HEADER

#include "Mesh.hpp"
#include "Color.hpp"
#include <aex/AexDrawable.hpp>
#include <aex/VertexData.hpp>
#include "ShaderManager.hpp"

namespace aex {
    namespace gui {

        /**
         * @brief Circle progress bar effect.
         *
         * @details Class rendering circle progress bar efect.
         */
        class AEX_PUBLIC_API CircleBarMesh : public Mesh {
        public:

            /**
             * @brief New CricleBarMesh
             *
             * @param area Location for circle progress bar.
             * @param color1 Foreground color.
             * @param color2 Background color.
             * @param thickness border thickness.
             * @param percent Current percent.
             * @param startPercent Efect start percent.
             */
            CircleBarMesh(const Area& area, const Color& color1, const Color& color2, float thickness, float percent, float startPercent = 0);
            CircleBarMesh(CircleBarMesh& cbm) = delete;
            CircleBarMesh(CircleBarMesh&& cbm) = delete;
            virtual ~CircleBarMesh();

            /**
             * @brief Set foreground color for this effect.
             *
             * @param color Foreground color.
             */
            void setBarColor(const Color& color);

            /**
             * @brief Set background color for this effect.
             *
             * @param color Background color.
             */
            void setBackgroundColor(const Color& color);

            /**
             * @brief Small radius.
             *
             * @details All pixels whose distance to bar center
             * is less then small radius will be opaque.
             *
             * @param smallR Radius
             */
            void setSmallR(float smallR);

            /**
             * @brief Big radius.
             *
             * @details All pixels whose distance to bar center
             * is more then small radius but less than big radius
             * will have alpha channel interpolated. Those above 
             * big radius will be the same as background color.
             *
             * @param bigR Radius
             */
            void setBigR(float bigR);

            /**
             * @brief Change position and size of progress bar.
             *
             * @param area New area.
             */
            void setArea(const Area& area);

            /**
             * @brief Set start percent.
             *
             * @param p Start percent.
             */
            void setStartPercent(float p);

            /**
             * @brief Current progress in percents.
             *
             * @param p Current progres.
             */
            void setPercent(float p);

            /**
             * @brief Set blur effect on last percent.
             *
             * @param percent0_to_1 Blurr value.
             */
            void setBlurrPercent(float percent0_to_1);

            /**
             * @brief Return blur.
             *
             * @return Blur.
             */
            float getBlurrPercent() const;

            virtual void batchChanged();
            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);
            virtual void needsUpdate();

        protected:
            Area m_area;
            Color m_barColor;
            Color m_backgroundColor;
            float m_thickness;
            float m_smallR;
            float m_bigR;
            float m_percent;
            float m_startPercent;
            float m_blurrPercent;
            // ogl data
            UniformFloat4 m_uBarColor;
            UniformFloat4 m_uBackColor;
            UniformFloat2 m_uSmallBigR;
            UniformFloat2 m_uCenter;
            UniformFloat2 m_uPercents;
            UniformFloat  m_uBlurrPercent;

            ShaderManager::handle_t m_shaderHandle;
            ShaderDrw* m_shader;
        };
    }
}

#endif
