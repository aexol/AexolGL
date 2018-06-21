#ifndef TEXTURE_COMPONENT_AEX_GUI_HEADER
#define TEXTURE_COMPONENT_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "TriangleBatch.hpp"
#include "TextureMesh.hpp"
#include "Component.hpp"

namespace aex {
    namespace gui {

        class AEX_PUBLIC_API TextureComponent : public Component {
        public:
            friend class CardsPanel;
            TextureComponent(const Area& area, Texture* texture);
            virtual ~TextureComponent();
            void setTexture(Texture* texture);
            void setFlip(bool xAxis, bool yAxis);
        protected:
            void refillBatch(const Area& area);

        protected:
            virtual void draw(aex::AexRenderer* gl);
            virtual void finishdraw(aex::AexRenderer* gl);
            virtual void needsUpdate();
            virtual void refresh(int flags);
            ComponentPtr setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe);

        protected:
            TriangleBatch m_batch;
            std::unique_ptr<TextureMesh> m_mesh;
        };
    }
}

#endif
