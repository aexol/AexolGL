#ifndef SIDE_PANEL_AEX_GUI_HEADER
#define SIDE_PANEL_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "Panel.hpp"

namespace aex {
    namespace gui {

        class AEX_PUBLIC_API SidePanel : public Panel {
        public:

            enum SIDE {
                TOP,
                BOTTOM,
                RIGHT,
                LEFT
            };
        public:
            SidePanel();
            SidePanel(Area area);
            virtual ~SidePanel();
            virtual void addComponent(ComponentPtr component);
            virtual void removeComponent(ComponentPtr component);
            virtual void addComponent(ComponentPtr component, SIDE side);
            virtual void refresh(int flags);

        protected:
            ComponentPtr m_lastTop;
            ComponentPtr m_lastBottom;
            ComponentPtr m_lastRight;
            ComponentPtr m_lastLeft;
            std::map<ComponentPtr, SIDE> m_sides;
        };
    }
}

#endif