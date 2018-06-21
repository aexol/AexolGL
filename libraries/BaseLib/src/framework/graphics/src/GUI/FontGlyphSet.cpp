#include "aex/gui/FontGlyphSet.hpp"
#include "aex/gui/Frame.hpp"
#include <aex/Texture.hpp>
#include <aex/Exceptions.hpp>

namespace aex
{
    namespace gui
    {

        bool operator==(const KerningPair& kp1, const KerningPair& kp2)
        {
            return (kp1.first == kp2.first) && (kp1.second == kp2.second);
        }

        bool operator<(const KerningPair& kp1, const KerningPair& kp2)
        {
            return (kp1.first == kp2.first) ? (kp1.second < kp2.second) : (kp1.first < kp2.first);
        }

        FontGlyphSet::WordData::WordData() :
        m_representation(REPRESENT_WORD),
        m_positioned(false)
        {
        }

        FontGlyphSet::WordData::~WordData()
        {
        }

        void FontGlyphSet::WordData::addCharacter(const Area& area, const Area& UVs)
        {
            m_charsAreas.push_back(area);
            m_charsUVs.push_back(UVs);
        }

        void FontGlyphSet::WordData::moveBy(const Position& delta)
        {
            for (size_t i = 0; i < m_charsAreas.size(); i++)
            {
                m_charsAreas[i].move(delta);
            }
        }

        const std::vector<Area>& FontGlyphSet::WordData::getCharsAreas() const
        {
            return m_charsAreas;
        }

        const std::vector<Area>& FontGlyphSet::WordData::getCharsUVs() const
        {
            return m_charsUVs;
        }

        Size FontGlyphSet::WordData::getSize() const
        {
            if (m_charsAreas.empty())
                return Size(0, 0);

            Position leftUp = m_charsAreas[0].leftUp();
            Position lastRightDown = m_charsAreas[m_charsAreas.size() - 1].rightDown();

            return Size(lastRightDown.x() - leftUp.x(), lastRightDown.y() - leftUp.y());
        }

        Position FontGlyphSet::WordData::leftUp() const
        {
            if (m_charsAreas.empty())
                return Position(0, 0);

            return m_charsAreas[0].leftUp();
        }

        Position FontGlyphSet::WordData::rightUp() const
        {
            if (m_charsAreas.empty())
                return Position(0, 0);

            return m_charsAreas[m_charsAreas.size() - 1].rightUp();
        }

        bool FontGlyphSet::WordData::isIn(const Area& area)
        {
            if (m_charsAreas.empty())
                return false;

            const Area& char1A = m_charsAreas[0];
            const Area& charLastA = m_charsAreas[m_charsAreas.size() - 1];
            return area.contains(char1A) && area.contains(charLastA);
        }

        bool FontGlyphSet::WordData::crossWith(const Area& area)
        {
            bool retVal = false;
            for (size_t i = 0; i < m_charsAreas.size(); i++)
            {
                if (area.intersectWith(m_charsAreas[i]))
                {
                    retVal = true;
                    break;
                }
            }

            return retVal;
        }

        bool FontGlyphSet::WordData::isRepresentingWord() const
        {
            return m_representation == REPRESENT_WORD;
        }

        void FontGlyphSet::WordData::setRepresentingNewLine(const Position& pos)
        {
            m_representation = REPRESENT_NEWLINE;
            m_charsAreas.clear();
            m_charsUVs.clear();
            m_charsAreas.push_back(Area(pos, Size()));
        }

        bool FontGlyphSet::WordData::isRepresentingNewLine() const
        {
            return m_representation == REPRESENT_NEWLINE;
        }

        void FontGlyphSet::WordData::setRepresentingSpace(const Area& area)
        {
            m_representation = REPRESENT_SPACE;
            m_charsAreas.clear();
            m_charsUVs.clear();
            m_charsAreas.push_back(area);
        }

        bool FontGlyphSet::WordData::isRepresentingSpace() const
        {
            return m_representation == REPRESENT_SPACE;
        }

        void FontGlyphSet::WordData::setRepresentingTab(const Area& area)
        {
            m_representation = REPRESENT_TAB;
            m_charsAreas.clear();
            m_charsUVs.clear();
            m_charsAreas.push_back(area);
        }

        bool FontGlyphSet::WordData::isRepresentingTab() const
        {
            return m_representation == REPRESENT_TAB;
        }

