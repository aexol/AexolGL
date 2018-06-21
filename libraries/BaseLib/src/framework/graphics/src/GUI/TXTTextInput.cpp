#include "aex/gui/TXTTextInput.hpp"
#include "aex/gui/ColorRectComponent.hpp"
#include "aex/gui/Frame.hpp"

namespace aex
{
    namespace gui
    {

        TXTTextInput::TXTTextInput(const std::string& onEmptyText, const FontSize& fontSize, aex::unique_ptr<FontEffect> renderer) :
        TXTTextInput(text::s2ws(onEmptyText), fontSize, std::move(renderer))
        {
        }

        TXTTextInput::TXTTextInput(const std::string& onEmptyText, const FontSize& fontSize, const Area& area, aex::unique_ptr<FontEffect> renderer) :
        TXTTextInput(text::s2ws(onEmptyText), fontSize, area, std::move(renderer))
        {
        }

        // size is setted by text wrapped area
        TXTTextInput::TXTTextInput(const aex::wstring& onEmptyText, const FontSize& fontSize, aex::unique_ptr<FontEffect> renderer) :
        TextComponent(onEmptyText, fontSize, std::move(renderer)),
        m_belka(new ColorRectComponent(Color(0, 0, 0), Area(Size(0.005f, fontSize.getNormSize())))),
        m_bBelkaVisible(false),
        m_timestamp(Frame::timestampSeconds()),
        m_belkaBlinkDuration(0.5),
        m_bInputFocused(false),
        m_doNotUpdateTrigger(false)
        {
            setOnEmptyText(onEmptyText);
            setLineWrapping(true);
            setBarColor(getColor());
            updateBelkaPosition();

            rebuildFullTextComponent();
        }

        TXTTextInput::TXTTextInput(const aex::wstring& onEmptyText, const FontSize& fontSize, const Area& area, aex::unique_ptr<FontEffect> renderer) :
        TextComponent(onEmptyText, fontSize, area, std::move(renderer)) ,
        m_belka(new ColorRectComponent(Color(0, 0, 0), Area(Size(0.005f, area.s().h())))),
        m_bBelkaVisible(false),
        m_timestamp(Frame::timestampSeconds()),
        m_belkaBlinkDuration(0.5),
        m_bInputFocused(false),
        m_doNotUpdateTrigger(false)
        {
            setOnEmptyText(onEmptyText);
            setLineWrapping(true);
            setBarColor(getColor());
            updateBelkaPosition();

            rebuildFullTextComponent();
        }

        void TXTTextInput::draw(AexRenderer * gl)
        {
            TextComponent::draw(gl);
            TextComponent::finishdraw(gl);

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

        void TXTTextInput::finishdraw(AexRenderer * gl)
        {
        }

        void TXTTextInput::needsUpdate()
        {
        }

        void TXTTextInput::refresh(int flags)
        {
            TextComponent::refresh(flags); // parent
            if (checkFlag(FLAG::SIZE_CHANGED, flags))
            {
                // TODO ??
            }

            if (checkFlag(FLAG::TEXT_CHANGED | FLAG::POSITION_CHANGED, flags)){
               updateText();
               updateBelkaPosition();
            }

            m_belka->forceRefresh();
        }

        void TXTTextInput::onTextChanged()
        {
            if (m_doNotUpdateTrigger)
            {
                m_doNotUpdateTrigger = false;
            } else
            {
                updateText();
            }

            if(m_textChangedCB)
              (*m_textChangedCB)(this);
        }

        ComponentPtr TXTTextInput::setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe)
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

        void TXTTextInput::touchDown(float x, float y, int fingerId)
        {
            if (m_bInputFocused)
            {
                int cursor = getCharNumIn(Position(x, y));
                m_textBatch.setCursor(cursor+1);
                showBar();
                updateBelkaPosition();
            }
        }

