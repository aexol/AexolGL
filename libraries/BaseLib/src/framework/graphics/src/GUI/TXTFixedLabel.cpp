#include "aex/gui/TXTFixedLabel.hpp"
#include "aex/gui/Frame.hpp"

namespace aex
{
    namespace gui
    {

        TXTFixedLabel::TXTFixedLabel(const std::string& text, const FontSize& fontSize, aex::unique_ptr<FontEffect> renderer) :
        TXTFixedLabel(text::s2ws(text), fontSize, std::move(renderer))
        {
        }

        TXTFixedLabel::TXTFixedLabel(const aex::wstring& text, const FontSize& fontSize, aex::unique_ptr<FontEffect> renderer) :
        TextComponent(text, fontSize, std::move(renderer))
        {
            setLineWrapping(false);
            setFitAreaToContent(true, true);
            setAligning(ALIGN::LEFT);
            setDiscardTruncated(false);
            setDiscardOuted(false);

            rebuildFullTextComponent();
        }

        TXTFixedLabel::~TXTFixedLabel()
        {
        }

        ComponentPtr TXTFixedLabel::setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe)
        {
            if (isHitted(point))
            {
                if (type == FOCUS_TYPE::TOUCH)
                {
                    focusMe(this);
                    return this;
                }
            }
            return nullptr;
        }

        void TXTFixedLabel::onTextChanged()
        {
            // nothing, fitting on refresh in TextComponent will do it
        }
    }
}
