#include "aex/gui/BoundingRect.hpp"
#include "aex/gui/ColorRectComponent.hpp"

namespace aex
{
    namespace gui
    {

        BoundingRect::BoundingRect(const Color& color, const Area& area, float thickness) :
        m_color(color),
        m_thickness(thickness)
        {
            m_thickness = thickness;
            setArea(area);
        }

        void BoundingRect::refresh(int flags)
        {
            Area left(getArea().p(), Size(m_thickness, getArea().s().h()));
            Area top(getArea().p(), Size(getArea().s().w(), m_thickness));
            Area right(Position(getArea().p().x() + getArea().s().w() - m_thickness, getArea().p().y()), Size(m_thickness, getArea().s().h()));
            Area bottom(Position(getArea().p().x(), getArea().p().y() + getArea().s().h() - m_thickness), Size(getArea().s().w(), m_thickness));

            m_left.reset(new ColorRectComponent(m_color, left));
            m_top.reset(new ColorRectComponent(m_color, top));
            m_right.reset(new ColorRectComponent(m_color, right));
            m_bottom.reset(new ColorRectComponent(m_color, bottom));
        }

        ComponentPtr BoundingRect::setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe)
        {
            return nullptr;
        }

        void BoundingRect::draw(aex::AexRenderer* gl)
        {
            if (m_left)
            {
                m_left->draw(gl);
                m_left->finishdraw(gl);
                m_top->draw(gl);
                m_top->finishdraw(gl);
                m_right->draw(gl);
                m_right->finishdraw(gl);
                m_bottom->draw(gl);
                m_bottom->finishdraw(gl);
            }
        }

        void BoundingRect::finishdraw(aex::AexRenderer* gl)
        {
        }

        void BoundingRect::needsUpdate()
        {
            m_left->needsUpdate();
            m_top->needsUpdate();
            m_right->needsUpdate();
            m_bottom->needsUpdate();
        }
    }
}