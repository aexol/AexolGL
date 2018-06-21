#ifndef RADIO_BUTTON_AEX_GUI_HEADER
#define RADIO_BUTTON_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "Component.hpp"
#include "HudComponent.hpp"



namespace aex {
    namespace gui {

        class AEX_PUBLIC_API RadioButton : public Component {
        public:
            RadioButton(HudComponentPtr on, HudComponentPtr off, bool isOn = false);
            void setOnChangeAction(const std::function<void(bool)>& onChange);
            bool isOn() const;

        protected:
            virtual void draw(aex::AexRenderer* gl);
            virtual void finishdraw(aex::AexRenderer* gl);
            virtual void needsUpdate();
            virtual void refresh(int flags);
            virtual void touchDown(float x, float y, int fingerId);
            //            virtual void touchMotion(float x, float y, float dx, float dy, int fingerId);
            //            virtual void touchUp(float x, float y, int fingerId);
            //            virtual void touchLine(float x1, float y1, float x2, float y2, int fingerId);
            //            virtual void touchClick(float x, float y, int fingerId);
            //            virtual void touchScale(float scale);
            virtual ComponentPtr setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe);

        protected:
            HudComponentPtr m_onComp;
            HudComponentPtr m_offComp;
            HudComponentPtr m_drawComp;
            bool m_bIsOn;
            std::function<void(bool) > m_onChange;

        };
    }
}

#endif