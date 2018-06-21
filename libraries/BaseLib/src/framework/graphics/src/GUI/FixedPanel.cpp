#include "aex/gui/FixedPanel.hpp"

namespace aex
{
    namespace gui
    {

        FixedPanel::FixedPanel() : Panel()
        {
        }

        FixedPanel::FixedPanel(const Area& area) : Panel()
        {
            setArea(area);
        }

        FixedPanel::~FixedPanel()
        {
        }

        void FixedPanel::addComponent(ComponentPtr component)
        {
            FixedPanel::addComponent(component, Position(0, 0));
        }

        void FixedPanel::removeComponent(ComponentPtr component)
        {
            m_positions.erase(component);
            Panel::removeComponent(component);
        }

        void FixedPanel::addComponent(ComponentPtr component, const Position& relativePosition)
        {
            m_positions[component] = relativePosition;
            Panel::addComponent(component);
        }

        void FixedPanel::set(ComponentPtr component, const Position& relativePosition)
        {
            if (std::find(m_components.begin(),
                        m_components.end(), component) != m_components.end())
            {
                m_positions[component] = relativePosition;
                component->setPosition(getInnerArea().p()+relativePosition);
            } else {
                addComponent(component, relativePosition);
            }
            invalidate();
        }

        void FixedPanel::addComponent(ComponentPtr component, const Position& relativePosition, const Pivot& pivot)
        {
            addComponent(component,
                    relativePosition+getPosDeltaFor(component,pivot));
        }

        void FixedPanel::set(ComponentPtr component, const Position& relativePosition, const Pivot& pivot)
        {
            set(component, relativePosition+getPosDeltaFor(component,pivot));
        }

        void FixedPanel::refresh(int flags)
        {
            for (unsigned i = 0; i < m_components.size(); i++)
            {
                Position pos = getInnerArea().p() + m_positions[m_components[i]];
                m_components[i]->setPosition(pos);
            }
        }

        Position FixedPanel::getPosDeltaFor(ComponentPtr component, const Pivot& pivot) const
        {
            return Position( -component->getArea().s().w() * pivot.x(), -component->getArea().s().h() * pivot.y() );
        }
    }
}