        void TXTTextInput::touchMotion(float x, float y, float dx, float dy, int fingerId)
        {
            if (m_bInputFocused)
            {
                int cursor = getCharNumIn(Position(x, y));
                m_textBatch.setCursor(cursor+1);
                showBar();
                updateBelkaPosition();
            }
        }

        void TXTTextInput::touchUp(float x, float y, int fingerId)
        {

        }

        void TXTTextInput::touchLine(float x1, float y1, float x2, float y2, int fingerId)
        {

        }

        void TXTTextInput::touchClick(float x, float y, int fingerId)
        {

        }

        void TXTTextInput::touchScale(float scale)
        {

        }

        void TXTTextInput::key_down_event(KeyboardEventData e)
        {
            m_textBatch.key_down_event(e);
            updateText();
        }

        void TXTTextInput::key_up_event(KeyboardEventData e)
        {
            m_textBatch.key_up_event(e);
            updateText();
        }

        void TXTTextInput::key_textedit_event(TextEditEventData e)
        {
            m_textBatch.key_textedit_event(e);
            updateText();
        }

        void TXTTextInput::key_textinput_event(TextInputEventData e)
        {
            m_textBatch.key_textinput_event(e);
            updateText();
        }

        void TXTTextInput::focusCatched(FOCUS_TYPE type)
        {
            if (type == FOCUS_TYPE::INPUT)
            {
                m_bInputFocused = true;
                m_textBatch.setCursor(m_textBatch.getText().size());
                m_onEmptyText2 = L"";
                updateText();
            }
        }

        void TXTTextInput::focusReleased(FOCUS_TYPE type)
        {
            if (type == FOCUS_TYPE::INPUT)
            {
                m_bInputFocused = false;
                m_onEmptyText2 = m_onEmptyText;
                updateText();
            }
        }

        void TXTTextInput::setOnEmptyText(const aex::wstring & text)
        {
            m_onEmptyText = text;
            m_onEmptyText2 = m_onEmptyText;
            updateText();
        }

        void TXTTextInput::setBarColor(const Color & color)
        {
            m_belka->setColor(color);
        }

        void TXTTextInput::setCharsLimit(unsigned maxChars)
        {
          m_textBatch.setCharsLimit(maxChars);
        }

        void TXTTextInput::clearText()
        {
          m_textBatch.setText(std::wstring());
          updateText();
        }

        void TXTTextInput::setBatchText(const aex::wstring& text)
        {
          m_textBatch.setText(text);
          updateText();
        }

        void TXTTextInput::setTextChangedCallback(const std::function<void(TXTTextInput*)>& cb)
        {
          m_textChangedCB.reset(new std::function<void(TXTTextInput*)>(cb));
        }

        void TXTTextInput::showBar()
        {
            m_timestamp = Frame::timestampSeconds();
            m_bBelkaVisible = true;
        }

        void TXTTextInput::updateBelkaPosition()
        {
            if(m_textBatch.getCursor() == 0){
                m_belka->setPosition(getInnerArea().p());
                m_belka->setSize(Size( m_belka->getArea().s().w() , getLineHeight() ));
                return;
            }

            Area charArea = getAreaOfChar(m_textBatch.getCursor()-1);
            if(m_textBatch.getText().empty())
                charArea.setPosition(getInnerArea().p());
            Position pos = Position(charArea.rightUp().x(), getLineTopAt(charArea.center()));
            m_belka->setPosition(pos);
            m_belka->setSize(Size( m_belka->getArea().s().w() , getLineHeight() ));
            setRefresh(true);
        }

        void TXTTextInput::updateText()
        {
            showBar();
            aex::wstring newText;
            if (m_textBatch.getText().empty())
            {
                newText = m_onEmptyText2;
            } else
            {
                newText = m_textBatch.getText(); // TODO CHANGE BACTCH TO LOCALES
            }
            setText(newText);
            m_doNotUpdateTrigger = true;
        }
    }
}
