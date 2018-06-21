#ifndef FONT_GLYPH_SET_AEX_GUI_HEADER
#define FONT_GLYPH_SET_AEX_GUI_HEADER

#include <aex/gui/GUIMath.hpp>
#include <iostream>
#include <array>
#include <vector>
#include "TextSettings.hpp"

namespace aex {
    class Texture;
    namespace gui {

        struct Glyph {

            Glyph() : c('?'), x(0), y(0), width(0), height(0), xoffset(0), yoffset(0), advance(0), page(0), chnl(0) {
            }
            wchar_t c;
            int x, y;
            int width, height;
            int xoffset, yoffset;
            int advance;
            int page;
            int chnl; // chanel ? (4 bit if 15 ?)

            bool isEmpty() const {
                return ((x + y + width + height + advance) == 0);
            }

            GArea2D<int> getAreaOnTexture() const {
                return GArea2D<int>(GPosition2D<int>(x, y), GSize2D<int>(width, height));
            }
        };

        struct KerningPair {

            KerningPair() {
            }

            KerningPair(const wchar_t& first, const wchar_t& second) : first(first), second(second) {
            }
            wchar_t first;
            wchar_t second;

            friend bool operator==(const KerningPair& kp1, const KerningPair& kp2);
            friend bool operator<(const KerningPair& kp1, const KerningPair& kp2);
        };
        bool operator==(const KerningPair& kp1, const KerningPair& kp2);
        bool operator<(const KerningPair& kp1, const KerningPair& kp2);

        struct FontInfo {
            // info line
            std::string name;
            int size;
            std::string face;
            bool bold;
            bool italic;
            std::string charset;
            std::string unicode;
            int stretchH;
            bool smooth;
            bool aa;
            std::array<int, 4> padding;
            std::array<int, 2> spacing;
            bool outline;

            // common line
            int lineHeight;
            int base; // height ? like a or y size base?
            int textureWidth;
            int textureHeight;
            int pages;
            bool packed;
        };

        /**
         * @brief This is a set of data needed to render text with font.
         *
         * @details Base class for implementing glyph sets to render. Pure abstract.
         */
        class FontGlyphSet {
        public:

            /**
             * @brief Set of characters representing a word.
             *
             * Set of characteres to be rendered with ready positions
             * and uvs.
             */
            class WordData {
            public:
                friend class FontGlyphSet;
                const static int REPRESENT_WORD = 0;
                const static int REPRESENT_NEWLINE = 1;
                const static int REPRESENT_SPACE = 2;
                const static int REPRESENT_TAB = 3;

                /**
                 * @brief New WordData.
                 */
                WordData();
                virtual ~WordData();

                /**
                 * @brief Add new character to word.
                 *
                 * @param area Position of character.
                 * @param UVs Character UVs in font texture.
                 */
                void addCharacter(const Area& area, const Area& UVs);

                /**
                 * @brief Move word by delta.
                 *
                 * @param moveBy move word by delta.
                 */
                void moveBy(const Position& delta);

                /**
                 * @brief Get word characters positions.
                 *
                 * @return Vector.
                 */
                const std::vector<Area>& getCharsAreas() const;

                /**
                 * @brief Get word characters uvs.
                 *
                 * @return Vector.
                 */
                const std::vector<Area>& getCharsUVs() const;

                /**
                 * @brief Word size from top left corner to botom right.
                 *
                 * @return Size.
                 */
                Size getSize() const;

                /**
                 * @brief Top left position of the word.
                 * 
                 * @return Position
                 */
                Position leftUp() const;
                
                /**
                 * @brief Bottom right position of the word.
                 *
                 * @return Position.
                 */
                Position rightUp() const;

                /**
                 * @brief Test whether all of the word is contained in area.
                 *
                 * @param area Area to be tested agains.
                 * @return True word is in area.
                 */
                bool isIn(const Area& area); // all characters

                /**
                 * @brief Test whether atleas one character is in area.
                 *
                 * @param area Area to be tested agains.
                 * @return True atleast one character succeeds.
                 */
                bool crossWith(const Area& area); // just one of word chars

                /**
                 * @brief Is this a word.
                 *
                 * @return True this is really a word.
                 */
                bool isRepresentingWord() const;

                /**
                 * @brief Is this a newline.
                 *
                 * @return True if this is a newline.
                 */
                bool isRepresentingNewLine() const;

                /**
                 * @brief Is this a space.
                 *
                 * @return True if this is a space.
                 */
                bool isRepresentingSpace() const;

