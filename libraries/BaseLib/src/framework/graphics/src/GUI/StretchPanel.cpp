#include "aex/gui/StretchPanel.hpp"

namespace aex
{
    namespace gui
    {

        StretchPanel::StretchPanel(const Area& area) : Panel()
        {
            setArea(area);
        }

        StretchPanel::~StretchPanel()
        {
        }

        void StretchPanel::addComponent(ComponentPtr component)
        {
            addComponent(component, StretchPanel::ORIENTATION::TOP_BOTTOM);
        }

        void StretchPanel::removeComponent(ComponentPtr component)
        {
            m_orientations.erase(component);
            Panel::removeComponent(component);
        }

        void StretchPanel::addComponent(ComponentPtr component, const ORIENTATION orientation)
        {
            m_orientations[component] = orientation;
            Panel::addComponent(component);
        }

        void StretchPanel::refresh(int flags)
        {
            m_lastLeft = nullptr;
            m_lastTop = nullptr;

            int leftRightElems = 0;
            int topBottomElems = 0;

            float leftRightSum = 0;
            float topBottomSum = 0;

            for (unsigned i = 0; i < m_components.size(); i++)
            {
                if (m_orientations[m_components[i]] == StretchPanel::ORIENTATION::LEFT_RIGHT)
                {
                    leftRightElems++;
                    leftRightSum += m_components[i]->getOuterArea().s().w();
                } else
                    if (m_orientations[m_components[i]] == StretchPanel::ORIENTATION::TOP_BOTTOM)
                {
                    topBottomElems++;
                    topBottomSum += m_components[i]->getOuterArea().s().h();
                }
            }

            float leftRightSpace = (getInnerArea().s().w() - leftRightSum) / (leftRightElems - 1);
            float topBottomSpace = (getInnerArea().s().h() - topBottomSum) / (topBottomElems - 1);

            leftRightSpace = std::max(0.0f, leftRightSpace);
            topBottomSpace = std::max(0.0f, topBottomSpace);

            float dX, dY;
            for (unsigned i = 0; i < m_components.size(); i++)
            {
                StretchPanel::ORIENTATION orient = m_orientations[m_components[i]];
                switch (orient)
                {
                    case StretchPanel::ORIENTATION::LEFT_RIGHT:
                        if (m_lastLeft == nullptr)
                        {
                            m_lastLeft = m_components[i];
                            m_lastLeft->setPosition(getInnerArea().p());
                        } else
                        {
                            Position nextP = m_lastLeft->getOuterArea().rightUp();
                            Position vector = Position(leftRightSpace, 0.0f);
                            nextP = nextP + vector;
                            m_lastLeft = m_components[i];
                            m_lastLeft->setPosition(nextP);
                        }
                        break;
                    case StretchPanel::ORIENTATION::TOP_BOTTOM:
                        if (m_lastTop == nullptr)
                        {
                            m_lastTop = m_components[i];
                            m_lastTop->setPosition(getInnerArea().p());
                        } else
                        {
                            Position nextP = m_lastLeft->getOuterArea().leftDown();
                            Position vector = Position(0.0f, topBottomSpace);
                            nextP = nextP + vector;
                            m_lastTop = m_components[i];
                            m_lastTop->setPosition(nextP);
                        }
                        break;
                    case StretchPanel::ORIENTATION::CENTER:
                        dX = (getInnerArea().s().w() - m_components[i]->getOuterArea().s().w()) / 2.0f;
                        dY = (getInnerArea().s().h() - m_components[i]->getOuterArea().s().h()) / 2.0f;

                        m_components[i]->setPosition(getInnerArea().leftUp() + Position(dX, dY));

                        break;
                    default:
                        break;
                }
            }
        }
    }
}