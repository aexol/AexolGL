#ifndef FIXED_LABEL_AEX_GUI_HEADER
#define FIXED_LABEL_AEX_GUI_HEADER

#include "FontRenderer.hpp"

namespace aex {
    namespace gui {

        class AEX_PUBLIC_API FixedLabel : public FontRenderer {
        public:
            /**
             * @brief New FixedLabel component.
             *
             * @param text Text
             * @param height Font height
             * @param font Use this font to render.
             */
            FixedLabel(const std::string& text, float height, const BitmapFontGroup& font);
            virtual ~FixedLabel();
            virtual void refresh(int flags);
        protected:
            ComponentPtr setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe);
            virtual void onTextChanged();
        };
    }
}
#endif

