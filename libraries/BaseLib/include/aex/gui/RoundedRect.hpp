#ifndef ROUNDED_RECT_AEX_GUI_HEADER
#define ROUNDED_RECT_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "TriangleBatch.hpp"
#include "Component.hpp"
#include "RoundedRectMesh.hpp"

namespace aex {
    namespace gui {

        class AEX_PUBLIC_API RoundedRect : public Component {
        public:
            RoundedRect(const Area& area, const Color& color, float circleRadius);

            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);
            virtual void needsUpdate();
            void setColor(const Color& color);
            const Color& getColor() const;
            void setCircleRadius(float circleRadius);
            float getCircleRadius() const;
            void setBlurrPercent(float percent0_to_1);
            float getBlurrPercent() const;
            virtual void refresh(int flags);
        protected:
            virtual ComponentPtr setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe);
        protected:
            TriangleBatch m_batch;
            std::unique_ptr<RoundedRectMesh> m_mesh;
        };
    }
}

#endif
