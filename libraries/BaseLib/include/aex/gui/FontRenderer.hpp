#ifndef FONT_RENDERER_AEX_GUI_HEADER
#define FONT_RENDERER_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "Component.hpp"
#include "BitmapFont.hpp"
#include <aex/TextDrw.hpp>


namespace aex {
    namespace gui {
        /**
         * @brief 
         */
        class AEX_PUBLIC_API FontRenderer : public Component {
        public:

            struct FontCounter {

                FontCounter() : counter(0) {
                }

                FontCounter(aex::TextDrw_ptr t) : texts(t), counter(0) {
                }
                aex::TextDrw_ptr texts;
                unsigned counter;
            };
            FontRenderer(float height, const BitmapFontGroup& font);
            virtual ~FontRenderer();
            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);
            virtual void needsUpdate();
            virtual void refresh(int flags) = 0;
            void setText(const std::string& text, bool callback = true);
        protected:
            virtual void onTextChanged() = 0;

        protected:
            static std::map<std::string, FontCounter> g_fontsRenderers;
            std::string m_glyphsPath;
            aex::TextDrw_ptr m_textRef;
            aex::TextString_ptr m_textString;
            float m_scale;
            float m_height;
            std::string m_string;
        };
    }
}

#endif
