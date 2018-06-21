#include "aex/gui/FontSourceTTF.hpp"

namespace aex
{
    namespace gui
    {   
        FontSourceTTF::FontSourceTTF(const std::string& fontPath, const FontSize& fontSize, const FontSettings& settings) :
        m_settings(settings),
        m_ttfsHandle(TrueTypeFontSetManager::get(fontPath, fontSize.getSizePt(), settings)),
        m_fontSize(fontSize)
        {
        }

        Texture* FontSourceTTF::getTexture()
        {
            return m_ttfsHandle->getTexture();
        }

        FontGlyphSet* FontSourceTTF::getFontGlyphSet()
        {
            return m_ttfsHandle.get();
        }
        
        const FontGlyphSet* FontSourceTTF::getConstFontGlyphSet() const
        {
            return m_ttfsHandle.get();
        }
    }
}