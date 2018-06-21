#ifndef TEXT_AREA_AEX_GUI_HEADER
#define TEXT_AREA_AEX_GUI_HEADER

#include "TextComponent.hpp"

namespace aex
{
    namespace gui
    {
        class TXTTextArea : public TextComponent
        {
        public:
            TXTTextArea(const std::string& text, const FontSize& fontSize, const Area& area, aex::unique_ptr<FontEffect> renderer);
            TXTTextArea(const aex::wstring& text, const FontSize& fontSize, const Area& area, aex::unique_ptr<FontEffect> renderer);
            virtual ~TXTTextArea();
        protected:
            ComponentPtr setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe);
            virtual void onTextChanged();
        };
    }
}


#endif