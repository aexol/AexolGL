#ifndef _AEX_FONT_HPP
#define _AEX_FONT_HPP

#include <aex/details/cds_helpers.hpp>
#include <aex/Point.hpp>
#include <aex/Box.hpp>
#include <aex/AexDrawable.hpp>
#include <aex/AWindow.hpp>
#include <aex/VertexData.hpp>
#include <aex/Thread.hpp>
#include <aex/ShaderDrw.hpp>
#include <cds/container/michael_list_hp.h>
#include <cds/container/split_list_map.h>
#include <cds/container/msqueue.h>
#include <cds/algo/atomic.h>
#include <boost/signals2/signal.hpp>
#include <type_traits>
#include <vector>
#include <map>
#include <set>
#include <exception>
#include <mutex>

struct AEX_PUBLIC_API SDL_Surface;

namespace aex {
    namespace _font_detail {
        namespace cc = ::cds::container;
        template<class K, class D, class Hash = std::hash<D>, class Less = std::less<D>>
        struct _map_type {
            struct map_traits : public cc::split_list::traits {
                using ordered_list = cc::michael_list_tag;
                using hash = Hash;
                using bit_reversal = aex::_cds_details::lookup;
                //using hash = kern_hasher<kern_key_pair>;
                struct ordered_list_traits : public cc::michael_list::traits {
                    using less = Less;
                };
            };
            using type = cc::SplitListMap<
                cds::gc::HP,
                K,
                D,
                map_traits
            >;
        };

        template<class K, class D, class Hash = std::hash<D>, class Less = std::less<D>>
        using map_type = typename _map_type<K, D, Hash, Less>::type;

        template<class T>
        using queue_type = cds::container::MSQueue<cds::gc::HP,
            T,
            typename cds::container::msqueue::make_traits<
                cds::opt::stat<cds::container::msqueue::stat<>>,
                cds::opt::item_counter<cds::atomicity::item_counter>
            >::type
        >;
    }

    static CONSTEXPR uint32_t LTRLayout = 1;
    static CONSTEXPR uint32_t RTLLayout = LTRLayout << 1;
    static CONSTEXPR uint32_t VerticalLayout = LTRLayout << 2;
    class VertexData;
    class IndexData;
    class FontMap;
    class Font;
    class DisplayText;

    enum TTF_FONT_TEX_CHANNELS {
        ALPHA = 0,
        RGBA
    };

    AEX_PUBLIC_API bool operator<(const FontMap& lhs, const FontMap& rhs);

    struct AEX_PUBLIC_API GlyphCharMetrics {
        int m_minx;
        int m_maxx;
        int m_miny;
        int m_maxy;
        int advance;
        wchar_t glyph;
    };
    class AexRenderer;

    struct AEX_PUBLIC_API GlyphChar {
        GlyphCharMetrics m_metrics;
        FBox2D uvData;
        IBox2D rectOnTexture;
        int m_kernAdvance;
    };

    class AEX_PUBLIC_API GlyphStringIterator {
        typedef std::vector<GlyphChar> GlyphArray;
        typedef typename GlyphArray::iterator iterator;
    public:


    };
    AEX_PUBLIC_API bool operator==(const GlyphChar& lhs, const GlyphChar& rhs);

    class AEX_PUBLIC_API GlyphString {
        typedef std::vector<GlyphChar> GlyphArray;
    public:
        typedef typename GlyphArray::iterator iterator;
        typedef typename GlyphArray::const_iterator const_iterator;

        GlyphString();
        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
        const_iterator cbegin() const;
        const_iterator cend() const;
        ~GlyphString();
    private:
        GlyphArray m_uvCoords;
        friend class FontMap;
    };

    class AEX_PUBLIC_API FontMap {
    public:
        typedef std::map<wchar_t, GlyphChar> CharSet;
    public:
        FontMap();
        FontMap(const FontMap& map);
        FontMap& operator=(const FontMap& map);
        //TODO: remove
        //SDL_Texture* blitOnScreen(SDL_Renderer* renderer);
        ~FontMap();
        UIPoint2D mapSize() const;
        bool spitOutText(const aex::wstring& text, GlyphString& dest, bool withKern = true) const;
        bool contains(const aex::wstring& text) const;
        const aex::string& backingTexture();

        int ascent() const {
            return m_ascent;
        }

        int descent() const {
            return m_descent;
        }

        int height() const {
            return m_fontHeight;
        }

        int lineSkip() const {
            return m_fontLineSkip;
        }

        // nullptr if not founded

        const GlyphChar* getGlyph(wchar_t c) const {
            if (m_uvMappings.find(c) != m_uvMappings.end()) {
                return &m_uvMappings.at(c);
            }

            return nullptr;
        }

    private:
        FontMap(const aex::string& texName, const CharSet& map, SDL_Surface* bitmap, int fontH, int fontLineSkip, int ascent, int descent, Font* f, TTF_FONT_TEX_CHANNELS chnnls);
        void onFontDeath();
        void attachToFont();

        class AEX_PUBLIC_API BackingBitmap {
        public:
            BackingBitmap(SDL_Surface* bitmap, TTF_FONT_TEX_CHANNELS channels);
            ~BackingBitmap();
            bool isBuild() const;
            void build(const aex::string& texName);
            UIPoint2D mapSize() const;
            //TODO: remove

            uint32_t* surf32() {
                return (uint32_t*) m_bitmap;
            }

            uint8_t* surf8() {
                return (uint8_t*) m_bitmap;
            }

            uint32_t sizeX() const {
                return m_sizeX;
            }

            uint32_t sizeY() const {
                return m_sizeY;
            }

        private:
            aex::string m_texName;

            void* m_bitmap;

