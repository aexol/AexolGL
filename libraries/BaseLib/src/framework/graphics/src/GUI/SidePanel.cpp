#include "aex/gui/SidePanel.hpp"

namespace aex
{
    namespace gui
    {
        SidePanel::SidePanel() :
        Panel(),
        m_lastTop(nullptr),
        m_lastBottom(nullptr),
        m_lastRight(nullptr),
        m_lastLeft(nullptr)
        {
        }

        SidePanel::SidePanel(Area area) :
        Panel(),
        m_lastTop(nullptr),
        m_lastBottom(nullptr),
        m_lastRight(nullptr),
        m_lastLeft(nullptr)
        {
            setArea(area);
        }

        SidePanel::~SidePanel()
        {
        }

        void SidePanel::addComponent(ComponentPtr component)
        {
            SidePanel::addComponent(component, SIDE::TOP);
        }

        void SidePanel::removeComponent(ComponentPtr component)
        {
            m_sides.erase(component);
            Panel::removeComponent(component);
        }

        void SidePanel::addComponent(ComponentPtr component, SIDE side)
        {
            m_sides[component] = side;
            Panel::addComponent(component);
        }

        void SidePanel::refresh(int flags)
        {
            // porozmieszczanie wedlug kierunkow lewo prawo gora dol
            m_lastTop = nullptr;
            m_lastBottom = nullptr;
            m_lastRight = nullptr;
            m_lastLeft = nullptr;

            for (unsigned i = 0; i < m_components.size(); i++)
            {
                SIDE side = m_sides[m_components[i]];
                switch (side)
                {
                    case SIDE::TOP:
                        if (m_lastTop == nullptr)
                        {
                            m_components[i]->setPosition(getInnerArea().p());
                            m_lastTop = m_components[i];
                        } else
                        {
                            Position nextP = m_lastTop->getOuterArea().p();
                            Position vector;
                            vector = Position(0.0f, m_lastTop->getOuterArea().s().h());
                            nextP = nextP + vector;
                            m_lastTop = m_components[i];
                            m_lastTop->setPosition(nextP);
                        }
                        break;
                    case SIDE::BOTTOM:
                        if (m_lastBottom == nullptr)
                        {
                            Position nextP = getInnerArea().p();
                            Position vector = Position(0.0f, -m_components[i]->getOuterArea().s().h());
                            Position vector2 = Position(0.0f, getInnerArea().s().h());
                            nextP = nextP + vector;
                            nextP = nextP + vector2;
                            m_lastBottom = m_components[i];
                            m_lastBottom->setPosition(nextP);
                        } else
                        {
                            Position nextP = m_lastBottom->getOuterArea().p();
                            Position vector = Position(0.0f, -m_components[i]->getOuterArea().s().h());
                            nextP = nextP + vector;
                            m_lastBottom = m_components[i];
                            m_lastBottom->setPosition(nextP);
                        }
                        break;
                    case SIDE::RIGHT:
                        if (m_lastRight == nullptr)
                        {
                            Position nextP = getInnerArea().p();
                            Position vector = Position(getInnerArea().s().w(), 0.0f);
                            Position vector2 = Position(-m_components[i]->getOuterArea().s().w(), 0.0f);
                            nextP = nextP + vector;
                            nextP = nextP + vector2;
                            m_lastRight = m_components[i];
                            m_lastRight->setPosition(nextP);
                        } else
                        {
                            Position nextP = m_lastRight->getOuterArea().p();
                            Position vector = Position(-m_components[i]->getOuterArea().s().w(), 0.0f);
                            nextP = nextP + vector;
                            m_lastRight = m_components[i];
                            m_lastRight->setPosition(nextP);
                        }
                        break;
                    case SIDE::LEFT:
                        if (m_lastLeft == nullptr)
                        {
                            Position nextP = getInnerArea().p();
                            m_lastLeft = m_components[i];
                            m_lastLeft->setPosition(nextP);
                        } else
                        {
                            Position nextP = m_lastLeft->getOuterArea().p();
                            Position vector = Position(m_lastLeft->getOuterArea().s().w(), 0.0f);
                            nextP = nextP + vector;
                            m_lastLeft = m_components[i];
                            m_lastLeft->setPosition(nextP);
                        }
                        break;
                    default:
                        break;
                }
            }

        }
    }
}