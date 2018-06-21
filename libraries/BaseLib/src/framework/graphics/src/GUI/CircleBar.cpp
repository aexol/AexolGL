#include "aex/gui/CircleBar.hpp"
#include <iostream>

namespace aex
{
    namespace gui
    {
        CircleBar::CircleBar(const Area& area, const Color& color, const Color& backColor, float thickness, float percent, float startPercent)
        {
            m_area = area;
            m_barMesh.reset(new CircleBarMesh(area, color, backColor, thickness, percent, startPercent));
            setArea(area);
        }

        CircleBar::~CircleBar()
        {
        }

        void CircleBar::setStartPercent(float p)
        {
            using namespace std;
            m_startPercent = p;
            m_startPercent = max(min(100.0f, m_startPercent), 0.0f);
            m_barMesh->setStartPercent(m_startPercent);
        }

        void CircleBar::setBarColor(const Color& c)
        {
            m_barMesh->setBarColor(c);
        }

        void CircleBar::setBackgroundColor(const Color& c)
        {
            m_barMesh->setBackgroundColor(c);
        }

        void CircleBar::setPercent(float percent0_to_100)
        {
            using namespace std;
            m_percent = percent0_to_100;
            m_percent = max(min(100.0f, m_percent), 0.0f);
            m_barMesh->setPercent(m_percent);
        }

        void CircleBar::setBlurrPercent(float percent0_to_1)
        {
            m_barMesh->setBlurrPercent(percent0_to_1);
        }

        float CircleBar::getBlurrPercent() const
        {
            return m_barMesh->getBlurrPercent();
        }

        void CircleBar::refresh(int flags)
        {
            m_barMesh->setArea(getInnerArea()); // TODO add check if just movement and delta refreshing only
        }

        void CircleBar::draw(AexRenderer* gl)
        {
            m_barMesh->draw(gl);
            m_barMesh->finishdraw(gl);
        }

        void CircleBar::finishdraw(AexRenderer* gl)
        {
        }

        void CircleBar::needsUpdate()
        {
            m_barMesh->needsUpdate();
        }

        ComponentPtr CircleBar::setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe)
        {
            return nullptr; // NOT FOCUS CATCHING
        }
    }
}
