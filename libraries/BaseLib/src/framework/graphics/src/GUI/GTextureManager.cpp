#include "aex/gui/TextureManager.hpp"

namespace aex
{
    namespace gui
    {

        long long TextureManager::Allocator::g_counter = 0;



        TextureManager::TextureManager()
        {
        }

        TextureManager::~TextureManager()
        {
        }

        TextureManager::handle_t TextureManager::get(const std::string& path, TextureConfig cfg)
        {
            static TextureManager::manager_t g_manager;
            return g_manager.GetResourceHandle(path, Allocator(path, cfg));
        }
    }
}
