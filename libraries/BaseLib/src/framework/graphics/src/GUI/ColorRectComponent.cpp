#include "aex/gui/ColorRectComponent.hpp"

namespace aex
{
    namespace gui
    {
        ColorRectComponent::ColorRectComponent(const Area& area, const Color& color) :
        ColorRectComponent(color, area)
        {
        }


        ColorRectComponent::ColorRectComponent(const Color& color, const Area& area) :
        Component(area)
        {
            Area tmpArea = getArea();
            tmpArea.setPosition(Position(0,0));
            m_batch.refillBatch(getArea().inPosition(Position(0,0)), m_batch.getFullTextureUVsArea());
            m_mesh.reset(new ColorMesh(color, m_batch));
            m_mesh->setDelta(area.leftUp());
        }

        void ColorRectComponent::refresh(int flags)
        {
            if (checkFlag(FLAG::SIZE_CHANGED, flags))
            {
                m_batch.refillBatch(getArea().inPosition(Position(0,0)), m_batch.getFullTextureUVsArea());
                m_mesh->setBatch(m_batch);
            }
            m_mesh->setDelta(getArea().leftUp());
        }

        void ColorRectComponent::draw(AexRenderer* gl)
        {
            m_mesh->draw(gl);
        }

        void ColorRectComponent::finishdraw(AexRenderer* gl)
        {
            m_mesh->finishdraw(gl);
        }

        void ColorRectComponent::needsUpdate()
        {
        }

        void ColorRectComponent::setColor(const Color& color)
        {
            m_mesh->setColor(color);
        }

        const Color& ColorRectComponent::getColor() const
        {
            return m_mesh->getColor();
        }

        ComponentPtr ColorRectComponent::setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe)
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
