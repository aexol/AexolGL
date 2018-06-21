#ifndef TEXTURE_MANAGER_AEX_GUI_HEADER
#define TEXTURE_MANAGER_AEX_GUI_HEADER

#include <aex/UniversalManager.hpp>

#include <aex/Logger.hpp>
#include <aex/Texture.hpp>

#include <aex/Common.hpp>
#include <aex/FileManager.hpp>
#include <vector>

namespace aex {
    namespace gui {

        class TextureManager {
        protected:

            class Allocator : public std::allocator<Texture> {
            public:

                Allocator() {
                }

                Allocator(const std::string& path, TextureConfig cfg) :
                m_path(path),
                m_cfg(cfg) {
                }

                Texture* allocate(std::size_t n) {
                    g_counter++;
                    LOG_DEBUG(__FILE__, ":" , __LINE__,": Allocating Texture: ", g_counter);
                    return std::allocator<Texture>::allocate(n);
                }

                void deallocate( Texture* p, std::size_t n )
                {
                    g_counter--;
                    LOG_DEBUG(__FILE__, ":" , __LINE__,": Deallocating Texture num: ", g_counter);
                    std::allocator<Texture>::deallocate(p, n);
                }

                void construct(Texture* obj) {
                    new(obj) Texture(m_path, m_cfg); // invoke constructor on clean allocated object
                }

            protected:
                std::string m_path;
                aex::TextureConfig m_cfg;
                static long long g_counter;
            };
        public:
            using manager_t = aex::Manager<Texture, Allocator>;
            using handle_t = manager_t::uh_type;
        public:
            TextureManager();
            virtual ~TextureManager();

            static handle_t get(const std::string& path, TextureConfig cfg);
        };
    }
}

#endif
