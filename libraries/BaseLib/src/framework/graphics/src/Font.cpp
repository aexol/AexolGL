#include <aex/Font.hpp>
#include <aex/Texture.hpp>
#include <aex/TextureManager.hpp>
#include <aex/SubsystemGuard.hpp>
#include <aex/ShaderProgram.hpp>
#include <aex/VertexData.hpp>
#include <algorithm>
#include <future>
#include "../../include/internal_sdl.hpp"
#include <unordered_map>
#include <deque>
#include <locale>
#include <aex/Exceptions.hpp>
#include <clocale>
#include <cstdlib>
#include <string>

/* SDL interprets each pixel as a 32-bit number, so our masks must depend
   on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
Uint32 RMASK = 0xff000000;
Uint32 GMASK = 0x00ff0000;
Uint32 BMASK = 0x0000ff00;
Uint32 AMASK = 0x000000ff;
#else
Uint32 RMASK = 0x000000ff;
Uint32 GMASK = 0x0000ff00;
Uint32 BMASK = 0x00ff0000;
Uint32 AMASK = 0xff000000;
#endif

Uint32 rgbaToInt32(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    Uint32 color = 0;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    color |= (r & 255) << 24;
    color |= (g & 255) << 16;
    color |= (b & 255) << 8;
    color |= (a & 255);
#else
    color |= (a & 255) << 24;
    color |= (b & 255) << 16;
    color |= (g & 255) << 8;
    color |= (r & 255);
#endif
    return color;
}

// utf8 string
char* convertWStringToCharPtr(const std::wstring& input)
{
    size_t outputSize = input.length()*4 + 1; // +1 for null terminator
    char* outputString = new char[outputSize];
    const wchar_t * inputW = input.c_str();
    //size_t bytesWrited = std::wcstombs(outputString, inputW, outputSize);
    std::wcstombs(outputString, inputW, outputSize);
    return outputString;
}

Uint16* convertWStringToUnicodeTab(const std::wstring& input)
{
    size_t outputSize = input.length() + 1;
    Uint16* output = new Uint16[outputSize];
    const wchar_t* inputStr = input.c_str();
    for (size_t i = 0; i < input.length(); i++)
    {
        output[i] = inputStr[i]; // casting
    }
    output[input.length()] = '\0';
    return output;
}


void getTTFSize(TTF_Font* font, const aex::wstring& text, int* retWidth, int* retHeight)
{
    //  char* cStr = convertWStringToCharPtr(strBuff);
    //  TTF_SizeUTF8(font, cStr , &firstWidth, &firstHeight);
    //  delete [] cStr;
    Uint16* uStr = convertWStringToUnicodeTab(text);
    TTF_SizeUNICODE(font, uStr, retWidth, retHeight);
    delete[] uStr;
}

SDL_Surface* renderTTFSolid(TTF_Font* font, const aex::wstring& text, SDL_Color& color){
    Uint16* utext = convertWStringToUnicodeTab(text);
    SDL_Surface* retSurface = TTF_RenderUNICODE_Solid(font, utext, color);
    delete[] utext;
    return retSurface;
}

SDL_Surface* renderTTFBlended(TTF_Font* font, const aex::wstring& text, SDL_Color& color){
    Uint16* utext = convertWStringToUnicodeTab(text);
    SDL_Surface* retSurface = TTF_RenderUNICODE_Blended(font, utext, color);
    delete[] utext;
    return retSurface;
}

SDL_Surface* renderTTFShaded(TTF_Font* font, const aex::wstring& text, SDL_Color& color, SDL_Color& background){
    Uint16* utext = convertWStringToUnicodeTab(text);
    SDL_Surface* retSurface = TTF_RenderUNICODE_Shaded(font, utext, color, background);
    delete[] utext;
    return retSurface;
}


namespace aex
{
    //    GlyphChar::GlyphChar(char c, FBox2D data, float kernAdvance):
    //    m_metrics{0,0,0,0,0},
    //    uvData(data),
    //    size{(uvData[1][0]-uvData[0][0])*(uvData[1][1]-uvData[0][1])},
    //    m_kernAdvance(kernAdvance),
    //    glyph(c)
    //    {
    //    }


    typedef IPoint3D Indices;
    template<typename T, size_t DIM> using Square = std::array<std::array<T, DIM>, 4>;

    using FSquare3D = Square<float, 3>;
    using FSquare2D = Square<float, 2>;

    template<size_t size>
    using IndicesArray = std::array<std::array<uint32_t, 3>, size>;

    //    static CONSTEXPR FSquare3D VerticesBase =
    //    {
    //        {
    //            {{-1.0f, 1.0f, 1.0f}},  //Top left
    //            {{1.0f, 1.0f, 1.0f}},   //Top right
    //            {{1.0f, -1.0f, 1.0f}},  //Bottom right
    //            {{-1.0f, -1.0f, 1.0f}}  //Bottom left
    //        }
    //    };
    //    static CONSTEXPR FSquare2D UVBase =
    //    {
    //        {
    //            {{0.0f, 1.0f}}, //Top left
    //            {{1.0f, 1.0f}}, //Top right
    //            {{1.0f, 0.0f}}, //Bottom right
    //            {{0.0f, 0.0f}}  //Bottom left
    //        }
    //    };
    //
    //    static CONSTEXPR IndicesArray<2> squareIndices =
    //    {
    //        {
    //            {{0, 1, 3}},
    //            {{1, 2, 3}}
    //        }
    //    };

    static std::mutex TTF_mutex;

    struct TextureAtlasNode
    {
        std::unique_ptr<TextureAtlasNode> m_left;
        std::unique_ptr<TextureAtlasNode> m_right;
        IBox2D rect;
        bool m_used;

        TextureAtlasNode* insert(const IBox2D& box)
        {
            TextureAtlasNode* newNode;

            if (m_left)
            {
                if (!(newNode = m_left->insert(box)))
                {
                    newNode = m_right->insert(box);
                }

                return newNode;
            }

            if (m_used)
            {
                return nullptr;
            }

            if (box[1][0] > rect[1][0] || box[1][1] > rect[1][1])
            {
                return nullptr;
            }

            if (box[1][0] == rect[1][0] && box[1][1] == rect[1][1])
            {
                m_used = true;
                return this;
            }

            auto dw = rect[1][0] - box[1][0];
            auto dh = rect[1][1] - box[1][1];

            if (dw > dh)
            {
                //Vertical split
                m_left.reset(new TextureAtlasNode{
                    nullptr,
                    nullptr,
                    {
                        IPoint2D
                        {rect[0][0], rect[0][1]},
                        IPoint2D
                        {box[1][0], rect[1][1]}
                    },
                    false
                }
                );
                m_right.reset(new TextureAtlasNode{
                    nullptr,
                    nullptr,
                    {
                        IPoint2D
                        {rect[0][0] + box[1][0] + 1, rect[0][1]},
                        IPoint2D
                        {rect[1][0] - box[1][0] - 1, rect[1][1]}
                    },
                    false
                }
                );
            } else
            {
                //Horizontal split
                m_left.reset(new TextureAtlasNode{
                    nullptr,
                    nullptr,
                    {
                        IPoint2D
                        {rect[0][0], rect[0][1]},
                        IPoint2D
                        {rect[1][0], box[1][1]}
                    },
                    false
                }
                );
                m_right.reset(new TextureAtlasNode{
                    nullptr,
                    nullptr,
                    {
                        IPoint2D
                        {rect[0][0], rect[0][1] + box[1][1] + 1},
                        IPoint2D
                        {rect[1][0], rect[1][1] - box[1][1] - 1}
                    },
                    false
                }
                );
            }

            return m_left->insert(box);
        }
        //        void setBox(int x, int y, int w, int h){ rect = {x, y, w, h};}
        //        const IBox2D& getBox(){return rect;}
        //        TextureAtlasNode* left(){ return m_left.get(); }
        //        TextureAtlasNode* right(){ return m_right.get(); }

        void clear()
        {
            if (m_left)
            {
                m_left->clear();
                m_right->clear();
                m_left.reset();
                m_left.reset();
            }
        }
    };

    SDL_Rect* IBox2DToSDLRect(const IBox2D& box)
    {
        SDL_Rect* rect = new SDL_Rect{};
        rect->x = box[0][0];
        rect->y = box[0][1];
        rect->w = box[1][0];
        rect->h = box[1][1];
        return rect;
    }

    static bool IsMappable(wchar_t c)
    {
        // static std::locale loc;
        // return /*!std::isspace(c) &&*/ !std::iscntrl(c, loc); // check is control character (DEL, PG UP itp);
        return true;
    }

    struct RemoveNotMappable
    {

        bool operator()(wchar_t c)
        {
            return IsMappable(c);
        }
    };
    //    struct FindTextureNode
    //    {
    //        bool operator()(const TextureAtlasNode& lhs, const TextureAtlasNode& rhs)
    //        {
    //            return &lhs == &rhs;
    //        }
    //    };
    //    struct DeleteSDLSurface
    //    {
    //        void operator()(SDL_Surface* surface)
    //        {
    //            SDL_FreeSurface(surface);
    //        }
    //    };
    //    class TextureAtlasTooSmall : public std::exception
    //    {
    //    public:
    //        const char* what() const NOEXCEPT
    //        {
    //            return "TextureAtlas is to small to fit texture";
    //        }
    //    };
    //    template<class node_type = TextureAtlasNode, class Key = aex::string,class node_find = FindTextureNode>
    //    class TextureAtlas
    //    {
    //        struct RGBMask
    //        {
    //            uint32_t rmask;
    //            uint32_t gmask;
    //            uint32_t bmask;
    //            uint32_t amask;
    //        };
    //#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    //        static CONSTEXPR RGBMask RGBA = {0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff};
    //#else
    //        static CONSTEXPR RGBMask RGBA = {0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000};
    //#endif
    //        static CONSTEXPR RGBMask RGB565 = {0xf800, 0x07E0, 0x001F, 0x0000};
    //        static CONSTEXPR RGBMask RGBA5551 = {0xf800, 0x07C0, 0x003E, 0x0001};
    //        static CONSTEXPR RGBMask RGBA4 = {0xf000, 0x0f00, 0x00f0, 0x000f};
    //        static CONSTEXPR RGBMask ONE_CHANNEL = {0x0, 0x0, 0x0, 0x0};
    //    public:
    //        enum class RGBType
    //        {
    //            RGBA,
    //            RGB565,
    //            RGBA5551,
    //            RGBA4,
    //            ONE_CHANNEL
    //        };
    //        typedef node_type value_type;
    //        typedef Key key_type;
    //        TextureAtlas(size_t width, size_t height) :
    //        m_width(width),
    //        m_height(height),
    //        m_mask(RGBA),
    ////        m_surface(nullptr, DeleteSDLSurface{}),
    //        m_root(new value_type)
    //        {
    ////            SDL_Surface* surface = SDL_CreateRGBSurface(0, m_width, m_height, depth(), m_mask.rmask, m_mask.gmask, m_mask.bmask, m_mask.amask);
    ////            m_surface.reset(surface);
    //            m_root.setBox(0, 0, m_width, m_height);
    //        }
    //        TextureAtlas(size_t width, size_t height, RGBType rgb) :
    //        m_width(width),
    //        m_height(height),
    ////        m_surface(nullptr, DeleteSDLSurface{}),
    //        m_root(new value_type)
    //        {
    ////            switch(rgb)
    ////            {
    ////                case RGBType::RGBA:
    ////                {
    ////                    m_mask = RGBA;
    ////                    break;
    ////                }
    ////                case RGBType::RGB565:
    ////                {
    ////                    m_mask = RGB565;
    ////                    break;
    ////                }
    ////                case RGBType::RGBA5551:
    ////                {
    ////                    m_mask = RGBA5551;
    ////                    break;
    ////                }
    ////                case RGBType::RGBA4:
    ////                {
    ////                    m_mask = RGBA4;
    ////                    break;
    ////                }
    ////            }
    //
    ////            SDL_Surface* surface = SDL_CreateRGBSurface(0, m_width, m_height, depth(), m_mask.rmask, m_mask.gmask, m_mask.bmask, m_mask.amask);
    ////            m_surface.reset(surface);
    //            m_root.setBox(0, 0, m_width, m_height);
    //        }
    //        value_type& insert(const key_type& key, SDL_Surface* newTexture)
    //        {
    //            auto dw = newTexture->w;
    //            auto dh = newTexture->h;
    //            if(!m_root.left())
    //            {
    //                if(newTexture->w >= newTexture->h)
    //                {
    //                    IBox2D boxLeft{0, 0,dw, dh};
    //                    IBox2D boxRight{dw+1, 0, m_width - dw - 1, dh};
    //                } else
    //                {
    //
    //                }
    //            }
    //        }
    //    private:
    //        void insert_impl(std::unique_ptr<value_type>& node,const key_type& key, SDL_Surface* newTexture)
    //        {
    //
    //        }
    //        uint32_t depth()
    //        {
    //            uint32_t retDepth = 0;
    //            uint32_t masks = m_mask.rmask | m_mask.gmask | m_mask.bmask | m_mask.amask;
    //            if(masks == 0) return 8;
    //            while(masks)
    //            {
    //                ++retDepth;
    //                masks >>= 1;
    //            }
    //            return retDepth;
    //        }
    //        size_t m_width;
    //        size_t m_height;
    //        RGBMask m_mask;
    ////        std::unique_ptr<SDL_Surface, DeleteSDLSurface> m_surface;
    //        std::unique_ptr<value_type> m_root;
    //        std::unordered_map<key_type, value_type*> m_textures;
    //    };

    struct GlyphMetricWithSize
    {
        GlyphCharMetrics metrics;
        size_t size;
    };

    struct smaller_glyph
    {
        bool operator()(const GlyphMetricWithSize& lhs, const GlyphMetricWithSize& rhs);
    };

    bool
    smaller_glyph::operator()(const GlyphMetricWithSize& lhs, const GlyphMetricWithSize& rhs)
    {
        return lhs.size < rhs.size || lhs.metrics.glyph < rhs.metrics.glyph;
    }

    bool operator==(const GlyphChar& lhs, const GlyphChar& rhs)
    {
        return lhs.m_metrics.glyph == rhs.m_metrics.glyph &&
                lhs.m_metrics.m_maxx == rhs.m_metrics.m_maxx &&
                lhs.m_metrics.m_maxy == rhs.m_metrics.m_maxy &&
                lhs.m_metrics.m_minx == rhs.m_metrics.m_minx &&
                lhs.m_metrics.m_miny == rhs.m_metrics.m_miny;
    }

    GlyphString::GlyphString()
    {
    }

    GlyphString::~GlyphString()
    {
    }

    GlyphString::iterator
    GlyphString::begin()
    {
        return m_uvCoords.begin();
    }

    GlyphString::iterator
    GlyphString::end()
    {
        return m_uvCoords.end();
    }
    //    GlyphString::const_iterator
    //    GlyphString::begin() const
    //    {
    //        return m_uvCoords.begin();
    //    }
    //    GlyphString::const_iterator
    //    GlyphString::end() const
    //    {
    //        return m_uvCoords.end();
    //    }
    //
    //    GlyphString::const_iterator
    //    GlyphString::cbegin() const
    //    {
    //        return m_uvCoords.cbegin();
    //    }
    //    GlyphString::const_iterator
    //    GlyphString::cend() const
    //    {
    //        return m_uvCoords.cend();
    //    }

    //FontMap implementation

    FontMap::FontMap() :
    m_backingBitmap
    {
        nullptr
    },
    m_font{nullptr}

    ,
    m_used(false)
    {
    }

    FontMap::FontMap(const aex::string& texName, const CharSet& map, SDL_Surface* bitmap, int fontH, int fontLineSkip, int ascent, int descent, Font* f, TTF_FONT_TEX_CHANNELS chnnls) :
    m_backingBitmap
    {
        new BackingBitmap{bitmap, chnnls}
    },
    m_texName{texName},
    m_font{f},
    m_uvMappings{map},
    m_fontHeight{fontH},
    m_fontLineSkip{fontLineSkip},
    m_ascent{ascent},
    m_descent{descent}

    ,
    m_used(false),
    m_channels(chnnls)
    {
        attachToFont();
    }

    FontMap::FontMap(const FontMap& map) :
    m_backingBitmap
    {
        map.m_backingBitmap
    },
    m_texName{map.m_texName},
    m_font{map.m_font},
    m_uvMappings{map.m_uvMappings},
    m_fontHeight{map.m_fontHeight},
    m_fontLineSkip{map.m_fontLineSkip},
    m_ascent{map.m_ascent},
    m_descent{map.m_descent},
    m_used{map.m_used},
    m_channels{map.m_channels}
    {
        attachToFont();
    }

    FontMap& FontMap::operator=(const FontMap& map)
    {
        /*
         if(m_used)
        {
            auto& tManager = TextureManager::GetInstance();
            tManager.ReleaseTexture(m_texName);
        }
         */
        m_backingBitmap = map.m_backingBitmap;
        m_texName = map.m_texName;
        m_font = map.m_font;
        m_uvMappings = map.m_uvMappings;
        m_fontHeight = map.m_fontHeight;
        m_fontLineSkip = map.m_fontLineSkip;
        m_ascent = map.m_ascent;
        m_descent = map.m_descent;
        m_used = map.m_used;
        m_channels = map.m_channels;
        attachToFont();
        return *this;
    }

    FontMap::~FontMap()
    {
        if (m_used)
        {
            /*
            m_fontConnection.disconnect();
            auto& tManager = TextureManager::GetInstance();
            tManager.ReleaseTexture(m_texName);
             */

        }
    }

    FontMap::BackingBitmap::BackingBitmap(SDL_Surface* bitmap, TTF_FONT_TEX_CHANNELS channels) :
    m_texName
    {
    },
    m_build{false},
    m_sizeX{static_cast<uint32_t> (bitmap->w)},
    m_sizeY{static_cast<uint32_t> (bitmap->h)},
    m_channels(channels)
    {
        int alignmentOffset = 0;
        if (m_channels == TTF_FONT_TEX_CHANNELS::RGBA)
        {
            alignmentOffset = 0;
            m_bitmap = (void*) new uint32_t[m_sizeX * m_sizeY];
        }

        if (m_channels == TTF_FONT_TEX_CHANNELS::ALPHA)
        {
            alignmentOffset = (4 - m_sizeX % 4) % 4;
            m_bitmap = (void*) new uint8_t[(m_sizeX + alignmentOffset) * m_sizeY];
        }

        int bpp = bitmap->format->BytesPerPixel;
        for (uint32_t y = 0; y < m_sizeY; ++y)
        {
            //auto dstRow = (m_sizeX + alignmentOffset) * y;
            auto dstRow = (m_sizeX + alignmentOffset) * y;
            auto srcRow = bitmap->pitch * y;

            for (uint32_t x = 0; x < m_sizeX; ++x)
            {
                int dstCol, srcCol;
                srcCol = x * bpp;
                dstCol = x;
                Uint8* p = (Uint8*) bitmap->pixels + srcRow + srcCol;
                uint8_t r, g, b, a;
                SDL_GetRGBA(*((Uint32*) p), bitmap->format, &r, &g, &b, &a);
                uint32_t color = rgbaToInt32(r, r, r, r); // all red because there is only a gray, alpha must be setted for FontEffects

                if (m_channels == TTF_FONT_TEX_CHANNELS::RGBA)
                    ((uint32_t*) m_bitmap)[dstRow + dstCol] = color;
                if (m_channels == TTF_FONT_TEX_CHANNELS::ALPHA)
                    ((uint8_t*) m_bitmap)[dstRow + dstCol] = r;

                //m_bitmap[dstRow + dstCol] = color[((uint8_t*)bitmap->pixels)[srcRow + srcCol]].r;
            }
        }
    }

    FontMap::BackingBitmap::~BackingBitmap()
    {
        if (!m_build)
        {
            if (m_channels == TTF_FONT_TEX_CHANNELS::RGBA)
            {
                delete [] (uint32_t*) m_bitmap;
            }
            if (m_channels == TTF_FONT_TEX_CHANNELS::ALPHA)
            {
                delete[] (uint8_t*) m_bitmap;
            }
        } else
        {
          //  ((std::string*)nullptr)->empty();
            auto& tManager = TextureManager::GetInstance();
            tManager.releaseTexture(m_texName);
        }
    }

    bool
    FontMap::BackingBitmap::isBuild() const
    {
        return m_build;
    }

    void
    FontMap::BackingBitmap::build(const aex::string& texName)
    {
        auto& tManager = TextureManager::GetInstance();
        std::unique_lock<std::mutex> lock{m_lock};

        if (m_build || !m_bitmap)
        {
            return;
        }

        if (m_bitmap)
        {
#if __TARGET_AEX_API__ == __GLES__ || __TARGET_AEX_API__ == __GL__
            TextureConfig cfg = TextureConfig::makeDefaultTextureConfig();
            cfg.setMinFilterLinear();
            cfg.setMagFilterLinear();
            cfg.setWrapSClamToEdge();
            cfg.setWrapTClamToEdge();

            if (m_channels == TTF_FONT_TEX_CHANNELS::ALPHA)
                cfg.setAlphaChannelFormat();

            auto tex = tManager.getEmptyTexture(texName, cfg);
            m_texName = texName;
            int unpackOld;

            GL_CHECK(glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpackOld));
            GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));
            //            GL_CHECK( glBindTexture(GL_TEXTURE_2D, tex->getId()) );
            //            GL_CHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
            //            GL_CHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
            //            GL_CHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
            //            GL_CHECK( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
            //            GL_CHECK( glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, m_sizeX, m_sizeY,
            //                                       0, GL_ALPHA, GL_UNSIGNED_BYTE, m_bitmap) );
            tex->setData(m_bitmap, m_sizeX, m_sizeY);
            GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, unpackOld));
