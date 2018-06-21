#ifndef TRUETYPE_FONT_SET_AEX_GUI_HEADER
#define TRUETYPE_FONT_SET_AEX_GUI_HEADER

#include "FontGlyphSet.hpp"
#include <aex/Font.hpp>
#include <aex/GPUApi.hpp>

namespace aex
{
    namespace gui
    {
        class TrueTypeFontSet  : public FontGlyphSet, public IOnContextDirty
        {
        public:
            TrueTypeFontSet(const aex::string& fontPath, int fontSizePix, const aex::gui::FontSettings& settings);
            virtual ~TrueTypeFontSet();

            virtual bool canResizeCharset() const;
            virtual Texture* addToCharset(const aex::wstring& chars);
            virtual aex::wstring getCharset() const;

        protected:
            void regen(const aex::wstring& newChars); // if size == 0, force rebuild
            void mapGlyphsToFontGlyphSet();
            static Font::FontSettings mapSettings(const aex::gui::FontSettings& settings);
            void onContextDirty();
        protected:
            const aex::string& m_fontPath;
            FontSettings m_fontSettings;
            Font m_font;
            FontMap m_fontMap;
            aex::wstring m_charset;
            bool m_contextInvalid;
        };
    }
}

#endif
