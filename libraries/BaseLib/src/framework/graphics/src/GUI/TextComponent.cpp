#include "aex/gui/TextComponent.hpp"


namespace aex
{
    namespace gui
    {

        TextComponent::TextComponent(const aex::wstring& text, const FontSize& fontSize, aex::unique_ptr<FontEffect> renderer) :
        TextComponent::TextComponent(text, fontSize, Area(), std::move(renderer))
        {
            setFitAreaToContent(true, true);
            setArea(getTextWrapSize());
            m_textSettings.color = m_renderer->getColor();
        }

        TextComponent::TextComponent(const aex::wstring& text, const FontSize& fontSize, const Area& area, aex::unique_ptr<FontEffect> renderer) :
        Component(area),
        m_renderer(std::move(renderer))
        {

            setFitAreaToContent(false, false);
            m_textSettings.align = ALIGN::LEFT;
            m_textSettings.area = area;
            m_textSettings.fontSize = fontSize;
            m_textSettings.lineWrap = false;
            m_textSettings.text = text;
            m_textSettings.withKerning = false;
            m_textSettings.discardTruncated = true;
            m_textSettings.discardOuted = true;
            m_textSettings.apllyXOffsetAtWordBeginning = true;
            m_textSettings.trimLeft = false;
            m_textSettings.trimRight = false;
            m_textSettings.color = m_renderer->getColor();
        }

        void TextComponent::draw(AexRenderer* gl)
        {
            m_renderer->draw(gl);
        }

        void TextComponent::finishdraw(AexRenderer* gl)
        {
            m_renderer->finishdraw(gl);
        }

        void TextComponent::needsUpdate()
        {
            m_renderer->needsUpdate();
        }

        void TextComponent::setText(const aex::wstring& text)
        {
            if (text != getText())
            {
                m_textSettings.text = text;
                this->setFlag(flags::FLAG::TEXT_CHANGED);
                setRefresh(true);
            }
        }

        void TextComponent::setText(const std::string& text)
        {
            setText(text::s2ws(text));
        }

        void TextComponent::setFontSize(const FontSize& fontSize)
        {
            if (m_textSettings.fontSize != fontSize)
            {
                m_textSettings.fontSize = fontSize;
                this->setFlag(flags::FLAG::FONT_SIZE_CHANGED);
                setRefresh(true);
            }
        }

        void TextComponent::setColor(const Color& color)
        {
            if(m_textSettings.color != color )
            {
                m_textSettings.color = color;
                this->setFlag(flags::FLAG::COLOR_CHANGED);
                setRefresh(true);
            }
        }

        void TextComponent::setKerning(bool bKern)
        {
            if (isKerning() != bKern)
            {
                m_textSettings.withKerning = bKern;
                this->setFlag(flags::FLAG::KERNING_CHANGED);
                setRefresh(true);
            }
        }

        void TextComponent::setLineWrapping(bool wrap)
        {
            if (isLineWrapping() != wrap)
            {
                m_textSettings.lineWrap = wrap;
                this->setFlag(flags::FLAG::LINE_WRAP_CHANGED);
                setRefresh(true);
            }
        }

        void TextComponent::setAligning(ALIGN align)
        {
            if (m_textSettings.align != align)
            {
                m_textSettings.align = align;
                this->setFlag(flags::FLAG::ALIGNING_CHANGED);
                setRefresh(true);
            }
        }

        void TextComponent::setDiscardTruncated(bool discard)
        {
            if (m_textSettings.discardTruncated != discard)
            {
                m_textSettings.discardTruncated = discard;
                this->setFlag(flags::FLAG::DISCARD_TRUNCATED_CHANGED);
                setRefresh(true);
            }
        }

        void TextComponent::setDiscardOuted(bool discard)
        {
            if (m_textSettings.discardOuted != discard)
            {
                m_textSettings.discardOuted = discard;
                this->setFlag(flags::FLAG::DISCARD_OUTED_CHANGED);
                setRefresh(true);
            }
        }

        void TextComponent::setApplyXOffsetAtWordBeginning(bool apply)
        {
            if(m_textSettings.apllyXOffsetAtWordBeginning != apply)
            {
                m_textSettings.apllyXOffsetAtWordBeginning = apply;
                this->setFlag(flags::FLAG::REBUILD_TEXT);
                setRefresh(true);
            }
        }

