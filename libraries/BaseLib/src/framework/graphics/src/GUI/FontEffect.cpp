#include "aex/gui/FontEffect.hpp"
#include "aex/gui/Frame.hpp"
#include <aex/TextureManager.hpp>

namespace aex
{
    namespace gui
    {
        using namespace flags;

        FontEffect::FontEffect(ShaderManager::handle_t shaderHandle,
                std::unique_ptr<FontSource>&& fntSrc,
                const Color& color) :
        m_triangleBatch(false),
        m_shaderHandle(std::move(shaderHandle)),
        m_fntSrc(std::move(fntSrc)),
        m_color(color)
        {
            m_shader = m_shaderHandle.get();
            initTexture(m_fntSrc->getTexture());
            m_fontGlyphSetRegenNum = getFontGlyphSet()->getRegenNum();
        }

        FontEffect::~FontEffect()
        {
        }

        void FontEffect::draw(AexRenderer* gl)
        {
            // if other text using same FontGlyphSet (which is singleton resource for same text params components (size, font_type etc...))
            if(getFontGlyphSet()->getRegenNum() != m_fontGlyphSetRegenNum)
            {
                m_fontGlyphSetRegenNum = getFontGlyphSet()->getRegenNum();
                initTexture(getFontGlyphSet()->getTexture());
                rebuildBatch();
            }
        }

        void FontEffect::setColor(const Color& color)
        {
            m_color = color;
            colorChanged();
        }

        Color FontEffect::getColor() const
        {
            return m_color;
        }

        Size FontEffect::getTextWrapSize() const
        {
            return m_testWrapSize;
        }

        Area FontEffect::getAreaOfChar(int num) const
        {
            const std::vector<FontGlyphSet::WordData>& words = m_words.getWordsData();

            int id = 0;
            Area retArea;
            bool end = false;
            for (size_t i = 0; i < words.size(); i++)
            {
                const std::vector<Area>& charsAreas = words[i].getCharsAreas();

                for (size_t j = 0; j < charsAreas.size(); j++)
                {
                    if (num == id)
                    {
                        retArea = charsAreas[j];
                        end = true;
                        break;
                    }
                    id++;
                }
                if (end)
                    break;
            }

            // because data are generated always in left up corner
            Position baseDelta = m_textSettings.area.leftUp();
            retArea.setPosition(retArea.leftUp() + baseDelta);
            return retArea;
        }

        int FontEffect::getCharNumIn(const Position& pos) const
        {
            const std::vector<FontGlyphSet::WordData>& words = m_words.getWordsData();
             if (words.empty())
                return 0;

            // because data are generated always in left up corner
            Position originPos = pos - m_textSettings.area.leftUp();
            Position wordsCorner = Position(0,0);

            int lineOfPos = 0;
            float lineHeight = getLineHeight();
            lineOfPos = ( originPos.y() - wordsCorner.y() ) / lineHeight;
            if(originPos.y() < wordsCorner.y())
                lineOfPos = 0;

            // if pos is under last line, select in last line
            int lastLine = (words[words.size()-1].leftUp().y() - wordsCorner.y() ) / lineHeight;
            lineOfPos = std::min(lineOfPos, lastLine);

            int retNum = 0;
            int id = 0;
            float delta = std::numeric_limits<float>::max();
            for (size_t i = 0; i < words.size(); i++)
            {
                const std::vector<Area>& charAreas = words[i].getCharsAreas();

                int lineOfWord = (charAreas[0].leftUp().y() - wordsCorner.y()) / lineHeight;
                if(lineOfWord < lineOfPos)
                {
                    id += charAreas.size();
                    continue;
                }
                else if(lineOfWord > lineOfPos)
                    break;

                // equal line of text
                for (size_t j = 0; j < charAreas.size(); j++)
                {
                    const Area& a = charAreas[j];
                    float tmpDelta = std::abs(a.leftUp().x() - originPos.x());
                    if (delta >= tmpDelta)
                    {
                        delta = tmpDelta;
                        retNum = id;
                    }
                    id++; // id always heave to be ++
                }
            }

            return retNum;
        }

