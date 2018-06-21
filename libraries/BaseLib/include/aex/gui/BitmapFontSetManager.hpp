#ifndef  BITMAP_FONT_SET_MANAGER_AEX_GUI_HEADER
#define  BITMAP_FONT_SET_MANAGER_AEX_GUI_HEADER

#include "BitmapFontSet.hpp"

#include <aex/Logger.hpp>
#include <aex/UniversalManager.hpp>


namespace aex {
    namespace gui {

        /**
         * @brief Resource manager for bitmap fonts.
         * 
         * This is an example usage of BitMapFontSetManager:
         * \n
         * @include gui/fonts/bitmap_fonts/bitmap_font.cpp
         * \n
         * <em>Full example at gui/fonts/bitmap_fonts</em>
         */
        class BitmapFontSetManager {
            protected:

                class Allocator : public std::allocator<BitmapFontSet> {
                    public:
                        Allocator();

                        Allocator(const std::string& txtFilePath,
                                const std::string& bitmapFilePath);

                        BitmapFontSet* allocate(std::size_t n);
                        void deallocate( BitmapFontSet* p, std::size_t n );
                        void construct(BitmapFontSet* obj);
                    protected:
                        std::string m_txtFilePath;
                        std::string m_bitmapFilePath;
                };

            public:
                using manager_t = aex::Manager<BitmapFontSet, Allocator>;
                using handle_t = manager_t::uh_type;

            public:
                /**
                 * @brief Get a handle to BitmapFontSet.
                 *
                 * @details Returns an unique handle to a shared BitmapFontSet
                 * for a pair of file paths txtFilePath and bitmapFilePath.
                 */
                static handle_t get(const std::string& txtFilePath, const std::string& bitmapFilePath) ;
        };

        inline BitmapFontSetManager::Allocator::Allocator(){}

        inline BitmapFontSetManager::Allocator::Allocator(const std::string& txtFilePath,
                const std::string& bitmapFilePath):
            m_txtFilePath(txtFilePath),
            m_bitmapFilePath(bitmapFilePath){}

        inline BitmapFontSet* BitmapFontSetManager::Allocator::allocate(std::size_t n) {
            return std::allocator<BitmapFontSet>::allocate(n);
        }

        inline void BitmapFontSetManager::Allocator::deallocate( BitmapFontSet* p,
                std::size_t n )
        {
            std::allocator<BitmapFontSet>::deallocate(p, n);
        }

        inline void BitmapFontSetManager::Allocator::construct(BitmapFontSet* obj) {
            new(obj) BitmapFontSet(m_txtFilePath, m_bitmapFilePath); // invoke constructor on clean allocated object
        }
    }
}

#endif
