#ifndef DEBUG_RECT_AEX_GUI_HEADER
#define DEBUG_RECT_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "Component.hpp"

namespace aex
{
    namespace gui
    {
        class AEX_PUBLIC_API DebugRect : public Component
        {
        public:
            DebugRect(const Area& area);
            virtual ~DebugRect();
            virtual void refresh(int flags) = 0;
            virtual void draw(AexRenderer* gl) = 0;
            virtual void finishdraw(AexRenderer* gl) = 0;
            virtual void needsUpdate() = 0;
        protected:
        };
    }
}

#endif