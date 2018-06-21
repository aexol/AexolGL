#ifndef COLOR_RECT_COMPONENT_AEX_GUI_HEADER
#define COLOR_RECT_COMPONENT_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "TriangleBatch.hpp"
#include "ColorMesh.hpp"
#include "Component.hpp"

namespace aex {
    namespace gui {

        /**
         * @brief Colored rectangle.
         */
        class AEX_PUBLIC_API ColorRectComponent : public Component {
        public:
            /**
             * @brief New ColorRectComponent
             *
             * @param area Component position.
             * @param color Component color.
             */
            ColorRectComponent(const Area& area, const Color& color);

            /**
             * @brief New ColorRectComponent
             *
             * @param color Component color.
             * @param area Component position.
             */
            ColorRectComponent(const Color& color, const Area& area);

            /**
             * @brief Change component color.
             *
             * @param color New color.
             */
            void setColor(const Color& color);

            /**
             * @brief Get component color.
             *
             * @return Color.
             */
            const Color& getColor() const;

            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);
            virtual void needsUpdate();
        protected:
            virtual void refresh(int flags);
            virtual ComponentPtr setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe);
        protected:
            TriangleBatch m_batch;
            std::unique_ptr<ColorMesh> m_mesh;
        };
    }
}

#endif
