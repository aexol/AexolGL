#ifndef BOUNDING_RECT_AEX_GUI_HEADER
#define BOUNDING_RECT_AEX_GUI_HEADER

#include "Component.hpp"
#include "GUIMath.hpp"
#include "Color.hpp"
#include "ColorRectComponent.hpp"

namespace aex
{
    namespace gui
    {
        /**
         * @brief Draw colored border.
         */
        class AEX_PUBLIC_API BoundingRect : public Component {
        public:
            /**
             * @brief Create new bounding rect.
             *
             * @param color Border color
             * @param area Rectangle area.
             * @param thickness Broder thickness.
             */
            BoundingRect(const Color& color, const Area& area, float thickness);
            virtual void draw(aex::AexRenderer* gl);
            virtual void finishdraw(aex::AexRenderer* gl);
            virtual void needsUpdate();
            virtual void refresh(int flags);
        protected:
            ComponentPtr setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe);
             
            Color m_color;
            float m_thickness;
            ColorRectComponentUPtr m_left;
            ColorRectComponentUPtr m_top;
            ColorRectComponentUPtr m_right;
            ColorRectComponentUPtr m_bottom;
        };
    }
}

#endif
