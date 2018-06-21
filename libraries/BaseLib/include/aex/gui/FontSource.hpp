#ifndef FONT_SOURCE_AEX_GUI_HEADER
#define FONT_SOURCE_AEX_GUI_HEADER

#include "TextSettings.hpp"
#include "FontGlyphSet.hpp"
#include <aex/Texture.hpp>

namespace aex
{
    namespace gui
    {
        class FontSource
        {
        public:
            FontSource();
            virtual ~FontSource();

            virtual Texture* getTexture() = 0;
            virtual FontGlyphSet* getFontGlyphSet() = 0;
            virtual const FontGlyphSet* getConstFontGlyphSet() const = 0;
        protected:
        };
    }
}

#endif