        void FontGlyphSet::WordData::setIsPositioned(bool val)
        {
            m_positioned = val;
        }

        bool FontGlyphSet::WordData::isPositioned() const
        {
            return m_positioned;
        }

        FontGlyphSet::Words::Words() :
        m_yStart(0.0f),
        m_lineHeight(1.0f)
        {
        }

        FontGlyphSet::Words::Words(float yStart, float lineHeight) :
        m_yStart(yStart),
        m_lineHeight(lineHeight)
        {
        }

        FontGlyphSet::Words::~Words()
        {
        }

        void FontGlyphSet::Words::push(WordData&& wordData)
        {
            m_words.push_back(std::move(wordData));
        }

        const std::vector<FontGlyphSet::WordData>& FontGlyphSet::Words::getWordsData() const
        {
            return m_words;
        }

        Size FontGlyphSet::Words::getSize() const
        {
            if (m_words.empty())
                return Size(0.0f, 0.0f);
            if (m_words[0].getCharsAreas().empty())
                return Size(0.0f, 0.0f);

            Position lu = m_words[0].getCharsAreas()[0].leftUp();
            Position rd = m_words[0].getCharsAreas()[0].rightDown();

            lu.setY(m_yStart);

            for (size_t i = 0; i < m_words.size(); i++)
            {
                const std::vector<Area>& wdAreas = m_words[i].getCharsAreas();
                if (wdAreas.empty())
                    continue;

                // check only 2 chars, first and last of tmp word
                lu.setX(std::min(lu.x(), wdAreas[0].leftUp().x())); // first char of word
                lu.setY(std::min(lu.y(), wdAreas[0].leftUp().y()));

                rd.setX(std::max(rd.x(), wdAreas[wdAreas.size() - 1].rightDown().x())); // last char of word
                rd.setY(std::max(rd.y(), wdAreas[wdAreas.size() - 1].rightDown().y()));
            }

            float nextYBottom = m_yStart;
            while (nextYBottom < rd.y())
                nextYBottom += m_lineHeight;

            rd.setY(nextYBottom);

            return Size(rd.x() - lu.x(), rd.y() - lu.y());
        }

        FontGlyphSet::FontGlyphSet() :
        m_regenNum(0)
        {
            m_texture = nullptr;
        }

        FontGlyphSet::~FontGlyphSet()
        {
        }

        Texture* FontGlyphSet::getTexture()
        {
            if (!m_texture)
                throw aex::AexolError("Fill m_texture, m_glyphs, m_kernings and m_fontInfo members if u wanna properly use FontGlyphSet class as base");

            return m_texture;
        }

        const FontInfo& FontGlyphSet::getFontInfo() const
        {
            return m_fontInfo;
        }

        float FontGlyphSet::getLineHeightNormalized() const
        {
            const FontInfo& fontInfo = getFontInfo();
            return fontInfo.size / std::min(Frame::getWidthPixels(), Frame::getHeightPixels()); // TODO check corectness
        }

        int32_t FontGlyphSet::getKerning(const KerningPair& kp) const
        {
            if (m_kernings.find(kp) != m_kernings.end())
                return m_kernings.at(kp);
            else
                return 0;
        }

        const Glyph& FontGlyphSet::getGlyph(const wchar_t& character) const
        {
            if (m_glyphs.find(character) != m_glyphs.end())
            {
                return m_glyphs.at(character);
            } else
            {
                return m_emptyGlyph;
            }
        }

        aex::gui::Area FontGlyphSet::genAreaFor(const Glyph& g, float fontHeight) const
        {
            const FontInfo& fontInfo = getFontInfo();
            if (g.isEmpty())
                return Area();

            float smallerSizePix = std::min(Frame::getWidthPixels(), Frame::getHeightPixels());
            float normalizedFontSize = fontInfo.size / smallerSizePix; // TODO // shouldnt be line hegith here ?
            float scale = fontHeight / normalizedFontSize;

            Position basePos(0, 0);
            Size charSize((g.width / smallerSizePix) * scale, (g.height / smallerSizePix) * scale);

            return Area(basePos, charSize);
        }