            bool m_build;
            uint32_t m_sizeX;
            uint32_t m_sizeY;
            TTF_FONT_TEX_CHANNELS m_channels;
            std::mutex m_lock;
        };

        std::shared_ptr<BackingBitmap> m_backingBitmap;
        aex::string m_texName;
        Font* m_font;
        boost::signals2::scoped_connection m_fontConnection;
        //Profile for memory usage
        CharSet m_uvMappings;
        int m_fontHeight;
        int m_fontLineSkip;
        int m_ascent;
        int m_descent;
        bool m_used;
        TTF_FONT_TEX_CHANNELS m_channels;
        friend bool operator<(const FontMap& lhs, const FontMap& rhs);
        friend class Font;
    };

    class AEX_PUBLIC_API Font {
    public:

        enum GLYPH_SHAPE {
            SOLID = 0,
            BLENDED,
            SHADED
        };

        struct FontSettings {
            FontSettings();
            FontSettings(bool genKernMap, TTF_FONT_TEX_CHANNELS channels, GLYPH_SHAPE shape);

            std::string getKeyForTexName() const
            {
                return "" + aex::to_string((int)channels) + "_" + aex::to_string((int)glyphShape);
            }

            bool genKernMap;
            TTF_FONT_TEX_CHANNELS channels;
            GLYPH_SHAPE glyphShape;
        };
    public:
        Font(const aex::string& fontFileName, int ptsize, bool withKernTable = true);
        Font(const aex::string& fontFileName, int ptsize, const FontSettings& fontSettings = FontSettings());
        Font(Font&& f);
        ~Font();
        FontMap fontMap();
        void requestMapUpdate(const aex::wstring& text);
        void setContextDirtyFlag(bool flag);
        int kernDistance(wchar_t first, wchar_t second);
        float stringLenght(const aex::wstring& str);
        float stringLenght(const aex::string& str);
    protected:
        using char_type = wchar_t;
        using kern_key_pair = std::pair<char_type, char_type>;
        boost::signals2::connection attachLifetimeListener(const aex::function<void()>& f);

        struct AEX_PUBLIC_API kern_hasher {

            size_t operator()(const kern_key_pair& kernPair) const {
                std::hash<char_type> charHasher;
                return charHasher(kernPair.first) ^ charHasher(kernPair.second);
            }
        };

        struct AEX_PUBLIC_API kern_pair_less {
            bool operator() (const kern_key_pair& lhs, const kern_key_pair& rhs) const {
                if(lhs.first == rhs.first){
                    return lhs.second < rhs.second;
                }
                return lhs.first < rhs.second;
            }
        };
        boost::signals2::connection attachDisplayText(const aex::function<void(const FontMap&) >& f);
    private:
        using PairsTable = _font_detail::map_type<kern_key_pair,
              int,
              kern_hasher,
              kern_pair_less>;
        template<class T>
        using queue_type = _font_detail::queue_type<T>;
        void buildMap();
        void makeBitmap();
        void swapFontMap(const FontMap& font);

        aex::string m_fontName;
        std::set<char_type> m_mappedChars;
        FontMap m_currentMap;
        boost::signals2::signal<void(const FontMap&) > m_sig;
        boost::signals2::signal<void() > m_death;
        queue_type<std::set<char_type>> m_pendingRequests;
        queue_type<FontMap> m_pendingMaps;
        PairsTable m_kerningPairs;
        std::mutex m_accessLock;
        std::mutex m_workersLock;
        std::mutex m_mapBuildLock;
        std::list<Thread> m_workers;
        int m_ptsize;
        int m_generateCount;
        FontSettings m_fontSettings;
        SubsystemGuard m_sg;
        bool m_ctxDirtyFlag;
        friend class DisplayText;
        friend class FontMap;
    };

    typedef aex::shared_ptr<Font> Font_ptr;

    class AEX_PUBLIC_API DisplayText : public DrawObject {
        typedef FPoint3D Poistion;
        typedef FPoint2D UVCoords;
        typedef SPoint3D Indices;
        typedef std::vector<Poistion> PoistionArray;
        typedef std::vector<UVCoords> UVCoordsArray;
        typedef std::vector<Indices> IndicesArray;
    public:
        DisplayText();
        DisplayText(const DisplayText& dt);
        DisplayText& operator=(const DisplayText& dt);
        DisplayText(const aex::string& text, Font& font);
        ~DisplayText();
        GlyphString getString();

        virtual void draw(AexRenderer* gl);
        virtual void finishdraw(AexRenderer* gl);
        void needsUpdate();
        void setSampler(AexRenderer* gl, UniformSampler& sampler);
        void enableLineWrap(float width);
        void disableLineWrap();
        bool isReady();
        Matrix scale(float scale);
        float computeWidth();
        float getWidth();
        bool updateMap();
        Texture* getFontTexture();
    protected:
        void notifyFontMapUpdate(const FontMap& map);
    private:
        template<class T>
        using queue_type = _font_detail::queue_type<T>;
        Font* m_font;
        FontMap m_map;
        aex::string m_text;
        GlyphString m_glyph;
        boost::signals2::scoped_connection m_connection;
        queue_type<FontMap> m_pendingMaps;
        PoistionArray m_positionAttrib;
        UVCoordsArray m_uvAttrib;
        IndicesArray m_indices;
        VertexData m_vertexVbo;
        VertexData m_uvVbo;
        IndexData m_indexVbo;
        aex::string m_fontBitmapName;
        bool m_ready;
        bool m_lineWrap;
        float m_lineWidth;
        float m_width = 0.0f;
        friend class Font;
        void clearBuffers();
        void genBuffers();
        void buildTextBatch();
        void uploadBuffers();
        void acquireTexture();
        void releaseTexture();
    };
}

#endif
