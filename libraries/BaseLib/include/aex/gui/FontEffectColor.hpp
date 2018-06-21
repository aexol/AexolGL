#ifndef FONT_EFFECT_COLOR_AEX_GUI_HEADER
#define FONT_EFFECT_COLOR_AEX_GUI_HEADER

#include "FontEffect.hpp"

namespace aex {
    namespace gui {

        class FontEffectColor : public FontEffect {
        public:
            FontEffectColor(std::unique_ptr<FontSource> fntSrc, Color color = Color(255,0,0) );
            virtual ~FontEffectColor();

            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);
            virtual void needsUpdate();
            virtual void colorChanged();
        protected:
            UniformFloat4 m_uColor;
        };
    }
}

#endif
