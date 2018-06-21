#include "aex/gui/TextInput.hpp"
#include "aex/gui/ColorRectComponent.hpp"
#include "aex/gui/Frame.hpp"
#include "aex/gui/TextSettings.hpp"

namespace aex
{
    namespace gui
    {
        TextInput::TextInput(const Area& area, const BitmapFontGroup& fontGroup, std::string onEmptyText) :
        FontRenderer(area.s().h(), fontGroup),
        m_belka(new ColorRectComponent(Color(0, 0, 0), Area(Size(0.005f, area.s().h())))),
        m_bBelkaVisible(false),
        m_timestamp(Frame::timestampSeconds()),
        m_belkaBlinkDuration(0.5),
        m_bInputFocused(false)
        {
            m_maxChars = 9999999; // Deprecated component so <-
            setArea(area);
            setOnEmptyText(onEmptyText);
        }

        void TextInput::draw(AexRenderer* gl)
        {
            FontRenderer::draw(gl);
            FontRenderer::finishdraw(gl);

            if (m_bInputFocused)
            {
                double tmp = Frame::timestampSeconds();
                if ((tmp - m_timestamp) > m_belkaBlinkDuration)
                {
                    m_bBelkaVisible = !m_bBelkaVisible;
                    m_timestamp = tmp;
                }
                if (m_bBelkaVisible)
                {
                    m_belka->draw(gl);
                    m_belka->finishdraw(gl);
                }
            }
        }

        void TextInput::finishdraw(AexRenderer* gl)
        {
        }

        void TextInput::needsUpdate()
        {
        }

        void TextInput::refresh(int flags)
        {
            Position belkaPos = getInnerArea().p();
            if (m_textString)
            {
                // m_textRef->setHudOffsetXY(Frame::x2N(getInnerArea().p().x()), Frame::y2N(getInnerArea().p().y()) );
                float movement = m_textRef->getPositionForCursor(m_textString, Frame::getWidthPixels(), Frame::getHeightPixels(), m_textBatch.getCursor());
                belkaPos.move(movement, 0.0f);
            }
            m_belka->setPosition(belkaPos);
            m_belka->forceRefresh();
        }

        void TextInput::onTextChanged()
        {
            m_textBatch.setText( text::s2ws(m_string) );
            updateText();
        }

        ComponentPtr TextInput::setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe)
        {
            if (isHitted(point))
            {
                if (type == FOCUS_TYPE::INPUT)
                {
                    focusMe(this);
                    return this;
                }
                if (type == FOCUS_TYPE::TOUCH)
                {
                    focusMe(this);
                    return this;
                }
            }

            return nullptr;
        }

        void TextInput::touchDown(float x, float y, int fingerId)
        {
            if (m_bInputFocused)
            {
                float inX = x - getInnerArea().p().x();
                int cursor = m_textRef->getCursorForPosition(m_textString, Frame::getWidthPixels(), Frame::getHeightPixels(), inX);
                m_textBatch.setCursor(cursor);
                showBelka();
                updateBelkaPosition();
            }
        }

        void TextInput::touchMotion(float x, float y, float dx, float dy, int fingerId)
        {
            if (m_bInputFocused)
            {
                float inX = x - getInnerArea().p().x();
                int cursor = m_textRef->getCursorForPosition(m_textString, Frame::getWidthPixels(), Frame::getHeightPixels(), inX);
                m_textBatch.setCursor(cursor);
                showBelka();
                updateBelkaPosition();
            }
        }

        void TextInput::touchUp(float x, float y, int fingerId)
        {

        }

        void TextInput::touchLine(float x1, float y1, float x2, float y2, int fingerId)
        {

        }

        void TextInput::touchClick(float x, float y, int fingerId)
        {

        }

        void TextInput::touchScale(float scale)
        {

        }

        void TextInput::key_down_event(KeyboardEventData e)
        {
            m_textBatch.key_down_event(e);
            updateText();
        }

        void TextInput::key_up_event(KeyboardEventData e)
        {
            m_textBatch.key_up_event(e);
            updateText();
        }

        void TextInput::key_textedit_event(TextEditEventData e)
        {
            m_textBatch.key_textedit_event(e);
            updateText();
        }

        void TextInput::key_textinput_event(TextInputEventData e)
        {
            if(m_textBatch.getText().size() >= m_maxChars)
                return;

            m_textBatch.key_textinput_event(e);
            updateText();
        }

        void TextInput::focusCatched(FOCUS_TYPE type)
        {
            if (type == FOCUS_TYPE::INPUT)
            {
                m_bInputFocused = true;
                m_textBatch.setCursor(m_textBatch.getText().size());
                m_onEmptyText2 = "";
                updateText();
            }
        }

        void TextInput::focusReleased(FOCUS_TYPE type)
        {
            if (type == FOCUS_TYPE::INPUT)
            {
                m_bInputFocused = false;
                m_onEmptyText2 = m_onEmptyText;
                updateText();
            }
        }

        void TextInput::setOnEmptyText(const std::string& text)
        {
            m_onEmptyText = text;
            m_onEmptyText2 = m_onEmptyText;
            updateText();
        }

        void TextInput::setBelkaColor(const Color& color)
        {
            m_belka->setColor(color);
        }

        std::string TextInput::getText() const
        {
            return text::ws2s(m_textBatch.getText());
        }

        void TextInput::setMaxChars(int max)
        {
            m_maxChars = max;
        }

        void TextInput::showBelka()
        {
            m_timestamp = Frame::timestampSeconds();
            m_bBelkaVisible = true;
        }

        void TextInput::updateBelkaPosition()
        {
            Position belkaPos = getInnerArea().p();
            float movement = m_textRef->getPositionForCursor(m_textString, Frame::getWidthPixels(), Frame::getHeightPixels(), m_textBatch.getCursor());
            belkaPos.move(movement, 0.0f);
            m_belka->setPosition(belkaPos);
            setRefresh(true);
        }

        void TextInput::updateText()
        {
            showBelka();
            std::string newText;
            if (m_textBatch.getText().empty())
            {
                newText = m_onEmptyText2;
            } else
            {
                newText = text::ws2s(m_textBatch.getText());
            }
            setText(newText, false);
            updateBelkaPosition();
        }
    }
}
