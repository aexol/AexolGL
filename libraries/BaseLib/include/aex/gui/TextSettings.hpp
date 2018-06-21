#ifndef TEXT_SETTINGS_AEX_GUI_HEADER
#define TEXT_SETTINGS_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "Frame.hpp"
#include "Color.hpp"
#include <string>
#include <locale>


namespace aex {
    namespace gui {
        enum FONT_TEX_CHANNELS {
            ALPHA = 0,
            RGBA
        };

        enum FONT_SMOOTH {
            SOLID = 0,
            SHADED,
            BLENDED,
        };

        struct FontSettings
        {
            FontSettings();
            FontSettings(FONT_TEX_CHANNELS channels, FONT_SMOOTH smooth);
            FONT_TEX_CHANNELS channels;
            FONT_SMOOTH smooth;

            friend std::ostream& operator << (std::ostream& out, const FontSettings& fntSettings);
        };

        std::ostream& operator << (std::ostream& out, const FontSettings& fntSettings);

        enum ALIGN {
            LEFT,
            RIGHT,
            CENTER,
            JUSTIFIED
        };

        class FontSize {
        public:
            FontSize(int sizePt);
            FontSize(int sizePt, const GSize2D<int>& mockupSize, SCALE_PLANE scalePlane=SCALE_PLANE::HORIZONTAL); // will scale font to norm size of mockup to current frame size
            FontSize(float normalizedSize);
            FontSize(double normalizedSize);

            virtual ~FontSize() {}

            int getSizePt() const;
            float getNormSize() const;
            GSize2D<int> getFontMockupSize() const;

            bool operator==(const FontSize& rightHand);
            bool operator!=(const FontSize& rightHand);
            bool operator<(const FontSize& rightHand);
            bool operator>(const FontSize& rightHand);
            bool operator<=(const FontSize& rightHand);
            bool operator>=(const FontSize& rightHand);

        protected:
            int m_sizePt; // in pixels
            GSize2D<int> m_mockupSize;
            float m_normSize;
            Scaler m_scaler;
        };

        struct TextSettings {
            TextSettings();
            TextSettings(const aex::wstring& text, const FontSize& fontSize, bool withKern, const Area& area, bool lineWrap, ALIGN align, bool discardTruncated, bool discardOuted);

            virtual ~TextSettings();

            aex::wstring text; // full text

            FontSize fontSize;
            bool withKerning;

            Area area; // contains start posision and if needed - size to line wrapping
            bool lineWrap; // Width getted from area.s().w()) to wrapping
            ALIGN align;
            bool discardTruncated;
            bool discardOuted;
            bool apllyXOffsetAtWordBeginning;
            bool trimLeft;
            bool trimRight;
            FontSize tracking;
            Color color;
        };

        namespace text {
            aex::wstring s2ws(const std::string& str);
            std::string ws2s(const aex::wstring& fstr);
            size_t to_narrow(const wchar_t* src, char* dest, size_t dest_len);
            char wc2c(const wchar_t& fc);
            wchar_t c2wc(const char& c);
            aex::wstring utf8_to_wstring(const std::string& utf8Str);
            aex::string wstring_to_utf8(const aex::wstring& wstr);
        }
    }
}

#endif
