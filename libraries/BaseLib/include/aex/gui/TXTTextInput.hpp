#ifndef TXTTEXT_INPUT_AEX_GUI_HEADER
#define TXTTEXT_INPUT_AEX_GUI_HEADER

#include "TextComponent.hpp"
#include "TextBatch.hpp"

namespace aex {
    namespace gui {

        class AEX_PUBLIC_API TXTTextInput : public TextComponent {
        public:
            TXTTextInput(const std::string& onEmptyText, const FontSize& fontSize, aex::unique_ptr<FontEffect> renderer);
            TXTTextInput(const std::string& onEmptyText, const FontSize& fontSize, const Area& area, aex::unique_ptr<FontEffect> renderer);

            TXTTextInput(const aex::wstring& onEmptyText, const FontSize& fontSize, aex::unique_ptr<FontEffect> renderer);
            TXTTextInput(const aex::wstring& onEmptyText, const FontSize& fontSize, const Area& area, aex::unique_ptr<FontEffect> renderer);

            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);
            virtual void needsUpdate();
            virtual void refresh(int flags);

            void setOnEmptyText(const aex::wstring& text);
            void setBarColor(const Color& color);
            void setCharsLimit(unsigned number);
            void clearText();
            void setBatchText(const aex::wstring& text);

            void setTextChangedCallback(const std::function<void(TXTTextInput*)>& cb);
        protected:
            virtual void onTextChanged();
        protected:
            virtual void touchDown(float x, float y, int fingerId);
            virtual void touchMotion(float x, float y, float dx, float dy, int fingerId);
            virtual void touchUp(float x, float y, int fingerId);
            virtual void touchLine(float x1, float y1, float x2, float y2, int fingerId);
            virtual void touchClick(float x, float y, int fingerId);
            virtual void touchScale(float scale);

            virtual void key_down_event(KeyboardEventData keyEvent);
            virtual void key_up_event(KeyboardEventData keyEvent);
            virtual void key_textedit_event(TextEditEventData textEditEvent);
            virtual void key_textinput_event(TextInputEventData textInputEvent);

            virtual void focusCatched(FOCUS_TYPE type);
            virtual void focusReleased(FOCUS_TYPE type);
            ComponentPtr setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe);
        protected:
            void showBar();
            void updateBelkaPosition();
            void updateText();

            TextBatch m_textBatch;
            ColorRectComponentUPtr m_belka;
            bool m_bBelkaVisible;
            double m_timestamp;
            float m_belkaBlinkDuration;
            bool m_bInputFocused;
            aex::wstring m_onEmptyText;
            aex::wstring m_onEmptyText2;
            bool m_doNotUpdateTrigger;
            std::unique_ptr<std::function<void(TXTTextInput*)>> m_textChangedCB;
        };
    }
}

#endif
