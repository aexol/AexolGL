#ifndef HUD_UTILS_AEX_GUI_HEADER
#define HUD_UTILS_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include <aex/Hud.hpp>
#include <aex/HudElement.hpp>
#include <aex/Texture.hpp>
 

namespace aex {
    namespace gui {
        
        class AEX_PUBLIC_API HudUtils {
        public:
            static aex::Hud_ptr getHud(const std::string& textureMapPath, const std::string& jsonPath, const aex::TextureConfig& cfg);
            static aex::TextureConfig getLinearTCFG();
            static aex::TextureConfig getNearestTCFG();
        };
    }
}

#endif