                /**
                 * @brief Is this a tab.
                 *
                 * @return True if this is a tab.
                 */
                bool isRepresentingTab() const;

            protected:
                void setRepresentingNewLine(const Position& pos);
                void setRepresentingSpace(const Area& area);
                void setRepresentingTab(const Area& area);

                void setIsPositioned(bool val);
                bool isPositioned() const;
                int m_representation;
                bool m_positioned;
                std::vector<Area> m_charsAreas;
                std::vector<Area> m_charsUVs;
            };

            /**
             * @brief Set of words.
             */
            class Words {
            public:
                friend class FontGlyphSet;
                /**
                 * @brief Create new empty word set.
                 */
                Words();
                
                /**
                 * @brief Create new empty word set.
                 *
                 * @param yStart Line start
                 * @param lineHeight Line height
                 */
                Words(float yStart, float lineHeight);

                Words(const Words& words) = delete;
                Words(Words&& words) = default;
                virtual ~Words();

                Words& operator=(const Words& words) = delete;
                Words& operator=(Words&& words) = default;;

                /**
                 * @brief Add word to word set.
                 *
                 * @param wordData Word.
                 */
                void push(WordData&& wordData);

                /**
                 * @brief Get words in word set
                 *
                 * @return Word set.
                 */
                const std::vector<WordData>& getWordsData() const;

                /**
                 * @brief Get word set area.
                 *
                 * @return Size.
                 */
                Size getSize() const;

                /**
                 * @brief return line height in word set.
                 *
                 * @return Line height.
                 */
                float getLineHeight() const {return m_lineHeight;}
            protected:
                std::vector<WordData> m_words;
                float m_yStart;
                float m_lineHeight;
            };

        public:
            /**
             * @brief Create new glyph set.
             */
            FontGlyphSet();
            virtual ~FontGlyphSet();

            /**
             * @brief Get texture  backing glyph set.
             *
             * @return Texture
             */
            Texture* getTexture();

            /**
             * @brief Get font information.
             *
             * @return Font information.
             */
            const FontInfo& getFontInfo() const;

            /**
             * @brief Normalized line height.
             */
            float getLineHeightNormalized() const;
            int32_t getKerning(const KerningPair& kp) const;

            /**
             * @brief Get glyph information for character.
             *
             * @param character Character.
             * @return Glyph info.
             */
            const Glyph& getGlyph(const wchar_t& character) const;

            /**
             * @brief Generate words form textSettings.
             *
             * @details Factory method used to generate positions and uvs
             * based on textSettings needed for rendering.
             *
             * @return New words.
             */
            Words generate(const TextSettings& textSettings);

            /**
             * @brief Can this set be resized.
             */
            virtual bool canResizeCharset() const = 0;

            /**
             * @brief If set can be resized returns texture with new chars.
             */
            virtual Texture* addToCharset(const aex::wstring& chars) = 0;

            /**
             * @brief Returns charaters in this set.
             */
            virtual aex::wstring getCharset() const = 0;

            /**
             * @brief Return list of unsupported characters in text.
             */
            aex::wstring whichCharsCantRenderFrom(const aex::wstring& text);

            int getRegenNum() const {return m_regenNum;}

            friend std::ostream& operator << (std::ostream& out, const FontGlyphSet& fgs);

        protected:
            aex::gui::Area genAreaFor(const Glyph& g, float fontHeight) const;
            aex::gui::Area genUVSFor(const Glyph& g) const;
            bool isNewline(const wchar_t& fc) const;
            bool isSpace(const wchar_t& fc) const;
            bool isTab(const wchar_t& fc) const;
            float getSpaceLenNorm(const TextSettings& textSettings, float scale) const;
            void placeWords(Words& words, const TextSettings& textSettings, float scale) const;
            void placeInLine(std::vector<WordData*> words, const TextSettings& ts, float scale, int lineNum) const;
            std::vector<WordData*> getWordsToPlace(Words& words, int idFrom, int idToWithoutIt) const;
            std::vector<WordData*> getTrimmed(const std::vector<WordData*>& input, const TextSettings& ts) const;
        protected:
            Glyph m_emptyGlyph;

            // need to be filled in child class
            FontInfo m_fontInfo;
            std::map<wchar_t, Glyph> m_glyphs;
            std::map<KerningPair, int32_t> m_kernings;
            Texture* m_texture;
            std::vector<wchar_t> m_notInGlyphsChars;
            int m_regenNum;
        };
        std::ostream& operator << (std::ostream& out, const FontGlyphSet& fgs);
    }
}

#endif