        aex::gui::Area FontGlyphSet::genUVSFor(const Glyph& g) const
        {
            const FontInfo& fontInfo = getFontInfo();
            if (g.isEmpty())
                return Area();

            GArea2D<int> gArea = g.getAreaOnTexture();
            GSize2D<int> texSize(fontInfo.textureWidth, fontInfo.textureHeight);

            aex::gui::Position leftUpUV((gArea.leftUp().x() / (float) texSize.w()), (gArea.leftUp().y() / (float) texSize.h()));
            aex::gui::Position leftDownUV((gArea.leftDown().x() / (float) texSize.w()), (gArea.leftDown().y() / (float) texSize.h()));
            aex::gui::Position rightUpUV((gArea.rightUp().x() / (float) texSize.w()), (gArea.rightUp().y() / (float) texSize.h()));
            aex::gui::Position rightDownUV((gArea.rightDown().x() / (float) texSize.w()), (gArea.rightDown().y() / (float) texSize.h()));

            Area UVs(leftDownUV, Size(rightUpUV.x() - leftUpUV.x(), rightUpUV.y() - rightDownUV.y()));

            return UVs;
        }

        FontGlyphSet::Words FontGlyphSet::generate(const TextSettings& textSettings)
        {
            int wordCharNum = 0;
            const FontInfo& fontInfo = getFontInfo();
            Position ptrPos = textSettings.area.leftUp();

            float smallerSizePix = std::min(Frame::getWidthPixels(), Frame::getHeightPixels());
            float normalizedFontSize = fontInfo.size / smallerSizePix;
            float scale = textSettings.fontSize.getNormSize() / normalizedFontSize;

            Words words(textSettings.area.leftUp().y(), Frame::pix2Norm(fontInfo.lineHeight) * scale);
            WordData word;
            float lineHeightNorm = Frame::pix2Norm(fontInfo.lineHeight) * scale;

            wchar_t prev = 0;
            for (size_t i = 0; i < textSettings.text.size(); i++)
            {
                wchar_t next = textSettings.text.at(i);
                bool putNewWord = false;
                if (isNewline(next) || isSpace(next) || isTab(next))
                {
                    putNewWord = true;
                }

                if (putNewWord)
                {
                    if (!word.getCharsAreas().empty()) // if spaces or tabs before new line, will be empty
                        words.push(std::move(word));

                    word = WordData();

                    if (isNewline(next))
                    {
                        WordData newLineRepresentation;
                        newLineRepresentation.setRepresentingNewLine(ptrPos);
                        words.push(std::move(newLineRepresentation));
                    } else if (isSpace(next))
                    {
                        float sizeNorm = getSpaceLenNorm(textSettings, scale);
                        WordData spaceRep;
                        spaceRep.setRepresentingSpace(Area(ptrPos, Size(sizeNorm, lineHeightNorm)));
                        words.push(std::move(spaceRep));
                        ptrPos.move(sizeNorm, 0);
                    } else if (isTab(next))
                    {
                        float sizeNorm = getSpaceLenNorm(textSettings, scale) * 4;
                        WordData tabRep;
                        tabRep.setRepresentingTab(Area(ptrPos, Size(sizeNorm, lineHeightNorm)));
                        words.push(std::move(tabRep));
                        ptrPos.move(sizeNorm, 0);
                    }

                    wordCharNum = 0;
                    continue;
                }

                const Glyph& g = getGlyph(next);
                if (g.isEmpty())
                {
                    continue;
                }

                float xOffsetNorm = Frame::pix2Norm(g.xoffset) * scale;
                float yOffsetNorm = Frame::pix2Norm(g.yoffset) * scale;
                float xAdvanceNorm = Frame::pix2Norm(g.advance) * scale;
                float trackingNorm = textSettings.tracking.getNormSize(); // not needed scale, info from user FontSize class (real normalized size in screen)
                float xKerningNorm = 0.0f;

                if (wordCharNum == 0)
                {
                    if (!textSettings.apllyXOffsetAtWordBeginning)
                    {
                        xOffsetNorm = 0;
                    }
                } else
                {
                    if (textSettings.withKerning)
                    {
                        int32_t kerning = getKerning(KerningPair(prev, next));
                        xKerningNorm = Frame::pix2Norm(kerning) * scale;
                    }
                }

                // char tmp ptr
                Position glyphPtr = ptrPos;
                //glyphPtr.move(xOffsetNorm + xKerningNorm, yOffsetNorm);
                glyphPtr.move(xOffsetNorm, yOffsetNorm);
                ptrPos.move(xAdvanceNorm + xKerningNorm + trackingNorm, 0.0f);

                Area glyphArea = genAreaFor(g, textSettings.fontSize.getNormSize());
                Area glyphUVs = genUVSFor(g);
                glyphArea.setPosition(glyphPtr);
                word.addCharacter(glyphArea, glyphUVs);

                ++wordCharNum;
                prev = next;
            }
            if (!word.getCharsAreas().empty())
            {
                words.push(std::move(word));
            }

            placeWords(words, textSettings, scale);
            return words;
        }

