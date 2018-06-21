#ifndef STRETCH_PANEL_AEX_GUI_HEADER
#define STRETCH_PANEL_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "Panel.hpp"

namespace aex {
    namespace gui {

        class AEX_PUBLIC_API StretchPanel : public Panel {
        public:

            enum ORIENTATION {
                LEFT_RIGHT,
                TOP_BOTTOM,
                CENTER
            };
        public:
            StretchPanel(const Area& area);
            virtual ~StretchPanel();
            virtual void addComponent(ComponentPtr component);
            virtual void removeComponent(ComponentPtr component);
            virtual void addComponent(ComponentPtr component, const ORIENTATION orientation);
            virtual void refresh(int flags);
        protected:

        protected:
            std::map<ComponentPtr, StretchPanel::ORIENTATION> m_orientations;
            ComponentPtr m_lastLeft;
            ComponentPtr m_lastTop;
        };
    }
}

#endif