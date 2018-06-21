#ifndef TXTFIXED_LABEL_AEX_GUI_HEADER
#define TXTFIXED_LABEL_AEX_GUI_HEADER

#include "TextComponent.hpp"

namespace aex {
    namespace gui {

        class AEX_PUBLIC_API TXTFixedLabel : public TextComponent {
        public:
            TXTFixedLabel(const std::string& text, const FontSize& fontSize, aex::unique_ptr<FontEffect> renderer);
            TXTFixedLabel(const aex::wstring& text, const FontSize& fontSize, aex::unique_ptr<FontEffect> renderer);
            virtual ~TXTFixedLabel();
        protected:
            ComponentPtr setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe);
            virtual void onTextChanged();
        };
    }
}
#endif
