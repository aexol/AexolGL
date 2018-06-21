#include "aex/gui/TXTTextArea.hpp"

namespace aex
{
    namespace gui
    {

        TXTTextArea::TXTTextArea(const std::string& text, const FontSize& fontSize, const Area& area, aex::unique_ptr<FontEffect> renderer) :
        TXTTextArea(text::s2ws(text), fontSize, area, std::move(renderer))
        {
        }

        TXTTextArea::TXTTextArea(const aex::wstring& text, const FontSize& fontSize, const Area& area, aex::unique_ptr<FontEffect> renderer) :
        TextComponent(text, fontSize, area, std::move(renderer))
        {
            setLineWrapping(true);
            setFitAreaToContent(false, true);
            setAligning(ALIGN::LEFT);
            setDiscardTruncated(false);
            setKerning(false);
            setDiscardOuted(false);

            rebuildFullTextComponent();
        }

        TXTTextArea::~TXTTextArea()
        {
        }

        ComponentPtr TXTTextArea::setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe)
        {
            if (type != FOCUS_TYPE::TOUCH)
                return nullptr;

            if (isHitted(point))
            {
                focusMe(this);
                return this;
            }

            return nullptr;
        }

        void TXTTextArea::onTextChanged()
        {
        }
    }
}
