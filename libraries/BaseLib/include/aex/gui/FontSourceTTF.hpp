#ifndef FONT_SOURCE_TTF_AEX_GUI_HEADER
#define FONT_SOURCE_TTF_AEX_GUI_HEADER

#include "FontSource.hpp"
#include "TrueTypeFontSetManager.hpp"

namespace aex
{
    namespace gui
    {
        class FontSourceTTF : public FontSource
        {
        public:
            FontSourceTTF(const std::string& fontPath, const FontSize& fontSize, const FontSettings& settings = FontSettings());
            virtual Texture* getTexture();
            virtual FontGlyphSet* getFontGlyphSet();
            virtual const FontGlyphSet* getConstFontGlyphSet() const;
        protected:
            FontSettings m_settings;
            TrueTypeFontSetManager::handle_t m_ttfsHandle;
            FontSize m_fontSize; 
        };
    }
}


#endif