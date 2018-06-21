#include "aex/gui/BitmapFontSet.hpp"
#include "aex/gui/GUIMath.hpp"


#include <aex/FileManager.hpp>
#include <aex/Exceptions.hpp>
#include <aex/Texture.hpp>
#include <aex/TextureManager.hpp>

#include <sstream>

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>


// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}


// trim from both ends (copying)
static inline std::string trimmed(std::string s) {
    trim(s);
    return s;
}

namespace aex
{
    namespace gui
    {

        BitmapFontSet::BitmapFontSet(const std::string& txtFilePath, const std::string& texturePath) :

        m_txtFilePath(txtFilePath),
        m_texturePath(texturePath)
        {
            typedef boost::filesystem::path path;
            FileManager fManager;

            path texPath = fManager.work_dir();
            texPath /= path{texturePath};

            m_texture = aex::TextureManager::GetInstance().getTexture(texturePath, aex::TextureConfig()); // TODO think about change a little bit it to set TextureConfig Better
            if (m_texture == nullptr)
                throw aex::FileNotFoundError("Can't open texture: " + texPath.string());

            ifstream fileHandle = fManager.open_asset(txtFilePath);
            if (!fileHandle)
            {
                path p = fManager.work_dir();
                p /= path{txtFilePath};
                throw aex::FileNotFoundError("Can't open file: " + p.string());
            }

            std::string line, part;
            while (std::getline(fileHandle, line))
            {
                std::stringstream lineSS;
                lineSS << line;
                
                while (std::getline(lineSS, part, ' '))
                {
                    if (part == "info")
                    {
                        parseInfoLine(line);
                        break;
                    }
                    if (part == "common")
                    {
                        parseCommonLine(line);
                        break;
                    }
                    if (part == "page")
                    {
                        parsePageLine(line);
                        break;
                    }
                    if (part == "char")
                    {
                        parseCharLine(line);
                        break;
                    }
                    if (part == "kerning")
                    {
                        parseKerningLine(line);
                        break;
                    }
                }
            }
            
            //LOG_DEBUG(*this);
        }

        BitmapFontSet::~BitmapFontSet()
        {
            if (m_texture)
            {
                aex::TextureManager::GetInstance().releaseTexture(m_texture);
            }
        }
        
        bool BitmapFontSet::canResizeCharset() const
        {
            return false;
        }
        
        Texture* BitmapFontSet::addToCharset(const aex::wstring& chars)
        {
            return m_texture;
        }
        
        aex::wstring BitmapFontSet::getCharset() const
        {
            aex::wstring charset;
            for(const auto& pair : m_glyphs)
                charset += pair.first;
            return charset;
        }
        
        void BitmapFontSet::parseInfoLine(const std::string & infoLine)
        {
            std::string part;
            std::stringstream lineSS(infoLine);
            while (std::getline(lineSS, part, ' '))
            {
                std::string key = getPartKey(part); trim(key);
                if (key =="face" )
                {
                    m_fontInfo.face = getPartValue(part);
                    continue;
                }
                if (key =="size" )
                {
                    m_fontInfo.size = atoi(getPartValue(part).c_str());
                    continue;
                }
                if (key =="bold" )
                {
                    m_fontInfo.bold = trimmed(getPartValue(part)) == "1";
                    continue;
                }
                if (key =="italic" )
                {
                    m_fontInfo.italic = trimmed(getPartValue(part)) == "1";
                    continue;
                }
                if (key =="charset" )
                {
                    m_fontInfo.charset = getPartValue(part);
                    continue;
                }
                if (key =="unicode" )
                {
                    m_fontInfo.unicode = getPartValue(part);
                    continue;
                }
                if (key =="stretchH" )
                {
                    m_fontInfo.stretchH = atoi(getPartValue(part).c_str());
                    continue;
                }
                if (key =="smooth" )
                {
                    m_fontInfo.smooth = getPartValue(part) == "1";
                    continue;
                }
                if (key =="aa" )
                {
                    m_fontInfo.aa = getPartValue(part) == "1";
                    continue;
                }
                if (key =="padding" )
                {
                    std::vector<std::string> vec = getCommaSeparatedVector(getPartValue(part));
                    for (size_t i = 0; i < vec.size(); i++)
                    {
                        m_fontInfo.padding[i] = atoi(vec[i].c_str());
                        continue;
                    }
                }
                if (key =="spacing" )
                {
                    std::vector<std::string> vec = getCommaSeparatedVector(getPartValue(part));
                    for (size_t i = 0; i < vec.size(); i++)
                    {
                        m_fontInfo.spacing[i] = atoi(vec[i].c_str());
                        continue;
                    }
                }
                if (key =="outline" )
                {
                    m_fontInfo.outline = atoi(getPartValue(part).c_str());
                    continue;
                }
            }
        }

