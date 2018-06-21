#ifndef FONT_SOURCE_BITMAP_AEX_GUI_HEADER
#define FONT_SOURCE_BITMAP_AEX_GUI_HEADER

#include "FontSource.hpp"
#include "BitmapFontSetManager.hpp"

namespace aex
{
    namespace gui
    {
        class FontSourceBitmap : public FontSource
        {
        public:
            FontSourceBitmap(const std::string& txtFilePath, const std::string& bitmapPath);
            virtual Texture* getTexture();
            virtual FontGlyphSet* getFontGlyphSet();
            virtual const FontGlyphSet* getConstFontGlyphSet() const;
        protected:
            BitmapFontSetManager::handle_t m_bfsHandle; 
            std::string m_txtFilePath;
            std::string m_bitmapPath;
        };
    }
}


#endif