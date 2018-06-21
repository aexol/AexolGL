#ifndef TRUETYPE_FONT_SET_MANAGER_AEX_GUI_HEADER
#define TRUETYPE_FONT_SET_MANAGER_AEX_GUI_HEADER

#include "TrueTypeFontSet.hpp"

#include <aex/Logger.hpp>
#include <aex/UniversalManager.hpp>

namespace aex {
    namespace gui {

        class TrueTypeFontSetManager {
        protected:

            class Allocator : public std::allocator<TrueTypeFontSet> {
            public:
                Allocator() {
                }

                Allocator(const std::string& fontPath, int fontSizePt, const FontSettings& settings) :
                m_fontPath(fontPath),
                m_fontSizePt(fontSizePt),
                m_settings(settings)
                {
                }

                TrueTypeFontSet* allocate(std::size_t n) {
                    return std::allocator<TrueTypeFontSet>::allocate(n);
                }

                void deallocate( TrueTypeFontSet* p, std::size_t n )
                {
                    std::allocator<TrueTypeFontSet>::deallocate(p, n);
                }

                void construct(TrueTypeFontSet* obj) {
                    new(obj) TrueTypeFontSet(m_fontPath, m_fontSizePt, m_settings); // invoke constructor on clean allocated object
                }

            protected:
                std::string m_fontPath;
                int m_fontSizePt;
                FontSettings m_settings;
            };

        public:
            using manager_t = aex::Manager<TrueTypeFontSet, Allocator>;
            using handle_t = manager_t::uh_type;

        public:
            TrueTypeFontSetManager();
            virtual ~TrueTypeFontSetManager();
            static handle_t get(const std::string& fontPath, int fontSizePt, const FontSettings& settings) ;
        };
    }
}

#endif
