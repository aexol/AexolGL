#ifndef BITMAP_FONT_HEADER_AEX
#define BITMAP_FONT_HEADER_AEX

#include <string>
#include "GUIMath.hpp"

namespace aex {
    namespace gui {

        /**
         * @brief Pair of Angel Code and texture paths.
         *
         * @details This is class is a pair of two files.
         * One is Angel Code description of font map and
         * the other is bitmap with font.
         *
         * @headerfile BitmapFont.hpp <aex/BitmapFont.hpp>
         */
        class AEX_PUBLIC_API BitmapFont {
        public:
            /**
             * @brief New BitmapFont
             * 
             * @details BitmapFont constructor.
             *
             * @param glyphsPath Angel Code text file describing the font map.
             * @param texturePath Bitmap with font.
             * @param size Font size.
             */
            BitmapFont(const std::string& glyphsPath,
                    const std::string& texturePath, int size);

            virtual ~BitmapFont();

            /**
             * @brief Returns path to Angel Code file.
             *
             * @details Returns the path to Angel Code file.
             *
             * @return File path.
             */
            const std::string& getGlyphsPath() const;

            /**
             * @brief Returns path to bitmap file.
             *
             * @details Returns the path to bitmap file.
             *
             * @return File path.
             */
            const std::string& getTexturePath() const;

            /**
             * @brief Returns font size of this bitmap.
             *
             * @details Returns the size of this bitmap.
             *
             * @return Size.
             */
            int getSize() const;
        protected:
            std::string m_glyphsPath;
            std::string m_texturePath;
            int m_size;
        };

        /**
         * @brief Group different sizes of font.
         * 
         * Creates a group of bitmaps using the same font.
         * Useful when there's multiple prerendered bitmaps
         * for better quality.
         */
        class AEX_PUBLIC_API BitmapFontGroup {
        public:
            /**
             * @brief Create new font group.
             *
             * @param fontsGroup Vector of bitmaps. (Optional)
             */
            BitmapFontGroup(const std::vector<BitmapFont>& fontsGroup = std::vector<BitmapFont>());
            virtual ~BitmapFontGroup();

            /**
             * @brief Add new bitmap to font group.
             *
             * @param font Angel Code/Bitmap pair.
             */
            void addBitmapFont(const BitmapFont& font);

            /**
             * @brief Find the best matching bitmap for size.
             *
             * @param size Requested size.
             */
            const BitmapFont& getBestFontFor(int size) const;
        protected:
            std::vector<BitmapFont> m_fonts;
        };
    }
}


#endif
