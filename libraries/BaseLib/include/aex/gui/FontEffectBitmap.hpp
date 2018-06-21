#ifndef FONT_EFFECT_BITMAP_AEX_GUI_HEADER
#define FONT_EFFECT_BITMAP_AEX_GUI_HEADER

#include "FontEffect.hpp"

namespace aex {
    namespace gui {
        /**
         * @brief FontEffect using prerendered bitmap font.
         *
         * @details This font effect draws text with pair of
         * Angel Code and prerendered font bitmap.
         */
        class FontEffectBitmap : public FontEffect {
        public:
            /**
             * @brief New FontEffectbitmap
             *
             * @param fntSrc Font source.
             */
            FontEffectBitmap(std::unique_ptr<FontSource>&& fntSrc);
            virtual ~FontEffectBitmap();

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
