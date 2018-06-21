#include "aex/gui/BitmapFont.hpp"
namespace aex
{
    namespace gui
    {   
        BitmapFont::BitmapFont(const std::string& glyphsPath, const std::string& texturePath, int size) :
         m_glyphsPath(glyphsPath),
         m_texturePath(texturePath),
         m_size(size)
         {
         }
         
         BitmapFont::~BitmapFont()
         {
         }
         
         const std::string& BitmapFont::getGlyphsPath() const
         {
             return m_glyphsPath;
         }
         
         const std::string& BitmapFont::getTexturePath() const
         {
             return m_texturePath;
         }
         
         int BitmapFont::getSize() const
         {
             return m_size;
         }
         
         BitmapFontGroup::BitmapFontGroup(const std::vector<BitmapFont>& fontsGroup) :
         m_fonts(fontsGroup)
         {
         }
            
         BitmapFontGroup::~BitmapFontGroup()
         {
         }
         
         void BitmapFontGroup::addBitmapFont(const BitmapFont& font)
         {
             m_fonts.push_back(font);
         }
         
         const BitmapFont& BitmapFontGroup::getBestFontFor(int size) const
         {
             size = 0.80 * size +1;
             int index = 0;
             for(unsigned i = 0 ; i < m_fonts.size() ; i++)
             {
                 index = i;
                 if(m_fonts[i].getSize() >= size)
                     break;
             }
             
             return m_fonts[index];
         }
    }
}