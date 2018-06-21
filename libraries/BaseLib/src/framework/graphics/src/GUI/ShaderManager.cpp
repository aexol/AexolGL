#include "aex/gui/ShaderManager.hpp"

namespace aex
{
    namespace gui
    {
        ShaderManager::ShaderManager()
        {
        }

        ShaderManager::~ShaderManager()
        {
        }

        ShaderManager::handle_t ShaderManager::get(const std::string& name, const char* vsh, const char* fsh,  const char* attribs)
        {
            static ShaderManager::manager_t g_manager;
            return g_manager.GetResourceHandle(name, Allocator(vsh, fsh, attribs));
        }
    }
}
