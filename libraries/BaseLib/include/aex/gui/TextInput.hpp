#ifndef TEXT_INPUT_AEX_GUI_HEADER
#define TEXT_INPUT_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "FontRenderer.hpp"
#include "TextBatch.hpp"

namespace aex {
    namespace gui {

        class AEX_PUBLIC_API TextInput : public FontRenderer {
        public:
            TextInput(const Area& area, const BitmapFontGroup& fontGroup, std::string onEmptyText = "");
            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);
            virtual void needsUpdate();
            virtual void refresh(int flags);

            void setOnEmptyText(const std::string& text);
            void setBelkaColor(const Color& color);
            std::string getText() const;
            void setMaxChars(int max);

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
            void showBelka();
            void updateBelkaPosition();
            void updateText();

            TextBatch m_textBatch;
            ColorRectComponentUPtr m_belka;
            bool m_bBelkaVisible;
            double m_timestamp;
            float m_belkaBlinkDuration;
            bool m_bInputFocused;
            std::string m_onEmptyText;
            std::string m_onEmptyText2;
            uint m_maxChars;
        };
    }
}

#endif