        aex::wstring FontGlyphSet::whichCharsCantRenderFrom(const aex::wstring& text)
        {
            aex::wstring retwstr;
            for(size_t i = 0 ; i < text.size() ; i++)
            {
                wchar_t next = text[i];
                const Glyph& g = getGlyph(next);
                if (g.isEmpty())
                {
                    retwstr += next;
                }
            }

            return retwstr;
        }

        bool FontGlyphSet::isNewline(const wchar_t& fc) const
        {
            if (char(fc) == '\n')
            {
                return true;
            }
            return false;
        }

        bool FontGlyphSet::isSpace(const wchar_t& fc) const
        {
            if (char(fc) == ' ')
                return true;

            return false;
        }

        bool FontGlyphSet::isTab(const wchar_t& fc) const
        {
            if (char(fc) == '\t')
                return true;

            return false;
        }

        float FontGlyphSet::getSpaceLenNorm(const TextSettings& textSettings, float scale) const
        {
            wchar_t space = text::c2wc(' ');
            const Glyph& g = getGlyph(space);
            if (!g.isEmpty())
            {
                return Frame::pix2Norm(g.advance) * scale;
            } else
            {
                throw FontError("Space character not generated in glyphs set. Please attach space character info (code of space char is 32)");
            }
        }

        void FontGlyphSet::placeWords(Words& words, const TextSettings& ts, float scale) const
        {
            const FontInfo& fontInfo = getFontInfo();
            int idFrom = 0;
            size_t idTo = 0;
            int lineNum = 0;
            Position posToLineWrap = ts.area.p();

            for (size_t i = 0; i < words.m_words.size(); i++)
            {
                // always do newlines
                if (words.m_words[i].isRepresentingNewLine())
                {
                    idTo = i;
                    placeInLine(getWordsToPlace(words, idFrom, idTo), ts, scale, lineNum);
                    lineNum++;
                    idFrom = idTo;

                    // if next word exists
                    if ((i + 1) < words.m_words.size())
                        posToLineWrap.setX(words.m_words[i + 1].getCharsAreas()[0].leftUp().x());

                } else if (ts.lineWrap)
                {
                    // check is in y height
                    float lineHeightNorm = Frame::pix2Norm(fontInfo.lineHeight) * scale * 1;
                    float deltaY = Frame::pix2Norm(fontInfo.lineHeight) * scale * lineNum;
                    Position linePos = ts.area.leftUp();
                    linePos.move(0, deltaY);

                    bool discardRestWords = false;
                    if (ts.discardTruncated && ((linePos.y() + lineHeightNorm) > ts.area.leftDown().y()))
                        discardRestWords = true;

                    if (ts.discardOuted && (linePos.y() > ts.area.leftDown().y()))
                        discardRestWords = true;

                    if (discardRestWords)
                    {
                        words.m_words.erase(words.m_words.begin() + i, words.m_words.end());
                        break;
                    }

                    Area tmpArea = ts.area;
                    tmpArea.setPosition(Position(posToLineWrap.x(), posToLineWrap.y() - 999));
                    tmpArea.setSize(Size(ts.area.s().w(), 999999));

                    // if not discardet and if not first word in line -> do wrapping
                    if (!words.m_words[i].isIn(tmpArea)) // if not full word is in and is not first word, move it to newLine (while moving, do not check this word again (if area is always smaller than word, we will have infinite loop)
                    {
                        if(idTo != i) // if not first word in this line
                        {
                          idTo = i;
                          placeInLine(getWordsToPlace(words, idFrom, idTo), ts, scale, lineNum);
                          lineNum++;
                          idFrom = idTo;
                          posToLineWrap.setX(words.m_words[i].getCharsAreas()[0].leftUp().x());
                      }
                    }
                } else
                {
                    // if not wrapping and wanna discard
                    bool removeRestWords = false;
                    if (ts.discardOuted && !words.m_words[i].isIn(ts.area))
                        removeRestWords = true;

                    if (ts.discardTruncated && (words.m_words[i].crossWith(ts.area) && !words.m_words[i].isIn(ts.area)))
                        removeRestWords = true;

                    if (removeRestWords)
                    {
                        words.m_words.erase(words.m_words.begin() + i, words.m_words.end());
                        break; // all loop
                    }
                }
            }

            // last line
            idTo = words.m_words.size();
            placeInLine(getWordsToPlace(words, idFrom, idTo), ts, scale, lineNum);
        }

