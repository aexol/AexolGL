#include "aex/gui/TrueTypeFontSetManager.hpp"

namespace aex
{
    namespace gui
    {

        TrueTypeFontSetManager::TrueTypeFontSetManager()
        {
        }

        TrueTypeFontSetManager::~TrueTypeFontSetManager()
        {
        }

        TrueTypeFontSetManager::handle_t TrueTypeFontSetManager::get(const std::string& fontPath, int fontSizePt, const FontSettings& settings)
        {
            static TrueTypeFontSetManager::manager_t g_manager;
            std::stringstream ss;
            ss << fontPath << aex::to_string(fontSizePt) << settings;
            std::string key = ss.str();
            return g_manager.GetResourceHandle(key, TrueTypeFontSetManager::Allocator(fontPath, fontSizePt, settings));
        }
    }
}
