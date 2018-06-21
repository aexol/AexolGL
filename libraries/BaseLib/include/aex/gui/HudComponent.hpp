#ifndef HUD_COMPONENT_AEX_GUI_HEADER
#define HUD_COMPONENT_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "Component.hpp"
#include "HudUtils.hpp"

namespace aex {
    namespace gui {
        
        class AEX_PUBLIC_API HudComponent : public Component {
        public:

            HudComponent(const Hud_ptr& hud);
            HudComponent(const std::string& texMapPath, const std::string& jsonPath, aex::TextureConfig texConfig = HudUtils::getLinearTCFG());
            virtual ~HudComponent();
            void setScale(const Scale& s);

            // DrawObject Interface
            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);
            virtual void needsUpdate();
            void hideAll();
            void showAll();
            void setVisibleFor(const std::string& hudElemName, bool visible);
            virtual void refresh(int flags);

            virtual ComponentPtr setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe);
            void setHud(const Hud_ptr& hud);
        protected:

            // TouchInterface
            virtual void touchDown(float x, float y, int fingerId);
            virtual void touchMotion(float x, float y, float dx, float dy, int fingerId);
            virtual void touchUp(float x, float y, int fingerId);
            virtual void touchLine(float x1, float y1, float x2, float y2, int fingerId);
            virtual void touchClick(float x, float y, int fingerId);
            virtual void touchScale(float scale);

        protected:
            Hud_ptr m_hud;
            std::map<HudElement*, Position> m_initialPositions;
            std::map<HudElement*, Size> m_initialSizes;
            Size m_initialSize;
            Size m_lastSize;
            Scale m_scale;
        };
    }
}

#endif