        // using aligning rules

        void FontGlyphSet::placeInLine(std::vector<WordData*> words, const TextSettings& ts, float scale, int lineNum) const
        {
            if (words.empty())
                return;

            const FontInfo& fontInfo = getFontInfo();
            float deltaY = Frame::pix2Norm(fontInfo.lineHeight) * scale * lineNum;
            Position linePos = ts.area.leftUp();
            linePos.move(0, deltaY);

            // first step - always move words pos y to chosen line: // DELTA BUT BY LINES HEIGHT, SO:!!!!
            Position wordsDeltaY = linePos - ts.area.leftUp(); //
            wordsDeltaY.setX(0);
            for (size_t i = 0; i < words.size(); i++)
            {
                words[i]->moveBy(wordsDeltaY);
            }

            if (ts.lineWrap)
            {
                if (ts.trimLeft || ts.trimRight)
                    words = getTrimmed(words, ts);
            }

            Position firstCharLeftUp = words[0]->getCharsAreas()[0].leftUp();
            Position lastCharRightUp = words[words.size() - 1]->getCharsAreas()[words[words.size() - 1]->getCharsAreas().size() - 1].rightUp(); // so ugly code line

            // now using aligning set X positions
            switch (ts.align)
            {
                case ALIGN::LEFT:
                {
                    Position wordsDeltaX = linePos - firstCharLeftUp;
                    wordsDeltaX.setY(0);
                    for (size_t i = 0; i < words.size(); i++)
                    {
                        words[i]->moveBy(wordsDeltaX);
                    }
                }
                    break;
                case ALIGN::CENTER:
                {
                    float leftSpace = (ts.area.s().w() - (lastCharRightUp.x() - firstCharLeftUp.x())) / 2;
                    Position startPtr = linePos;
                    startPtr.move(leftSpace, 0);
                    Position wordsDeltaX = startPtr - firstCharLeftUp;
                    wordsDeltaX.setY(0);
                    for (size_t i = 0; i < words.size(); i++)
                    {
                        words[i]->moveBy(wordsDeltaX);
                    }
                }
                    break;
                case ALIGN::RIGHT:
                {
                    float leftSpace = (ts.area.s().w() - (lastCharRightUp.x() - firstCharLeftUp.x()));
                    Position startPtr = linePos;
                    startPtr.move(leftSpace, 0);
                    Position wordsDeltaX = startPtr - firstCharLeftUp;
                    wordsDeltaX.setY(0);
                    for (size_t i = 0; i < words.size(); i++)
                    {
                        words[i]->moveBy(wordsDeltaX);
                    }
                }
                    break;
                case ALIGN::JUSTIFIED:
                {
                    // all to left, then;
                    Position wordsDeltaX = linePos - firstCharLeftUp;
                    wordsDeltaX.setY(0);
                    for (size_t i = 0; i < words.size(); i++)
                    {
                        words[i]->moveBy(wordsDeltaX);
                    }

                    float freeSpace = (ts.area.s().w() - (lastCharRightUp.x() - firstCharLeftUp.x()));
                    int spaces = words.size() - 1;
                    float addition = freeSpace;
                    if (spaces > 1)
                        addition /= spaces;

                    for (size_t i = 1; i < words.size(); i++)
                    {
                        Position wd40 = Position(addition*i, 0.0f);
                        words[i]->moveBy(wd40);
                    }
                }
                    break;
                default:
                {
                }
            }

            // check words
            for (size_t i = 0; i < words.size(); i++)
            {
                words[i]->setIsPositioned(true);
            }
        }

        std::vector<FontGlyphSet::WordData*> FontGlyphSet::getWordsToPlace(Words& words, int idFrom, int idToWithoutIt) const
        {
            std::vector<WordData*> wordsToPlace;
            for (int i = idFrom; i < idToWithoutIt; i++)
            {
                if (words.m_words[i].isPositioned())
                    continue;

//                if (words.m_words[i].isRepresentingNewLine())
//                    continue;

                wordsToPlace.push_back(&words.m_words[i]);
            }
            return wordsToPlace;
        }

