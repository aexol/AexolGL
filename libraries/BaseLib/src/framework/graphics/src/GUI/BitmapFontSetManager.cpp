#include "aex/gui/BitmapFontSetManager.hpp"

namespace aex
{
    namespace gui
    {
        BitmapFontSetManager::handle_t BitmapFontSetManager::get(const std::string& txtFilePath, const std::string& bitmapFilePath)
        {
            static BitmapFontSetManager::manager_t g_manager;
            return g_manager.GetResourceHandle(txtFilePath, BitmapFontSetManager::Allocator(txtFilePath, bitmapFilePath));
        }
    }
}
