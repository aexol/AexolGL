#include "aex/gui/FontSourceBitmap.hpp"

namespace aex
{
    namespace gui
    {

        FontSourceBitmap::FontSourceBitmap(const std::string& txtFilePath, const std::string& bitmapPath) :
        m_bfsHandle(BitmapFontSetManager::get(txtFilePath, bitmapPath)),
        m_txtFilePath(txtFilePath),
        m_bitmapPath(bitmapPath)
        {
        }

        Texture* FontSourceBitmap::getTexture()
        {
            return m_bfsHandle->getTexture();
        }

        FontGlyphSet* FontSourceBitmap::getFontGlyphSet()
        {
            return m_bfsHandle.get();
        }
        
        const FontGlyphSet* FontSourceBitmap::getConstFontGlyphSet() const
        {
            return m_bfsHandle.get();
        }
    }
}