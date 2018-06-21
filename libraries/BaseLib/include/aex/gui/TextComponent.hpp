#ifndef TEXT_COMPONENT_AEX_GUI_HEADER
#define TEXT_COMPONENT_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "FontEffect.hpp"
#include "Component.hpp"
#include "TextSettings.hpp"
#include "Flags.hpp"

namespace aex {
    namespace gui {

        class AEX_PUBLIC_API TextComponent : public Component {
        public:
            TextComponent(const aex::wstring& text, const FontSize& fontSize, aex::unique_ptr<FontEffect> renderer);
            TextComponent(const aex::wstring& text, const FontSize& fontSize, const Area& area, aex::unique_ptr<FontEffect> renderer);
            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);
            virtual void needsUpdate();

            void setText(const aex::wstring& text);
            void setText(const std::string& text);
            void setFontSize(const FontSize& fontSize);
            void setColor(const Color& color);
            void setKerning(bool bKern);
            void setLineWrapping(bool bWrap);
            void setAligning(ALIGN align);
            void setDiscardTruncated(bool discard);
            void setDiscardOuted(bool discard);
            void setApplyXOffsetAtWordBeginning(bool apply);
            void setTrimLeft(bool trim);
            void setTrimRight(bool trim);
            void setTracking(const FontSize& trackingSize);

            const aex::wstring& getText() const;
            std::string getTextSTDStr() const;
            FontSize getFontSize() const;
            Color getColor() const;
            bool isKerning() const;
            bool isLineWrapping() const;
            ALIGN getAligning() const;
            bool getDiscardTruncated() const;
            bool getDiscardOuted() const;
            bool isApplyXOffsetAtWordBeginning() const;
            bool isTrimLeft() const;
            bool isTrimRight() const;

            void setFitAreaToContent(bool xSizeFit, bool ySizeFit);
            bool isFittingAreaX() const;
            bool isFittingAreaY() const;

            // base interfejs for managing backend
            Size getTextWrapSize() const;
            Area getAreaOfChar(int num) const;
            int getCharNumIn(const Position& pos) const;
            float getLineHeight() const;
            float getLineTopAt(const Position& pos) const;
        protected:
            virtual void refresh(int flags);
            virtual void onTextChanged() = 0;
            void rebuildFullTextComponent();

        private:
            aex::unique_ptr<FontEffect> m_renderer;
            TextSettings m_textSettings;
            bool m_fitAreaX;
            bool m_fitAreaY;
        };
    }
}


#endif
