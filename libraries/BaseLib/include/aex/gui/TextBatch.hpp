#ifndef TEXT_BATCH_AEX_GUI_HEADER
#define TEXT_BATCH_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include <aex/KeyboardEvent.hpp>

namespace aex {
    namespace gui {

        class AEX_PUBLIC_API TextBatch {
        public:
            TextBatch();
            virtual ~TextBatch();
            void key_down_event(KeyboardEventData keyEvent);
            void key_up_event(KeyboardEventData keyEvent);
            void key_textedit_event(TextEditEventData textEditEvent);
            void key_textinput_event(TextInputEventData textInputEvent);

            void addText(const aex::wstring& wstr);
            void setCursor(int position);
            int getCursor();
            void backspace();
            void clear();
            void setText(const std::wstring& text);
            const std::wstring& getText() const;
            void setCharsLimit(unsigned number);
        private:
            std::wstring m_content;
            int m_cursor;
            unsigned m_charsLimit;
        };
    }
}

#endif
