#ifndef SPRITE_AEX_GUI_HEADER
#define SPRITE_AEX_GUI_HEADER

#include "Component.hpp"
#include "Color.hpp"
#include "TriangleBatch.hpp"
#include "TextureMesh.hpp"
#include "TextureManager.hpp"

namespace aex
{
    namespace gui
    {
        class TextureMap;

        class AEX_PUBLIC_API Sprite : public Component {
        public:
            virtual ~Sprite();
        protected:
            Sprite(const aex::gui::Area& area, const Area& UVs, const std::string& tMapPath, aex::TextureConfig tCfg);
        public:
            Sprite(Sprite& cm) = delete;
            Sprite(Sprite&& cm) = delete;
            void setColorMod(const Color& color);
            const Color& getColorMod() const;
            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);
            virtual void needsUpdate();
            virtual void refresh(int flags);

            friend class TextureMap;
        protected:
            virtual ComponentPtr setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe);
        protected:
            TriangleBatch m_batch;
            std::unique_ptr<TextureMesh> m_mesh;
            Area m_UVs;

            aex::gui::TextureManager::handle_t m_tMapHandler;
            aex::Texture* m_tMap;
        };
    }
}


#endif