        float FontEffect::getLineHeight() const
        {
//            float smallerSizePix = std::min(Frame::getWidthPixels(), Frame::getHeightPixels());
//            float normalizedFontSize = getConstFontGlyphSet()->getFontInfo().size / smallerSizePix;
//            float scale = m_textSettings.fontSize.getNormSize() / normalizedFontSize;
//            return Frame::pix2Norm(getConstFontGlyphSet()->getFontInfo().lineHeight) * scale;
            return m_words.getLineHeight();
        }

        float FontEffect::getLineTopAt(const Position& pos) const
        {
            float lineHeight = getLineHeight();
            return  ( (int)(( pos.y() - m_textSettings.area.leftUp().y() ) / lineHeight) ) * lineHeight + m_textSettings.area.leftUp().y();
        }

        void FontEffect::refresh(const TextSettings& ts, int flaggs)
        {
            m_textSettings = ts;

            if(flags::checkFlag(FLAG::TEXT_CHANGED, flaggs))
            {
                if(getFontGlyphSet()->canResizeCharset())
                {
                    aex::wstring notInSet = getFontGlyphSet()->whichCharsCantRenderFrom(m_textSettings.text);
                    if( ! notInSet.empty())
                    {
                        initTexture( getFontGlyphSet()->addToCharset(notInSet)) ;
                    }
                }
            }

            if (flags::checkFlag(FLAG::REBUILD_TEXT | FLAG::TEXT_CHANGED | FLAG::SIZE_CHANGED | FLAG::LINE_WRAP_CHANGED | FLAG::FONT_SIZE_CHANGED | FLAG::DISCARD_OUTED_CHANGED | FLAG::DISCARD_TRUNCATED_CHANGED | FLAG::ALIGNING_CHANGED, flaggs))
            {
                rebuildBatch();
            }

            if (flags::checkFlag(FLAG::POSITION_CHANGED, flaggs))
            {
                setDelta(m_textSettings.area.leftUp());
            }
        }

        void FontEffect::rebuildBatch()
        {
            Position originalPosition = m_textSettings.area.leftUp(); // it is delta too
            m_textSettings.area.setPosition(Position(0, 0)); // just for generation
            m_triangleBatch.clear();

            m_words = getFontGlyphSet()->generate(m_textSettings);
            const std::vector<FontGlyphSet::WordData>& wordsVec = m_words.getWordsData();
            m_testWrapSize = m_words.getSize();
            for (size_t i = 0; i < wordsVec.size(); i++)
            {
                if(!wordsVec[i].isRepresentingWord())
                    continue;

                const std::vector<Area>& charsAreas = wordsVec[i].getCharsAreas();
                const std::vector<Area>& uvsAreas = wordsVec[i].getCharsUVs();

                for (size_t j = 0; j < charsAreas.size(); j++)
                {
                    m_triangleBatch.addArea(charsAreas[j], uvsAreas[j]);
                }
            }
            m_textSettings.area.setPosition(originalPosition);

            m_aVertex2 = VertexData::makeVertexData(m_triangleBatch.getVBOData(), "aVertex2", 2);
            m_aUV2 = VertexData::makeVertexData(m_triangleBatch.getUVData(), "aUV", 2);
            m_indexes = IndexData::createTriangles(m_triangleBatch.getIndexes());
        }

        void FontEffect::initTexture(Texture* tex)
        {
            m_texture = tex;
            m_sampler = UniformSampler(m_texture, "uSampler");
        }

        FontGlyphSet* FontEffect::getFontGlyphSet()
        {
            return m_fntSrc->getFontGlyphSet();
        }

        const FontGlyphSet* FontEffect::getConstFontGlyphSet() const
        {
            return m_fntSrc->getConstFontGlyphSet();
        }
    }
}
