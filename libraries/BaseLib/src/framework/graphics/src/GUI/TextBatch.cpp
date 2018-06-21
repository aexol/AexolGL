#include "aex/gui/TextBatch.hpp"
#include <sstream>
#include "aex/gui/TextSettings.hpp"
#include <climits>

namespace aex
{
    namespace gui
    {

        TextBatch::TextBatch() :
        m_cursor(0),
        m_charsLimit(UINT_MAX)
        {
        }

        TextBatch::~TextBatch()
        {
        }

        void TextBatch::key_down_event(KeyboardEventData e)
        {
            if (e.scancode == AKEY_BACKSPACE)
            {
                backspace();
            }
            if (e.scancode == AKEY_RETURN)
            {
                LOG_DEBUG("Return clicked");
            }
            //LOUG(SDL_GetScancodeName(e->key.keysym.scancode));
            //LOUG(SDL_GetKeyName(e.scancode));
        }

        void TextBatch::key_up_event(KeyboardEventData e)
        {
        }

        void TextBatch::key_textedit_event(TextEditEventData e)
        {
        }

        void TextBatch::key_textinput_event(TextInputEventData e)
        {
            if(m_content.size() < m_charsLimit)
            {
              const std::wstring& wstr  = text::utf8_to_wstring(std::string(e.text) );
              addText(wstr);
            }
        }

        void TextBatch::addText(const aex::wstring& wstr)
        {
            m_content.insert(m_cursor, wstr.c_str());
            m_cursor += wstr.size();
        }

        void TextBatch::setCursor(int position)
        {
            if (position < 0)
                position = 0;
            if (position > (int) m_content.size())
                position = m_content.size();

            m_cursor = position;
        }

        int TextBatch::getCursor()
        {
            return m_cursor;
        }

        void TextBatch::backspace()
        {
            if (m_cursor > 0)
            {
                m_content.erase(m_cursor - 1, 1);
                m_cursor--;
            }
        }

        void TextBatch::clear()
        {
            m_content = L"";
            m_cursor = 0;
        }

        void TextBatch::setText(const std::wstring& text)
        {
            m_content = text;
            setCursor(text.size());
        }

        const std::wstring& TextBatch::getText() const
        {
            return m_content;
        }

        void TextBatch::setCharsLimit(unsigned number)
        {
          m_charsLimit = number;
        }
    }
}
