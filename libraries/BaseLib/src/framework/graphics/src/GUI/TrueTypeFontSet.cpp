#include "aex/gui/TrueTypeFontSet.hpp"
#include <aex/TextureManager.hpp>
#include <chrono>
namespace aex
{
    namespace gui
    {
        TrueTypeFontSet::TrueTypeFontSet(const aex::string& fontPath, int fontSizePix, const FontSettings& settings) :
        m_fontPath(fontPath),
        m_fontSettings(settings),
        m_font(fontPath, fontSizePix, mapSettings(settings)),
        m_charset(L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!№;%:?*()_+-=.,/|\"\\'@#$^&{}[]0123456789 "),
        m_contextInvalid(false)
        {
            using namespace std::chrono;
            m_font.requestMapUpdate(m_charset);
            m_fontMap = m_font.fontMap();

            // LOG_DEBUG("Generated in: ",  (timeNow2 - timeNow)/1000.0f, " s" );

            m_fontInfo.size = fontSizePix;
            m_texture = TextureManager::GetInstance().getEmptyTexture(m_fontMap.backingTexture());
            mapGlyphsToFontGlyphSet();
        }

        TrueTypeFontSet::~TrueTypeFontSet()
        {
            TextureManager::GetInstance().releaseTexture(m_fontMap.backingTexture());
        }

        bool TrueTypeFontSet::canResizeCharset() const
        {
            return true;
        }

        Texture* TrueTypeFontSet::addToCharset(const aex::wstring& chars)
        {
            bool requestUpdate = false;
            for(size_t i = 0 ; i < chars.size() ; i++)
            {
                wchar_t next = chars.at(i);
                if( std::find(m_charset.begin(), m_charset.end(), next) == m_charset.end() )
                {
                    requestUpdate = true;
                    m_charset += next;
                }
            }

            if(requestUpdate)
              regen(chars);

            return m_texture;
        }

        aex::wstring TrueTypeFontSet::getCharset() const
        {
            return m_charset;
        }

        void TrueTypeFontSet::regen(const aex::wstring& newChars)
        {
          m_regenNum++;
          TextureManager::GetInstance().releaseTexture(m_fontMap.backingTexture());

          if(newChars.size() == 0){
            m_font.setContextDirtyFlag(true);
            LOG_DEBUG("------------ SETTING CONTEXT DIRTY FLAG IN FONT  ------------------");
          }

          m_font.requestMapUpdate(newChars);
          m_fontMap = m_font.fontMap();
          m_texture = TextureManager::GetInstance().getEmptyTexture(m_fontMap.backingTexture());
          mapGlyphsToFontGlyphSet();
        }

        void TrueTypeFontSet::mapGlyphsToFontGlyphSet()
        {
            m_fontInfo.bold = false;
            m_fontInfo.italic = false;
            m_fontInfo.lineHeight = m_fontMap.lineSkip();
            m_fontInfo.base = m_fontMap.ascent(); // TODO check it corectness
            m_fontInfo.textureWidth = m_fontMap.mapSize().x();
            m_fontInfo.textureHeight = m_fontMap.mapSize().y();
            m_fontInfo.base = m_fontMap.ascent();

            // fill glyphs
            for (size_t i = 0; i < m_charset.size(); i++)
            {
                wchar_t nextChar = m_charset.at(i);

                const GlyphChar* ttGlyph = m_fontMap.getGlyph(nextChar);
                if (!ttGlyph)
                {
                    LOG_DEBUG("CANT GET: ", nextChar);
                    continue;
                }

                Glyph nextGlyph;
                nextGlyph.x = ttGlyph->rectOnTexture[0][0];
                nextGlyph.y = ttGlyph->rectOnTexture[0][1];
                nextGlyph.width = ttGlyph->rectOnTexture[1][0];
                nextGlyph.height = ttGlyph->rectOnTexture[1][1];
                nextGlyph.advance = ttGlyph->m_metrics.advance;
                nextGlyph.c = nextChar;
                nextGlyph.chnl = 15; // dunno what is it
                nextGlyph.page = 0;
                nextGlyph.xoffset = ttGlyph->m_metrics.m_minx;
                nextGlyph.yoffset = m_fontMap.ascent() - ttGlyph->m_metrics.m_maxy;
                //nextGlyph.yoffset = -m_fontMap.descent() + ttGlyph->m_metrics.m_miny;
                m_glyphs[nextGlyph.c] = nextGlyph;
            }

            // build kerning map:
            for (size_t i = 0; i < m_charset.size(); i++)
            {
                for (size_t j = 0; j < m_charset.size(); j++)
                {
                    wchar_t firstChar = m_charset.at(i);
                    wchar_t secondChar = m_charset.at(j);

                    KerningPair kernPair;
                    kernPair.first = firstChar;
                    kernPair.second = secondChar;

                    int32_t kern = m_font.kernDistance(firstChar, secondChar);
                    if (kern != 0)
                    {
                        m_kernings[kernPair] = kern;
                    }
                }
            }
        }

        Font::FontSettings TrueTypeFontSet::mapSettings(const aex::gui::FontSettings& settings)
        {
            Font::FontSettings retSettings;
            retSettings.genKernMap = true;

            switch (settings.channels)
            {
                case aex::gui::FONT_TEX_CHANNELS::RGBA:
                    retSettings.channels = TTF_FONT_TEX_CHANNELS::RGBA;
                    break;
                case aex::gui::FONT_TEX_CHANNELS::ALPHA:
                default:
                    retSettings.channels = TTF_FONT_TEX_CHANNELS::ALPHA;
                    break;
            }

            switch (settings.smooth)
            {
                case FONT_SMOOTH::SOLID:
                    retSettings.glyphShape = Font::GLYPH_SHAPE::SOLID;
                    break;

                case FONT_SMOOTH::BLENDED:
                    retSettings.glyphShape = Font::GLYPH_SHAPE::BLENDED;
                    break;

                default:
                case FONT_SMOOTH::SHADED:
                    retSettings.glyphShape = Font::GLYPH_SHAPE::SHADED;
                    break;
            }

            return retSettings;
        }

        void TrueTypeFontSet::onContextDirty(){
          regen(std::wstring()); // length == 0 so context dirty flag will be setted
        }
    }
}
