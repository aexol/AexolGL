#include "aex/gui/FixedLabel.hpp"
#include "aex/gui/Frame.hpp"

namespace aex
{
    namespace gui
    {

        FixedLabel::FixedLabel(const std::string& text, float height, const BitmapFontGroup& font) :
        FontRenderer(height, font)
        {
            setSize(Size(0, height));
            setText(text);
        }

        FixedLabel::~FixedLabel()
        {
        }

        void FixedLabel::refresh(int flags)
        {
            // dunno, position changing is in draw so...
        }

        ComponentPtr FixedLabel::setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe)
        {
            return nullptr;
        }

        void FixedLabel::onTextChanged()
        {
            setSize(Size(Frame::n2x(m_textString->getWidth()), getArea().s().h()));
        }
    }
}
