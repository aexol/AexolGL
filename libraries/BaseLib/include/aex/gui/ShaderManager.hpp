#ifndef SHADER_MANAGER_AEX_GUI_HEADER
#define SHADER_MANAGER_AEX_GUI_HEADER

#include "GUIMath.hpp"

#include <aex/ShaderDrw.hpp>
#include <aex/ShaderProgram.hpp>
#include <aex/UniversalManager.hpp>

namespace aex {
    namespace gui {

        class ShaderManager {
        protected:

            class Allocator : public std::allocator<ShaderDrw> {
            public:

                Allocator() {
                }

                Allocator(const std::string& vsh, const std::string& fsh, const std::string& attribs) :
                m_vsh(vsh),
                m_fsh(fsh),
                m_attribs(attribs) {
                }

                void construct(ShaderDrw* obj) {
                    new(obj) ShaderDrw(m_vsh.c_str(), m_fsh.c_str(), m_attribs.c_str()); // invoke constructor on clean allocated object
                }

            protected:
                std::string m_vsh;
                std::string m_fsh;
                std::string m_attribs;
            };

        public:
            using manager_t = aex::Manager<ShaderDrw, Allocator>;
            using handle_t = manager_t::uh_type;
        public:
            ShaderManager();
            virtual ~ShaderManager();
            static handle_t get(const std::string& name, const char* vsh, const char* fsh, const char* attribs);
        };
    }
}


#endif