        void BitmapFontSet::parseCommonLine(const std::string & commonLine)
        {
            std::string part;
            std::stringstream lineSS(commonLine);
            while (std::getline(lineSS, part, ' '))
            {
                 std::string key = getPartKey(part); trim(key);
                if (key =="lineHeight" )
                {
                    m_fontInfo.lineHeight = atoi(getPartValue(part).c_str());
                    continue;
                }
                if (key =="base" )
                {
                    m_fontInfo.base = atoi(getPartValue(part).c_str());
                    continue;
                }
                if (key =="scaleW" )
                {
                    m_fontInfo.textureWidth = atoi(getPartValue(part).c_str());
                    continue;
                }
                if (key =="scaleH" )
                {
                    m_fontInfo.textureHeight = atoi(getPartValue(part).c_str());
                    continue;
                }
                if (key =="pages" )
                {
                    m_fontInfo.pages = atoi(getPartValue(part).c_str());
                    continue;
                }
                if (key =="packed" )
                {
                    m_fontInfo.packed = getPartValue(part) == "1";
                    continue;
                }
            }
        }

        void BitmapFontSet::parsePageLine(const std::string & pageLine)
        {
            std::string part;
            std::stringstream lineSS(pageLine);
            while (std::getline(lineSS, part, ' '))
            {
                 std::string key = getPartKey(part); trim(key);
                if (key =="id" )
                {
                    // TODO pages in future 
                    continue;
                }
                if (key =="file" )
                {
                    // TODO pages in future
                    continue;
                }
            }
        }

        void BitmapFontSet::parseCharLine(const std::string & charLine)
        {
            std::string part;
            std::stringstream lineSS(charLine);
            Glyph g;
            while (std::getline(lineSS, part, ' '))
            {
                std::string key = getPartKey(part); trim(key);
                if (key =="id" )
                {
                    
                    g.c = static_cast<wchar_t> (atoi(getPartValue(part).c_str()));
                    
                    if(atoi(getPartValue(part).c_str()) == 32)
                    {
                        LOG_DEBUG("Space is: ", g.c, " char of it:", text::wc2c(g.c),"asdasd" );
                    }
                    continue;
                }
                if (key =="x" )
                {
                    g.x = atoi(getPartValue(part).c_str());
                    continue;
                }
                if (key =="y" )
                {
                    g.y = atoi(getPartValue(part).c_str());
                    continue;
                }
                 if (key =="width" )
                {
                    g.width = atoi(getPartValue(part).c_str());
                    continue;
                }
                if (key =="height" )
                {
                    g.height = atoi(getPartValue(part).c_str());
                    continue;
                }
                if (key =="xoffset" )
                {
                    g.xoffset = atoi(getPartValue(part).c_str());
                    continue;
                }
                if (key =="yoffset" )
                {
                    g.yoffset = atoi(getPartValue(part).c_str());
                    continue;
                }
                if (key =="xadvance" )
                {
                    g.advance = atoi(getPartValue(part).c_str());
                    continue;
                }
                if (key =="page" )
                {
                    g.page = atoi(getPartValue(part).c_str());
                    continue;
                }
                if (key =="chnl" )
                {
                    g.chnl = atoi(getPartValue(part).c_str());
                    continue;
                }
            }
            m_glyphs[g.c] = g;
        }

        void BitmapFontSet::parseKerningLine(const std::string & kerningLine)
        {
            std::string part;
            std::stringstream lineSS(kerningLine);
            KerningPair kp;
            float kern= 0.0f;
            while (std::getline(lineSS, part, ' '))
            {
                std::string key = getPartKey(part); trim(key);
                if (key =="first" )
                {
                    kp.first = static_cast<wchar_t> (atoi(getPartValue(part).c_str()));
                }
                if (key =="second" )
                {
                    kp.second = static_cast<wchar_t> (atoi(getPartValue(part).c_str()));
                }
                if (key =="amount" )
                {
                    kern = atoi(getPartValue(part).c_str());
                }
            }
            if(kern != 0.0f)
                m_kernings[kp] = kern;
        }
        
        std::string BitmapFontSet::getPartKey(const std::string & part)
        {
            // eg. part = "advance=12", we wanna 12 as float
            std::stringstream partSS;
            partSS << part;
            std::string value;
            std::getline(partSS, value, '='); // advance
            return value;
        }

        std::string BitmapFontSet::getPartValue(const std::string & part)
        {
            // eg. part = "advance=12", we wanna 12 as float
            std::stringstream partSS;
            partSS << part;
            std::string value;
            std::getline(partSS, value, '='); // advance
            if (!std::getline(partSS, value, '=')) // 12
                value = "";
            
            return value;
        }

        std::vector<std::string> BitmapFontSet::getCommaSeparatedVector(const std::string& commaSepVecString)
        {
            std::vector<std::string> result;
            std::stringstream vecSS;
            vecSS << commaSepVecString;
            std::string value;
            while (std::getline(vecSS, value, ','))
            {
                result.push_back(value);
            }
            return result;
        }
    }
}