        std::vector<FontGlyphSet::WordData*> FontGlyphSet::getTrimmed(const std::vector<WordData*>& input, const TextSettings& ts) const
        {
            std::vector<WordData*> retVec;
            int firstID = 0;
            int lastIDWithoutIt = input.size();
            if (ts.trimLeft)
            {
                Position areaLeftUp = ts.area.leftUp();
                for (size_t i = 0; i < input.size(); i++)
                {
                    if (input[i]->isRepresentingWord())
                    {
                        firstID = i;
                        break;
                    }
                    // move whitespaces to left border (for touched char num identifiaction)
                    Position deltaX = areaLeftUp - input[i]->leftUp();
                    deltaX.setY(0);
                    input[i]->moveBy(deltaX);
                }
            }
            if (ts.trimRight)
            {
                Position areaRightUp = ts.area.rightUp();
                for (int i = input.size() - 1; i >= 0; i--)
                {
                    if (input[i]->isRepresentingWord())
                    {
                        lastIDWithoutIt = i + 1;
                        break;
                    }
                    // move whitespaces to right border (for touched char num identifiaction)
                    Position deltaX = areaRightUp - input[i]->leftUp();
                    deltaX.setY(0);
                    input[i]->moveBy(deltaX);
                }
            }
            if (lastIDWithoutIt < firstID)
                lastIDWithoutIt = firstID;

            for (int i = firstID; i < lastIDWithoutIt; i++)
            {
                retVec.push_back(input[i]);
            }
            return retVec;
        }

        std::ostream& operator<<(std::ostream& out, const FontGlyphSet& fgs)
        {
            out << "#FontInfo: " << std::endl;
            out << "name: " << fgs.m_fontInfo.name << std::endl;
            out << "size: " << fgs.m_fontInfo.size << std::endl;
            out << "face: " << fgs.m_fontInfo.face << std::endl;
            out << "bold: " << fgs.m_fontInfo.bold << std::endl;
            out << "italic: " << fgs.m_fontInfo.italic << std::endl;
            out << "charset: " << fgs.m_fontInfo.charset << std::endl;
            out << "unicode: " << fgs.m_fontInfo.unicode << std::endl;
            out << "stretchH: " << fgs.m_fontInfo.stretchH << std::endl;
            out << "aa: " << fgs.m_fontInfo.aa << std::endl;
            out << "padding: " << aex::string(fgs.m_fontInfo.padding.begin(), fgs.m_fontInfo.padding.end()) << std::endl;
            out << "spacing: " << aex::string(fgs.m_fontInfo.spacing.begin(), fgs.m_fontInfo.spacing.end()) << std::endl;
            out << "outline: " << fgs.m_fontInfo.outline << std::endl;
            out << "lineHeight: " << fgs.m_fontInfo.lineHeight << std::endl;
            out << "base: " << fgs.m_fontInfo.base << std::endl;
            out << "textureWidth: " << fgs.m_fontInfo.textureWidth << std::endl;
            out << "textureHeight: " << fgs.m_fontInfo.textureHeight << std::endl;
            out << "pages: " << fgs.m_fontInfo.pages << std::endl;
            out << "packed: " << fgs.m_fontInfo.packed << std::endl;

            std::vector<wchar_t> keys;
            for (std::map<wchar_t, Glyph>::const_iterator it = fgs.m_glyphs.begin(); it != fgs.m_glyphs.end(); ++it)
            {
                keys.push_back(it->first);
            }

            // print glyphs
            std::sort(keys.begin(), keys.end());
            for (size_t i = 0; i < keys.size(); i++)
            {
                const Glyph& g = fgs.m_glyphs.at(keys[i]);
                out << "[code:" << g.c << "],";
                out << "[c:" << text::wc2c(g.c) << "],";
                out << "[x:" << g.x << "],";
                out << "[y:" << g.y << "],";
                out << "[width:" << g.width << "],";
                out << "[height:" << g.height << "],";
                out << "[xoffset:" << g.xoffset << "],";
                out << "[yoffset:" << g.yoffset << "],";
                out << "[advance:" << g.advance << "],";
                out << "[page:" << g.page << "],";
                out << "[chnl:" << g.chnl << "]";
                out << std::endl;
            }

            // print kernings ? TODO
            return out;
        }
    }
}
