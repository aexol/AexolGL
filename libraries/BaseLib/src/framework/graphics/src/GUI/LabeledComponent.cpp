#include "aex/gui/LabeledComponent.hpp"

namespace aex
{
    namespace gui
    {
        LabeledComponent::LabeledComponent(ComponentPtr component, ComponentPtr label, Position labelDelta, LabeledComponent::SIDE side, bool center) :
        m_component(component),
        m_label(label),
        m_labelDelta(labelDelta),
        m_labelSide(side),
        m_center(center)
        {
            setUsing(*m_component);
        }

        LabeledComponent::~LabeledComponent()
        {
        }

        void LabeledComponent::draw(AexRenderer* gl)
        {
            m_component->draw(gl);
            m_component->finishdraw(gl);
            
            m_label->draw(gl);
            m_label->finishdraw(gl);
        }

        void LabeledComponent::finishdraw(AexRenderer* gl)
        {

        }

        void LabeledComponent::needsUpdate()
        {
            m_component->needsUpdate();
        }

        void LabeledComponent::refresh(int flags)
        {
            m_component->setUsing(*this);
            m_component->forceRefresh();

            Position vector;
            switch (m_labelSide)
            {
                case LabeledComponent::SIDE::LEFT:
                    if (m_center)
                        vector = Position(-m_label->getArea().s().w(), (getArea().s().h() / 2.0f)-(m_label->getArea().s().h() / 2.0f));
                    else
                        vector = Position(-m_label->getArea().s().w(), 0.0f);
                    vector = vector + m_labelDelta;
                    m_label->setPosition(m_component->getArea().leftUp() + vector);
                    break;

                case LabeledComponent::SIDE::TOP:
                    if (m_center)
                        vector = Position((getArea().s().w() - m_label->getArea().s().w()) / 2.0f, -m_label->getArea().s().h());
                    else
                        vector = Position(0.0f, -m_label->getArea().s().h());
                    vector = vector + m_labelDelta;
                    m_label->setPosition(m_component->getArea().leftUp() + vector);
                    break;

                case LabeledComponent::SIDE::RIGHT:
                    if (m_center)
                        vector = Position(0.0f, (getArea().s().h() / 2.0f)-(m_label->getArea().s().h() / 2.0f));
                    else
                        vector = Position(0.0f, 0.0f);
                    vector = vector + m_labelDelta;
                    m_label->setPosition(m_component->getArea().rightUp() + vector);
                    break;

                case LabeledComponent::SIDE::BOTTOM:
                default:
                    if (m_center)
                        vector = Position((getArea().s().w() - m_label->getArea().s().w()) / 2.0f, 0.0f);
                    else
                        vector = Position(0.0f, 0.0f);
                    vector = vector + m_labelDelta;
                    m_label->setPosition(m_component->getArea().leftDown() + vector);
                    break;
            }

            m_label->forceRefresh();
        }

        ComponentPtr LabeledComponent::setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe)
        {
            return m_component->setFocus(point, type, focusMe);
        }
    }
}