#include "aex/gui/HudUtils.hpp"
#include "aex/gui/Frame.hpp"
#include <aex/Texture.hpp>

namespace aex
{
    namespace gui
    {
        aex::Hud_ptr HudUtils::getHud(const std::string& textureMapPath, const std::string& jsonPath, const aex::TextureConfig& cfg)
        {
            //aex::Hud_ptr hud = aex::make_shared<aex::Hud>("Data/hrm-vertical.png", cfg);
            aex::Hud_ptr hud = aex::make_shared<aex::Hud>(textureMapPath, cfg);
            hud->setQuatCamera(Frame::instance()->getRenderTreeRoot()->getQuatCamera());
            hud->setCanChangeBlendState(true);
            hud->readHudFromJson(jsonPath);
            hud->setShader(*aex::ShaderDrw::BasicHudElementShaderOrthoOffCenterAtlas());
            return hud;
        }

        aex::TextureConfig HudUtils::getLinearTCFG()
        {
            aex::TextureConfig cfg = aex::TextureConfig::makeDefaultTextureConfig();
            cfg.setMagFilterLinear();
            cfg.setMinFilterLinearMipmapLinear();
            return cfg;
        }

        aex::TextureConfig HudUtils::getNearestTCFG()
        {
            aex::TextureConfig cfg = aex::TextureConfig::makeDefaultTextureConfig();
            cfg.setMagFilterNearest();
            cfg.setMinFilterLinearMipmapNearest();
            return cfg;
        }
    }
}
