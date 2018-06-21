#ifndef FONT_EFFECT_AEX_GUI_HEADER
#define FONT_EFFECT_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "Mesh.hpp"
#include <aex/AexDrawable.hpp>
#include <string>
#include <locale>
#include <aex/Texture.hpp>
#include "TextSettings.hpp"
#include "ShaderManager.hpp"
#include "FontGlyphSet.hpp"
#include "Flags.hpp"
#include "FontSource.hpp"
#include "Color.hpp"

namespace aex {
    namespace gui {
        /**
         * @brief Abstract base class for font effects.
         */
        class AEX_PUBLIC_API FontEffect : public MeshBase, public DrawObject {
        public:
            /**
             * @brief New FontEffect.
             *
             * @param shaderHandle Handle to shader program used to draw font.
             * @param fnSrc Font source.
             * @param color Text color.
             */
            FontEffect(ShaderManager::handle_t shaderHandle,
                    std::unique_ptr<FontSource>&& fntSrc,
                    const Color& color);
            virtual ~FontEffect();

            /**
             * @brief Change text color.
             *
             * @param color New color.
             */
            void setColor(const Color& color);

            /**
             * @brief Get current text color.
             *
             * @return Color.
             */
            Color getColor() const;

            /**
             * @brief Called on color change.
             */
            virtual void colorChanged() = 0;

            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl) = 0;
            virtual void needsUpdate() = 0;
        protected:
            Size getTextWrapSize() const;
            Area getAreaOfChar(int num) const;
            int getCharNumIn(const Position& pos) const;
            float getLineHeight() const;
            float getLineTopAt(const Position& pos) const;

            void refresh(const TextSettings& ts, int flags);
            void rebuildBatch();
            virtual void batchRebuilded() {} // if inheriting classes wanna react on this (have their own vertex data on eg.)
            TriangleBatch m_triangleBatch;
            VertexData m_aVertex2;
            VertexData m_aUV2;
            IndexData m_indexes;

            Texture* m_texture;
            UniformSampler m_sampler;
            ShaderManager::handle_t m_shaderHandle;
            ShaderDrw* m_shader;

            Size m_testWrapSize;

            TextSettings m_textSettings;
            std::unique_ptr<FontSource> m_fntSrc;
            FontGlyphSet::Words m_words;
            int m_fontGlyphSetRegenNum;
            Color m_color;
        private:

            void initTexture(Texture* tex);
            FontGlyphSet* getFontGlyphSet();
            const FontGlyphSet* getConstFontGlyphSet() const;
            friend class TextComponent;
        };

        template<class T, class... Args>
        aex::unique_ptr<FontEffect> makefntrdr(Args&&... args) {
            return aex::unique_ptr<FontEffect>(new T(std::forward<Args>(args)...));
        }
    }
}

#endif
