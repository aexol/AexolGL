#ifndef ROOT_PANEL_AEX_GUI_HEADER
#define ROOT_PANEL_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "SimplePanel.hpp"

namespace aex {
    namespace gui {

        class AEX_PUBLIC_API RootPanel : public SimplePanel {
        public:
            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);
        protected:
            bool m_prevDepth;
        };
    }
}

#endif