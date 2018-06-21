#ifndef SIMPLE_PANEL_AEX_GUI_HEADER
#define SIMPLE_PANEL_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "Panel.hpp"

namespace aex
{
    namespace gui
    {
        class AEX_PUBLIC_API SimplePanel : public Panel
        {
        public:
            virtual void refresh(int flags);
        protected:
            
        };
    }
}

#endif