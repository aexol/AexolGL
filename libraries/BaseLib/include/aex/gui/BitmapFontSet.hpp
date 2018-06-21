#ifndef BITMAP_FONT_SET_AEX_GUI_HEADER
#define BITMAP_FONT_SET_AEX_GUI_HEADER

#include "FontGlyphSet.hpp"
#include "GUIMath.hpp"

#include <array>
#include <vector>
#include <map>


namespace aex {
    namespace gui {
        
        /**
         * @brief Bitmap implenetation of FontGlyphSet.
         */
        class BitmapFontSet : public FontGlyphSet {
        public:
            /**
             * @brief Create new bitmap glyph set.
             *
             * @param txtFilePath Path to Angel Code description.
             * @param texturePath Path to bitmap.
             */
            BitmapFontSet(const std::string& txtFilePath, const std::string& texturePath);
            virtual ~BitmapFontSet();
            
            /**
             * @brief Always false.
             */
            virtual bool canResizeCharset() const;

            /**
             * @brief Texture backing this bitmap.
             */
            virtual Texture* addToCharset(const aex::wstring& chars);

            /**
             * @brief Characters in set.
             */
            virtual aex::wstring getCharset() const;
            
        protected:
            void parseInfoLine(const std::string& infoLine);
            void parseCommonLine(const std::string& commonLine);
            void parsePageLine(const std::string& pageLine);
            void parseCharLine(const std::string& charLine);
            void parseKerningLine(const std::string& kerningLine);
            std::string getPartValue(const std::string& part);
            std::string getPartKey(const std::string& part);
            std::vector<std::string> getCommaSeparatedVector(const std::string& commaSepVecString);
            
        protected:
            std::string m_txtFilePath;
            std::string m_texturePath;
        };
    }
}

#endif
