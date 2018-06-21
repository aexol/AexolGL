#ifndef FONT_EFFECT_MOVING_BACKGROUND_AEX_GUI_HEADER
#define FONT_EFFECT_MOVING_BACKGROUND_AEX_GUI_HEADER

#include "FontEffect.hpp"
#include "TextureManager.hpp"

namespace aex {
    namespace gui {

        class FontEffectMovingBackground : public FontEffect {
        public:
            FontEffectMovingBackground(std::unique_ptr<FontSource> fntSrc, const std::string& backgroundTexturePath);
            virtual ~FontEffectMovingBackground();

            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);
            virtual void needsUpdate();
            virtual void colorChanged();
        protected:
            std::string m_backgroundTexturePath;
            aex::gui::TextureManager::handle_t m_backTexHandler;
            Texture* m_backTex;
            UniformSampler m_uBackTexSampler;
            UniformFloat2 m_uBackDelta;
            UniformFloat4 m_uColor;
            float m_deltaX;
            float m_deltaY;
            float m_modificatorX;
            float m_modificatorY;
        };

    }
}

#endif
