#ifndef LABELED_COMPONENT_AEX_GUI_HEADER
#define LABELED_COMPONENT_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "Component.hpp"

namespace aex {
    namespace gui {

        class AEX_PUBLIC_API LabeledComponent : public Component {
        public:

            enum SIDE {
                LEFT,
                RIGHT,
                TOP,
                BOTTOM
            };
            LabeledComponent(ComponentPtr component, ComponentPtr label, Position labelDelta = Position(0, 0), LabeledComponent::SIDE side = LabeledComponent::SIDE::BOTTOM, bool center = true);
            virtual ~LabeledComponent();

            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);
            virtual void needsUpdate();

            virtual void refresh(int flags);

            ComponentPtr setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe);
        protected:

            ComponentPtr m_component;
            ComponentPtr m_label;
            Position m_labelDelta;
            LabeledComponent::SIDE m_labelSide;
            bool m_center;
        };
    }
}

#endif