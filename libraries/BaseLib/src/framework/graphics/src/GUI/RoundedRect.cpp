#include "aex/gui/RoundedRect.hpp"

namespace aex
{
    namespace gui
    {
        RoundedRect::RoundedRect(const Area& area, const Color& color, float circleRadius) :
        Component(area)
        {
            Area tmpArea = getArea();
            tmpArea.setPosition(Position(0,0));
            m_batch.refillBatch(getArea().inPosition(Position(0,0)), m_batch.getFullTextureUVsArea());
            m_mesh.reset(new RoundedRectMesh(color, m_batch, getArea(), circleRadius));
            m_mesh->setDelta(area.leftUp());
        }

        void RoundedRect::refresh(int flags)
        {
            if (checkFlag(FLAG::SIZE_CHANGED, flags))
            {
                m_batch.refillBatch(getArea().inPosition(Position(0,0)), m_batch.getFullTextureUVsArea());
                m_mesh->setBatch(m_batch);
            }
            m_mesh->setDelta(getArea().leftUp());
            m_mesh->setAreaForUniforms(getArea());
        }

        void RoundedRect::draw(AexRenderer* gl)
        {
            m_mesh->draw(gl);
        }

        void RoundedRect::finishdraw(AexRenderer* gl)
        {
            m_mesh->finishdraw(gl);
        }

        void RoundedRect::needsUpdate()
        {
        }

        void RoundedRect::setColor(const Color& color)
        {
            m_mesh->setColor(color);
        }

        const Color& RoundedRect::getColor() const
        {
            return m_mesh->getColor();
        }

        void RoundedRect::setCircleRadius(float circleRadius)
        {
            m_mesh->setCircleRadius(circleRadius);
        }

        float RoundedRect::getCircleRadius() const
        {
            return m_mesh->getCircleRadius();
        }

        void RoundedRect::setBlurrPercent(float percent0_to_1)
        {
            m_mesh->setBlurrPercent(percent0_to_1);
        }

        float RoundedRect::getBlurrPercent() const
        {
            return m_mesh->getBlurrPercent();
        }

        ComponentPtr RoundedRect::setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe)
        {
            if (isHitted(point))
            {
                if (type == FOCUS_TYPE::TOUCH)
                {
                    focusMe(this);
                    return this;
                }
            }
            return nullptr;
        }
    }
}