#endif
            if (m_channels == TTF_FONT_TEX_CHANNELS::RGBA)
            {
                delete [] (uint32_t*) m_bitmap;
            }
            if (m_channels == TTF_FONT_TEX_CHANNELS::ALPHA)
            {
                delete[] (uint8_t*) m_bitmap;
            }
            m_bitmap = nullptr;
            lock.unlock();
            m_build = true;

        } else
        {
            /* TO-DO, error */
        }
    }

    UIPoint2D
    FontMap::BackingBitmap::mapSize() const
    {
        UIPoint2D ret{m_sizeX, m_sizeY};
        return ret;
    }

    UIPoint2D
    FontMap::mapSize() const
    {
        if (!m_backingBitmap) return UIPoint2D(0, 0);

        return m_backingBitmap->mapSize();
    }

    bool
    FontMap::spitOutText(const aex::wstring& text, GlyphString& dest, bool genKernMap) const
    {
        dest.m_uvCoords.clear();
        dest.m_uvCoords.reserve(text.size());
        auto prev = text.end();
        auto end = text.end();

        for (auto iter = text.begin(); iter != end; ++iter)
        {
            auto cIter = m_uvMappings.find(*iter);

            if (cIter == m_uvMappings.end() && IsMappable(*iter))
            {
                return false;
            }

            if (!genKernMap)
            {
                dest.m_uvCoords.push_back(cIter->second);
            } else if (prev != end && *iter != ' ' && *prev != ' ' && *iter != '\n' && *prev != '\n')
            {
                const auto& glyph = cIter->second;
                int kernAdvance = 0;

                if (m_font)
                {
                    kernAdvance = m_font->kernDistance(*prev, *iter);
                }

                dest.m_uvCoords.push_back({
                    glyph.m_metrics,
                    glyph.uvData,
                    glyph.rectOnTexture,
                    kernAdvance
                }
                );
            } else
            {
                if (*iter == '\n')
                {
                    dest.m_uvCoords.push_back({
                        { 0, 0, 0, 0, 0, '\n'},
                        FBox2D
                        {FPoint2D(0.0f, 0.0f), FPoint2D(0.0f, 0.0f)},
                        IBox2D(),
                        0
                    });
                } else
                {
                    dest.m_uvCoords.push_back(cIter->second);
                }
            }

            prev = iter;
        }

        return true;
    }

    /*float
    FontMap::GetLenght(const aex::string &text);
    for (auto iter = text.begin(); iter != end; ++iter) {
              auto cIter = m_uvMappings.find(*iter);
              if (cIter == m_uvMappings.end() && IsMappable(*iter))
       }
    }
     */
    bool
    FontMap::contains(const aex::wstring& text) const
    {
        auto end = text.end();

        for (auto iter = text.begin(); iter != end; ++iter)
        {
            auto cIter = m_uvMappings.find(*iter);

            if (cIter == m_uvMappings.end() && IsMappable(*iter))
            {
                return false;
            }
        }

        return true;
    }

    const aex::string&
    FontMap::backingTexture()
    {
        if (!m_backingBitmap)
        {
            return m_texName;
        }

        if (!m_used)
        {
            m_used = true;

            //            auto& tManager = TextureManager::GetInstance();
            //            tManager.GetEmptyTexture(m_texName);
            if (!m_backingBitmap->isBuild())
            {
                m_backingBitmap->build(m_texName);
            }
        }

        return m_texName;
    }

    void
    FontMap::onFontDeath()
    {
        m_fontConnection.disconnect();
        m_font = nullptr;
    }

    void
    FontMap::attachToFont()
    {
        if (m_font)
        {
            m_fontConnection = m_font->attachLifetimeListener(std::bind(&FontMap::onFontDeath, this));
        }
    }

    bool operator<(const FontMap& lhs, const FontMap& rhs)
    {
        return lhs.m_uvMappings.size() < rhs.m_uvMappings.size();
    }

    //    SDL_Texture*
    //    FontMap::blitOnScreen(SDL_Renderer* renderer)
    //    {
    //        if(m_backingBitmap)
    //        {
    //            SDL_Surface* surface = m_backingBitmap->surf();
    //            if(surface)
    //            {
    //                return SDL_CreateTextureFromSurface(renderer, surface);
    //            }
    //        }
    //        return nullptr;
    //    }

    Font::FontSettings::FontSettings() :
    genKernMap(true),
    channels(TTF_FONT_TEX_CHANNELS::ALPHA),
    glyphShape(GLYPH_SHAPE::SHADED)
    {
    }

    Font::FontSettings::FontSettings(bool genKernMap, TTF_FONT_TEX_CHANNELS channels, GLYPH_SHAPE shape) :
    genKernMap(genKernMap),
    channels(channels),
    glyphShape(shape)
    {
    }

    //Font implementation

    Font::Font(const aex::string& fontFileName, int ptsize, bool withKernTable) :
    Font(fontFileName, ptsize, FontSettings(withKernTable,TTF_FONT_TEX_CHANNELS::ALPHA, GLYPH_SHAPE::SHADED))
    {

    }

    Font::Font(const aex::string& fontFileName, int ptsize, const FontSettings& fontSettings) :
    m_fontName
    {
        fontFileName
    },
    m_mappedChars{},
    m_ptsize{ptsize},
    m_generateCount{0}

    ,
    m_fontSettings(fontSettings),
    m_ctxDirtyFlag(false)
    {
    }

    Font::~Font()
    {
        for (auto& t : m_workers)
        {
            t.join();
        }

        m_death();
    }

    FontMap
    Font::fontMap()
    {
        std::unique_lock<std::mutex> workersLock{m_workersLock};
        // wait to finish job
        for (auto& t : m_workers)
        {
            t.join();
        }
        m_workers.clear();

        std::unique_lock<std::mutex> lock{m_accessLock};
        return m_currentMap;
    }
    std::locale loc;

    void
    Font::requestMapUpdate(const aex::wstring& text)
    {
        std::set<wchar_t> newRequest;

        for (auto c : text)
        {
            newRequest.insert(c);
        }

        m_pendingRequests.push(newRequest);
        buildMap();
    }

    void Font::setContextDirtyFlag(bool flag)
    {
      m_ctxDirtyFlag = flag;
      if(m_ctxDirtyFlag)
        m_currentMap.m_uvMappings.clear();
    }

    boost::signals2::connection
    Font::attachLifetimeListener(const aex::function<void ()>& f)
    {
        return m_death.connect(f);
    }

    boost::signals2::connection
    Font::attachDisplayText(const aex::function<void(const FontMap&) >& f)
    {
        return m_sig.connect(f);
    }

    int
    Font::kernDistance(wchar_t first, wchar_t second)
    {
        auto kernDist = m_kerningPairs.get(std::make_pair(first, second));
        int retVal = 0;

        if (m_fontSettings.genKernMap && kernDist)
        {
            retVal = kernDist->second;
        }

        return retVal;
    }

    void
    Font::buildMap()
    {
        std::set<wchar_t> requstsUnique;
        std::set<wchar_t> pendingRequest;
        std::unique_lock<std::mutex> lock{m_mapBuildLock};

        //Are there any requests pending for new characters to be added?
        if (m_pendingRequests.pop(pendingRequest))
        {
            std::copy(pendingRequest.begin(), pendingRequest.end(),
                    std::inserter(requstsUnique, requstsUnique.begin()));

            //While we are at it we might as well check if there are any more requests and handle them all at once
            while (m_pendingRequests.pop(pendingRequest))
            {
                std::copy(pendingRequest.begin(), pendingRequest.end(),
                        std::inserter(requstsUnique, requstsUnique.begin()));
            }

            std::unique_lock<std::mutex> accessLock{m_accessLock};
            auto size = m_mappedChars.size();

            std::copy_if(
                    requstsUnique.begin(), requstsUnique.end(),
                    std::inserter(m_mappedChars, m_mappedChars.begin()),
                    RemoveNotMappable
            {}
            );

            //New characters, generate bitmap
            if ((size != m_mappedChars.size()))
            {
                m_ctxDirtyFlag = false;
                makeBitmap();
            }
        }

        if(m_ctxDirtyFlag){
          m_ctxDirtyFlag = false;
          makeBitmap();
        }
    }

    float
    Font::stringLenght(const aex::wstring& str)
    {
        float ret = 0.0f;

        std::unique_lock<std::mutex> lock(TTF_mutex);
        SubsystemGuard sg(aex::FontSubsystem);
        sg.init();

        FileManager fManager;
        typedef boost::filesystem::path path;
        path p = fManager.work_dir();
        p /= path{m_fontName};
        TTF_Font* font = TTF_OpenFont(p.string().c_str(), m_ptsize);

        // should be conv to const char* utf8 repr using wctombs than method TTF_SizeUTF8,but this method is not important now so:
        aex::string quickfixVal = aex::string(str.begin(), str.end());
        int w, h;
        if (font && TTF_SizeText(font, quickfixVal.c_str(), &w, &h) == 0)
        {
            ret = (float) w;
        }

        float pixelSize = 2.0f / (float) m_ptsize;
        TTF_CloseFont(font);
        lock.unlock();
        return ret * pixelSize;
    }

    float Font::stringLenght(const aex::string& str)
    {
        return stringLenght( aex::wstring(str.begin(), str.end()) );
    }

    void
    Font::makeBitmap()
    {
        auto genCount = m_generateCount++;

        //Start a new thread to render a font bitmap.
        std::unique_lock<std::mutex> workersLock{m_workersLock};
        m_workers.push_back(
            Thread{[this](aex::string texName, int ptsize, int generateCount, std::set<wchar_t> charsToRender)
            {
                FontMap::CharSet charSet;
                SubsystemGuard sg(aex::FontSubsystem);
                sg.init();

                //Open font file
                FileManager fManager;
                typedef boost::filesystem::path path;
                path p = fManager.work_dir();
                p /= path{texName};
                std::unique_lock<std::mutex> lock(TTF_mutex);
                TTF_Font* font = TTF_OpenFont(p.string().c_str(), ptsize);

                //Check if font opened
                if (!font)
                {
                    LOG_ERR("Can't open ttf font at path: " + p.string());
                    throw aex::FileNotFoundError("Can't open ttf font at path: " + p.string());
                    return;
                }

                auto ascent = TTF_FontAscent(font);
                auto descent = TTF_FontDescent(font);
                auto fontHeight = TTF_FontHeight(font);
                auto fontLineSkip = TTF_FontLineSkip(font);
                lock.unlock();

                std::set<GlyphMetricWithSize, smaller_glyph> metricsSet;
                size_t avg_size = 0;

                //Load necessary metrics for chars
                for (auto c : charsToRender)
                {
                    GlyphMetricWithSize glyphInfo;
                    glyphInfo.metrics.glyph = c;
                    lock.lock();

                    if (TTF_GlyphMetrics(font, glyphInfo.metrics.glyph, &glyphInfo.metrics.m_minx, &glyphInfo.metrics.m_maxx, &glyphInfo.metrics.m_miny, &glyphInfo.metrics.m_maxy, &glyphInfo.metrics.advance) == -1)
                    {
                        throw aex::TTFError("Error when loading glyph metrics using method: TTF_GlyphMetrics");
                    }

                    lock.unlock();
                            size_t sizeOfGlyph;

                    if ((sizeOfGlyph = (glyphInfo.metrics.m_maxx - glyphInfo.metrics.m_minx) * (glyphInfo.metrics.m_maxy - glyphInfo.metrics.m_miny)) != 0)
                    {
                        //Predict atlas size
                        avg_size += sizeOfGlyph;
                        glyphInfo.size = sizeOfGlyph;
                    }

                    metricsSet.insert(glyphInfo);
                }

                //Size prediction may use some polish
                avg_size = SDL_sqrt(avg_size);
                        TextureAtlasNode root{
                    nullptr,
                    nullptr,
                    {
                        IPoint2D(0, 0),
                        IPoint2D(avg_size, avg_size)
                    },
                    false
                };

                auto rIter = metricsSet.rbegin();
                auto rend = metricsSet.rend();

                //Build atlas tree resizing if necessary
                while (rIter != rend)
                {
                    auto& glyph = *rIter;
                        TextureAtlasNode* node = nullptr;
                        IBox2D minimalCharBox{
                        IPoint2D
                        {0, 0},
                        IPoint2D
                        {glyph.metrics.m_maxx - glyph.metrics.m_minx, glyph.metrics.m_maxy - glyph.metrics.m_miny}
                    };

                    if (minimalCharBox[1][0] == 0 || minimalCharBox[1][1] == 0)
                    {
                        rIter++;
                        continue;
                    }

                    //Restart if failed
                    if (!(node = root.insert(minimalCharBox)))
                    {
                        if (root.rect[1][0] < root.rect[1][1])
                        {
                            root.rect[1][0] += minimalCharBox[1][0];
                        } else
                        {
                            root.rect[1][1] += minimalCharBox[1][1];
                        }

                        root.clear();
                        rIter = metricsSet.rbegin();
                        continue;
                    }

                    rIter++;
                }

                root.clear();

                //Create surface that will be used for bitmap storage
                SDL_Surface* bitmap = SDL_CreateRGBSurface(0, root.rect[1][0], root.rect[1][1], 32, 0, 0, 0, 0);
                SDL_Color white = {255, 255, 255};
                SDL_Color black = {0, 0, 0};

                GlyphCharMetrics spaceMetrics;
                        spaceMetrics.glyph = ' ';
                if (TTF_GlyphMetrics(font, spaceMetrics.glyph, &spaceMetrics.m_minx, &spaceMetrics.m_maxx, &spaceMetrics.m_miny, &spaceMetrics.m_maxy, &spaceMetrics.advance) == -1)
                        spaceMetrics = GlyphCharMetrics();

                        //Start blitting each glyph into an atlas.
                    for (rIter = metricsSet.rbegin(); rIter != rend; ++rIter)
                    {
                        auto& glyph = *rIter;
                                TextureAtlasNode* node = nullptr;

                                //Crop glyph size, stripping wasted space.
                                IBox2D minimalCharBox{
                            IPoint2D
                            {0, 0},
                            IPoint2D
                            {glyph.metrics.m_maxx - glyph.metrics.m_minx, glyph.metrics.m_maxy - glyph.metrics.m_miny}
                        };

                        /*//If either size of glyph is 0 then most likely it was a whitespace, carry on.
                        if(minimalCharBox[1][0]==0 || minimalCharBox[1][1]==0)
                            continue;
                         */
                        if (!(node = root.insert(minimalCharBox)))
                        {
                            //Size was adjusted, we really should never be here,
                            LOG_DEBUG("ASSERT INVOKED ON GLYPH GENERATION: ", glyph.metrics.glyph, " as: ",(char)glyph.metrics.glyph);
                            continue;
                        }

                        lock.lock();
                        SDL_Surface* glyphSurface;
                        aex::wstring text;
                        text += ' ';
                        text += glyph.metrics.glyph;

                        switch (m_fontSettings.glyphShape)
                        {
                            case GLYPH_SHAPE::SOLID:
                                glyphSurface = renderTTFSolid(font, text, white);
                                break;
                            case GLYPH_SHAPE::BLENDED:
                                glyphSurface = renderTTFBlended(font,text,white);
                                break;
                            case GLYPH_SHAPE::SHADED:
                            default:
                            {
                                glyphSurface = renderTTFShaded(font,text,white,black);
                            }
                                break;
                        }
                                lock.unlock();
                                //Find glyph in surface.
                                auto minx = glyph.metrics.m_minx + spaceMetrics.advance;
                                auto miny = ascent - glyph.metrics.m_maxy;

                                auto srcrect = IBox2DToSDLRect(minimalCharBox);
                                srcrect->x = minx;
                                srcrect->y = miny;
                                auto dstrect = IBox2DToSDLRect(node->rect); // here is box description (x, y, width, height)

                                //Blit glyph into an atlas.
                                SDL_BlitSurface(
                                glyphSurface, srcrect,
                                bitmap, dstrect
                                );
                                SDL_FreeSurface(glyphSurface);
                                delete srcrect;
                                delete dstrect;

                                //Calculate UVs for added glyph
                                float top_u = ((float) node->rect[0][0]) / ((float) bitmap->w);
                                float top_v = ((float) node->rect[0][1]) / ((float) bitmap->h);
                                float bottom_u = top_u + ((float) node->rect[1][0]) / ((float) bitmap->w);
                                float bottom_v = top_v + ((float) node->rect[1][1] - 1) / ((float) bitmap->h);

                                charSet[glyph.metrics.glyph] = GlyphChar{
                            glyph.metrics,
                            {
                                FPoint2D(top_u, top_v),
                                FPoint2D(bottom_u, bottom_v)
                            },
                            node->rect,
                            0
                        };
                    }

                if (m_fontSettings.genKernMap)
                {
                    for (const std::pair<const wchar_t, aex::GlyphChar>& p : charSet)
                    {
                        wchar_t c = p.second.m_metrics.glyph;
                        for (const std::pair<const wchar_t, aex::GlyphChar>& p2 : charSet)
                        {
                            wchar_t cPair = p2.second.m_metrics.glyph;
                            using value_type = typename PairsTable::value_type;
                            m_kerningPairs.insert_with(
                                std::make_pair(c, cPair),
                                [&lock, &font](value_type& v)
                                {
                                    auto c = v.first.first;
                                    auto cPair = v.first.second;
                                    int width, widthKern;
                                    int height;
                                    wchar_t pair[3] = {c, cPair, '\0'};

                                    lock.lock();
                                    aex::wstring strBuff = aex::wstring(pair);
                                    TTF_SetFontKerning(font, 0);
                                    getTTFSize(font, strBuff, &width, &height);
                                    TTF_SetFontKerning(font, 1);
                                    getTTFSize(font,
                                            strBuff,
                                            &widthKern,
                                            &height);
                                    int kerning = widthKern - width;
                                    lock.unlock();
                                    v.second = kerning;
                                }
                            );
                        }
                    }
                }

                lock.lock();
                TTF_CloseFont(font);
                lock.unlock();
                //Create new FontMap
                std::stringstream s;
                s << texName << '_' << ptsize << '_' << generateCount << m_fontSettings.getKeyForTexName();
                FontMap fMap(s.str(), charSet, bitmap, fontHeight, fontLineSkip, ascent, descent, this, m_fontSettings.channels);
                SDL_FreeSurface(bitmap);
                //Setup 'newest' fontMap as the most current one.
                m_pendingMaps.push(fMap);

                while (m_pendingMaps.pop(fMap))
                {
                    std::unique_lock<std::mutex> accessLock{m_accessLock};

                    // it is for multithreading generation, only one thread have to switch member obj
                    if (m_currentMap < fMap)
                    {
                        m_currentMap = fMap;
                        m_sig(fMap);
                    }
                }
            }, "nn", m_fontName, m_ptsize, genCount, m_mappedChars});
    }

    //Display text implementation

    DisplayText::DisplayText() : m_font(nullptr), m_map(), m_text(""), m_ready(false), m_lineWrap(false)
    {
    }

    DisplayText::DisplayText(const aex::string& text, Font& font) :
    m_font(&font),
    m_map(font.fontMap()),
    m_text(text),
    m_connection(font.attachDisplayText(std::bind(&DisplayText::notifyFontMapUpdate, this, std::placeholders::_1))),
    m_ready(false),
    m_lineWrap(false)
    {
        if (!m_map.contains(aex::wstring(m_text.begin(), m_text.end())))
        {
            font.requestMapUpdate(aex::wstring(m_text.begin(), m_text.end()));
        }

        auto charCount = m_text.size();
        m_uvAttrib.reserve(charCount * 4);
        m_positionAttrib.reserve(charCount * 4);
        m_uvAttrib.reserve(charCount * 4);
        m_indices.reserve(charCount * 2);
        m_width = font.stringLenght(aex::wstring(m_text.begin(), m_text.end()));
    }

    DisplayText::DisplayText(const DisplayText& dt) : DisplayText(dt.m_text, *dt.m_font)
    {
    }

    DisplayText&
            DisplayText::operator=(const DisplayText& dt)
    {
        clearBuffers();
        releaseTexture();
        m_font = dt.m_font;
        m_map = m_font->fontMap();
        m_text = dt.m_text;
        m_connection = m_font->attachDisplayText(std::bind(&DisplayText::notifyFontMapUpdate, this, std::placeholders::_1));
        m_ready = false;
        m_lineWrap = dt.m_lineWrap;
        if (!m_map.contains(aex::wstring(m_text.begin(), m_text.end())))
        {
            m_font->requestMapUpdate(aex::wstring(m_text.begin(), m_text.end()));
        }
        auto charCount = m_text.size();
        m_uvAttrib.reserve(charCount * 4);
        m_positionAttrib.reserve(charCount * 4);
        m_uvAttrib.reserve(charCount * 4);
        m_indices.reserve(charCount * 2);
        m_width = m_font->stringLenght(aex::wstring(m_text.begin(), m_text.end()));
        return *this;
    }

    DisplayText::~DisplayText()
    {
        clearBuffers();
        releaseTexture();
    }

    void
    DisplayText::notifyFontMapUpdate(const aex::FontMap& map)
    {
        m_pendingMaps.push(map);
    }

    GlyphString
    DisplayText::getString()
    {
        updateMap();
        return m_glyph;
    }

    float
    DisplayText::computeWidth()
    {
        float ret = 0.0f;

        return ret;
    }

    float
    DisplayText::getWidth()
    {
        return m_width;
    }

    bool
    DisplayText::updateMap()
    {
        FontMap map;

        if (m_pendingMaps.pop(map))
        {
            //New map, clean up old buffers.
            clearBuffers();

            //Iterate over to most recent map.
            if (m_map < map)
            {
                m_map = map;
            }

            while (m_pendingMaps.pop(map))
            {
                if (m_map < map)
                {
                    m_map = map;
                }
            }

            m_ready = false;
        }

        bool retVal = m_ready;

        if (!retVal)
        {
            retVal = m_map.spitOutText(aex::wstring(m_text.begin(), m_text.end()), m_glyph);
            //Map is ready, prepare new draw buffers.
            if (retVal)
            {
                genBuffers();
                buildTextBatch();
                uploadBuffers();
                acquireTexture();
            }
        }

        return m_ready = retVal;
    }

    Texture* DisplayText::getFontTexture()
    {
        return TextureManager::GetInstance().getEmptyTexture(m_fontBitmapName);
    }

    void
    DisplayText::clearBuffers()
    {
        if (m_ready)
        {
            m_vertexVbo = VertexData();
            m_uvVbo = VertexData();
            m_indexVbo = IndexData();
        }
    }

    void
    DisplayText::genBuffers()
    {
    }

    void
    DisplayText::acquireTexture()
    {
        auto& tManager = TextureManager::GetInstance();

        if (!m_fontBitmapName.empty())
        {
            tManager.releaseTexture(m_fontBitmapName);
        }

        m_fontBitmapName = m_map.backingTexture();
        tManager.getEmptyTexture(m_fontBitmapName);
    }

    void
    DisplayText::releaseTexture()
    {
        auto& tManager = TextureManager::GetInstance();

        if (!m_fontBitmapName.empty())
        {
            tManager.releaseTexture(m_fontBitmapName);
        }

        m_fontBitmapName = "";
    }

    void
    DisplayText::buildTextBatch()
    {
        auto lineHeight = m_map.height();
        auto ascent = m_map.ascent();
        auto descent = m_map.descent();
        float pixelSize = 1.0f / (float) lineHeight;
        float accumHorizontalOffset = 0.0f;
        float accumVerticalOffset = 0.0f;
        size_t squareIdx = 0;
        m_positionAttrib.clear();
        m_uvAttrib.clear();
        m_indices.clear();
        wchar_t prev = '\0';
        bool breakWord = false;
        static std::locale loc;

        for (auto glyph : m_glyph)
        {
            if (m_lineWrap)
            {
                if (prev == '\0' || std::isspace(prev, loc))
                {
                    float wordSize = 0.0f;
                    auto tmpGlyph = std::find(m_glyph.begin(), m_glyph.end(), glyph);

                    while (tmpGlyph != m_glyph.end() &&
                            !std::isspace(tmpGlyph->m_metrics.glyph, loc) &&
                            wordSize < m_lineWidth)
                    {
                        wordSize += glyph.m_metrics.advance * pixelSize;
                        tmpGlyph++;
                    }

                    if (wordSize > m_lineWidth)
                    {
                        breakWord = true;
                    } else if (wordSize + accumHorizontalOffset * pixelSize > m_lineWidth)
                    {
                        accumVerticalOffset -= lineHeight;
                        accumHorizontalOffset = 0.0f;
                    }
                }
            }

            UNUSED(breakWord);

            if (glyph.m_metrics.m_minx == 0 && glyph.m_metrics.m_maxx == 0 && glyph.m_metrics.m_miny == 0 && glyph.m_metrics.m_maxy == 0)
            {
                if (glyph.m_metrics.advance != 0)
                {
                    //Whitespace character
                    accumHorizontalOffset += glyph.m_metrics.advance;
                    prev = ' ';
                    continue;
                } else if (glyph.m_metrics.glyph == '\n')
                {
                    accumVerticalOffset -= lineHeight;
                    accumHorizontalOffset = 0.0f;
                    continue;
                }
            }

            //Build positions.
            Poistion upperLeft;
            Poistion upperRigth;
            Poistion bottomLeft;
            Poistion bottomRigth;
            upperLeft[0] = upperRigth[0] = bottomLeft[0] = bottomRigth[0] = -1.0f;
            upperLeft[1] = upperRigth[1] = bottomLeft[1] = bottomRigth[1] = -1.0f;
            upperLeft[2] = upperRigth[2] = bottomLeft[2] = bottomRigth[2] = 0.0f;
            upperLeft[0] += (accumHorizontalOffset + glyph.m_metrics.m_minx/*+glyph.m_kernAdvance*/) * pixelSize;
            bottomLeft[0] += (accumHorizontalOffset + glyph.m_metrics.m_minx/*+glyph.m_kernAdvance*/) * pixelSize;
            upperLeft[1] += (accumVerticalOffset + lineHeight - ascent + glyph.m_metrics.m_maxy) * pixelSize;
            upperRigth[1] += (accumVerticalOffset + lineHeight - ascent + glyph.m_metrics.m_maxy) * pixelSize;
            upperRigth[0] += (accumHorizontalOffset + glyph.m_metrics.m_maxx/*+glyph.m_kernAdvance*/) * pixelSize;
            bottomRigth[0] += (accumHorizontalOffset + glyph.m_metrics.m_maxx/*+glyph.m_kernAdvance*/) * pixelSize;
            bottomLeft[1] += (accumVerticalOffset + -descent + glyph.m_metrics.m_miny) * pixelSize;
            bottomRigth[1] += (accumVerticalOffset + -descent + glyph.m_metrics.m_miny) * pixelSize;
            accumHorizontalOffset += glyph.m_metrics.advance/*+glyph.m_kernAdvance*/;


            m_positionAttrib.push_back(upperLeft);
            m_positionAttrib.push_back(upperRigth);
            m_positionAttrib.push_back(bottomLeft);
            m_positionAttrib.push_back(bottomRigth);

            //Build UVs
            UVCoords upperLeftUV;
            UVCoords upperRigthUV;
            UVCoords bottomLeftUV;
            UVCoords bottomRigthUV;
            upperLeftUV[0] = glyph.uvData[0][0];
            upperLeftUV[1] = glyph.uvData[0][1];
            upperRigthUV[0] = glyph.uvData[1][0];
            upperRigthUV[1] = glyph.uvData[0][1];
            bottomLeftUV[0] = glyph.uvData[0][0];
            bottomLeftUV[1] = glyph.uvData[1][1];
            bottomRigthUV[0] = glyph.uvData[1][0];
            bottomRigthUV[1] = glyph.uvData[1][1];

            m_uvAttrib.push_back(upperLeftUV);
            m_uvAttrib.push_back(upperRigthUV);
            m_uvAttrib.push_back(bottomLeftUV);
            m_uvAttrib.push_back(bottomRigthUV);

            auto indexOffset = squareIdx * 4;
            Indices firstTriangle(indexOffset + 0, indexOffset + 1, indexOffset + 2);
            Indices secondTriangle(indexOffset + 1, indexOffset + 3, indexOffset + 2);

            m_indices.push_back(firstTriangle);
            m_indices.push_back(secondTriangle);


            ++squareIdx;
            prev = glyph.m_metrics.glyph;
        }
    }

    void
    DisplayText::draw(AexRenderer* gl)
    {
        if (gl && updateMap())
        {
            const ShaderDrw* shader = gl->getShader();

            if (shader)
            {
                const AexShader& program = shader->GetProgram();
                program->setVertexData(m_uvVbo);
                program->setVertexData(m_vertexVbo);
                program->setIndices(m_indexVbo);
            }
        }
    }

    void
    DisplayText::finishdraw(AexRenderer* gl)
    {
        if (gl && updateMap())
        {
            auto& tManager = TextureManager::GetInstance();
            auto tex = tManager.getEmptyTexture(m_fontBitmapName);
            tex->unbind();
            const ShaderDrw* shader = gl->getShader();

            if (shader)
            {
                const AexShader& program = shader->GetProgram();
                program->releaseVertexData(m_uvVbo);
                program->releaseVertexData(m_vertexVbo);
            }

            tManager.releaseTexture(m_fontBitmapName);
        }
    }

    void
    DisplayText::needsUpdate()
    {

    }

    void
    DisplayText::uploadBuffers()
    {
        m_vertexVbo = *VertexData::createVertexData(m_positionAttrib, "gl_Vertex");
        m_uvVbo = *VertexData::createVertexData(m_uvAttrib, "gl_TexCoord");
        m_indexVbo = IndexData::createTriangles(m_indices);
    }

    void
    DisplayText::setSampler(AexRenderer* gl, UniformSampler& sampler)
    {
        if (updateMap())
        {
            const ShaderDrw* shader = gl->getShader();

            if (shader)
            {
                const AexShader& program = shader->GetProgram();
                auto& tManager = TextureManager::GetInstance();
                auto tex = tManager.getEmptyTexture(m_fontBitmapName);
                tex->bind();
                sampler.setValue(tex);
                sampler.setUniform(program);
                tManager.releaseTexture(m_fontBitmapName);
            }
        }
    }

    void
    DisplayText::enableLineWrap(float width)
    {
        m_lineWrap = true;
        m_lineWidth = width;
    }

    void
    DisplayText::disableLineWrap()
    {
        m_lineWrap = false;
    }

    bool
    DisplayText::isReady()
    {
        updateMap();
        return m_ready;
    }

    Matrix
    DisplayText::scale(float scale)
    {
        std::array<float, 16> scaleText //Completly unnecessary but wth.
        {
            {
                scale, 0.0f, 0.0f, scale - 1.0f,
                0.0f, scale, 0.0f, scale - 1.0f,
                0.0f, 0.0f, scale, scale - 1.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            }
        };
        Matrix m{scaleText};
        return m;
    }
}