        void TextComponent::setTrimLeft(bool trim)
        {
            if(m_textSettings.trimLeft != trim)
            {
                m_textSettings.trimLeft = trim;
                this->setFlag(flags::FLAG::REBUILD_TEXT);
                setRefresh(true);
            }
        }

        void TextComponent::setTrimRight(bool trim)
        {
            if(m_textSettings.trimRight != trim)
            {
                m_textSettings.trimRight = trim;
                this->setFlag(flags::FLAG::REBUILD_TEXT);
                setRefresh(true);
            }
        }

        void TextComponent::setTracking(const FontSize& trackingSize)
        {
            m_textSettings.tracking = trackingSize;
        }

        const aex::wstring& TextComponent::getText() const
        {
            return m_textSettings.text;
        }

        std::string TextComponent::getTextSTDStr() const
        {
            return text::ws2s(getText());
        }

        FontSize TextComponent::getFontSize() const
        {
            return m_textSettings.fontSize;
        }

        Color TextComponent::getColor() const
        {
          return m_textSettings.color;
        }

        bool TextComponent::isKerning() const
        {
            return m_textSettings.withKerning;
        }

        bool TextComponent::isLineWrapping() const
        {
            return m_textSettings.lineWrap;
        }

        ALIGN TextComponent::getAligning() const
        {
            return m_textSettings.align;
        }

        bool TextComponent::getDiscardTruncated() const
        {
            return m_textSettings.discardTruncated;
        }

        bool TextComponent::getDiscardOuted() const
        {
            return m_textSettings.discardOuted;
        }

        bool TextComponent::isApplyXOffsetAtWordBeginning() const
        {
            return m_textSettings.apllyXOffsetAtWordBeginning;
        }

        bool TextComponent::isTrimLeft() const
        {
            return m_textSettings.trimLeft;
        }

        bool TextComponent::isTrimRight() const
        {
            return m_textSettings.trimRight;
        }

        void TextComponent::setFitAreaToContent(bool xSizeFit, bool ySizeFit)
        {
            if ((m_fitAreaX != xSizeFit) || (m_fitAreaY != ySizeFit))
            {
                m_fitAreaX = xSizeFit;
                m_fitAreaY = ySizeFit;
                setFlag(FLAG::FIT_TO_CONTENT_CHANGED);
                setRefresh(true);
            }
        }

        bool TextComponent::isFittingAreaX() const
        {
            return m_fitAreaX;
        }

        bool TextComponent::isFittingAreaY() const
        {
            return m_fitAreaY;
        }

        void TextComponent::refresh(int flags)
        {
            m_textSettings.area = getInnerArea();
            m_renderer->refresh(m_textSettings, flags);

            if (m_fitAreaX || m_fitAreaY)
                if (checkFlag(FLAG::FONT_SIZE_CHANGED | FLAG::TEXT_CHANGED | FLAG::FIT_TO_CONTENT_CHANGED | FLAG::REBUILD_TEXT, flags))
                {
                    Size wrapSize = getTextWrapSize();
                    Area tmpArea = getArea();
                    Size newSize = tmpArea.s();

                    if (m_fitAreaX)
                        newSize.setW(wrapSize.w());
                    if (m_fitAreaY)
                        newSize.setH(wrapSize.h());

                    setSize(newSize);
                }

            if (flags::checkFlag(FLAG::TEXT_CHANGED, flags))
                onTextChanged();

            if(flags::checkFlag(FLAG::COLOR_CHANGED, flags))
                m_renderer->setColor(m_textSettings.color);
        }

        void TextComponent::rebuildFullTextComponent()
        {
            int flag = 0xFFFFFFFF;
            setFlag(flag);
            forceRefresh();
            //m_renderer->refresh(m_textSettings, flag); // in this method invokes rebuildBatch method
        }

        Size TextComponent::getTextWrapSize() const
        {
            return m_renderer->getTextWrapSize();
        }

        Area TextComponent::getAreaOfChar(int id) const
        {
            return m_renderer->getAreaOfChar(id);
        }

        int TextComponent::getCharNumIn(const Position& pos) const
        {
            return m_renderer->getCharNumIn(pos);
        }

        float TextComponent::getLineHeight() const
        {
            return m_renderer->getLineHeight();
        }

        float TextComponent::getLineTopAt(const Position& pos) const
        {
            return m_renderer->getLineTopAt(pos);
        }
    }
}
