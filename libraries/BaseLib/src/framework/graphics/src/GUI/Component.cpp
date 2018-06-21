#include "aex/gui/Component.hpp"
#include "aex/gui/Frame.hpp"
#include "aex/gui/ColorRectComponent.hpp"
#include <bitset>

namespace aex
{
    namespace gui
    {
        long long Component::GLOBAL_ID_COUNTER = 0;

        Component::Component() :
            m_bBackground(false),
            m_visible(true),
            m_needUpdate(true)
        {
            recalcAreas();
            clearFlags();
            m_id = GLOBAL_ID_COUNTER++;
        }

        Component::Component(const Area& area) :
            m_area(area),
            m_outerArea(area),
            m_bBackground(false),
            m_visible(true),
            m_needUpdate(true)
        {
            recalcAreas();
            clearFlags();
            m_id = GLOBAL_ID_COUNTER++;
        }

        Component::Component(const Area& area, const Interspace& margin, const Interspace& padding) :
            m_area(area),
            m_margin(margin),
            m_padding(padding),
            m_bBackground(false),
            m_visible(true),
            m_needUpdate(true)
        {
            recalcAreas();
            clearFlags();
            m_id = GLOBAL_ID_COUNTER++;
        }

        Component::~Component()
        {
        }

        void Component::setMargin(const Interspace& margin)
        {
            if (m_margin != margin)
                setFlag(FLAG::MARGIN_CHANGED);

            m_margin = margin;
            recalcAreas();
            setRefresh(true);
        }

        void Component::setPadding(const Interspace& padding)
        {
            if (m_padding != padding)
                setFlag(FLAG::PADDING_CHANGED);

            m_padding = padding;
            recalcAreas();
            setRefresh(true);
        }

        void Component::setArea(const Area& area)
        {
            if (m_area.s() != area.s())
                setFlag(FLAG::SIZE_CHANGED);

            m_area = area;
            recalcAreas();
            setRefresh(true);
        }

        void Component::setHitbox(const Scale& scale, const Position& movement)
        {
            if ((movement != m_deltaHitbox) || (m_scaleHitbox != scale))
            {
                setFlag(FLAG::HITBOX_CHANGED);
            }

            m_deltaHitbox = movement;
            m_scaleHitbox = scale;
            setRefresh(true);
        }

        void Component::setPosition(const Position& p)
        {
            if (p != m_area.p())
                setFlag(FLAG::POSITION_CHANGED);

            Position vector = Position(m_margin.l(), m_margin.t());
            m_area.setPosition(vector + p);
            recalcAreas();
            setRefresh(true);
        }

        void Component::setSize(const Size& s)
        {
            if (s != m_area.s())
                setFlag(FLAG::SIZE_CHANGED);

            m_area.setSize(s);
            recalcAreas();
            setRefresh(true);
        }

        void Component::setUsing(const Component& comp)
        {
            setArea(comp.getArea());
            setPadding(comp.getPadding());
            setMargin(comp.getMargin());
            setHitbox(comp.m_scaleHitbox, comp.m_deltaHitbox);
        }

        void Component::clearFlags()
        {
            flags::clearFlags(&m_flags);
        }

        void Component::setFlag(FLAG flag)
        {
            flags::setFlag(static_cast<int> (flag), &m_flags);
        }

        void Component::setFlag(int flag)
        {
            flags::setFlag(flag, &m_flags);
        }

        bool Component::checkFlag(FLAG flag, int inFlags)
        {
            return checkFlag(static_cast<int>(flag), inFlags);
        }

        bool Component::checkFlag(int flag, int inFlags)
        {
            return flags::checkFlag(flag, inFlags);
        }

        int Component::getFlags() const
        {
            return m_flags;
        }

        void Component::forceRefresh()
        {
            refresh(getFlags());
            onRefresh();
            setRefresh(false);
        }

        Area Component::getInnerArea() const
        {
            return m_innerArea;
        }

        Area Component::getOuterArea() const
        {
            return m_outerArea;
        }

        const Interspace& Component::getMargin() const
        {
            return m_margin;
        }

        const Interspace& Component::getPadding() const
        {
            return m_padding;
        }

        const Area& Component::getArea() const
        {
            return m_area;
        }

        void Component::setBackgroundVisible(bool visible)
        {
            if (m_bBackground != visible)
            {
                if (visible)
                {
                    m_bBackground = visible;
                    resetBackground();
                } else
                {
                    m_background.reset();
                }
            }
            m_bBackground = visible;
        }

        bool Component::isBackgroundVisible() const
        {
            return m_bBackground;
        }

        void Component::setBackgroundColor(const Color& color)
        {
            m_backgroundColor = color;
            resetBackground();
        }

        Area Component::getHitbox() const
        {
            Area hitbox = m_area;
            hitbox.scale(m_scaleHitbox);
            hitbox.move(m_deltaHitbox);
            return hitbox;
        }

        bool Component::isHitted(const Position& point) const
        {
            return getHitbox().isPointInArea(point);
        }

        bool Component::isTouchFocus() const
        {
            if (this == Frame::getGlobalTouchFocused())
                return true;
            return false;
        }

        bool Component::isInputFocus() const
        {
            if (this == Frame::getGlobalInputFocused())
                return true;
            return false;
        }

        bool Component::isVisible() const
        {
            return m_visible;
        }

        void Component::setVisible(bool value)
        {
            m_visible = value;
        }

        long long Component::getID() const
        {
            return m_id;
        }

        void Component::addTouchActionListener(TouchActionListener* listener)
        {
            m_touchActionListeners.push_back(listener);
        }

        bool Component::isFocusableFor(FOCUS_TYPE type) const
        {
            // always true for not known component
            return true;
        }

        void Component::focusCatched(FOCUS_TYPE type) {
        }

        void Component::focusReleased(FOCUS_TYPE type) {
        }

        void Component::drawBackground(AexRenderer* gl)
        {
            if (isBackgroundVisible())
            {
                m_background->draw(gl);
                m_background->finishdraw(gl);
            }
        }

        void Component::base_touchDown(float x, float y, int fingerId)
        {
            this->touchDown(x, y, fingerId);
            for (unsigned i = 0; i < m_touchActionListeners.size(); i++)
            {
                Position innerPos(x - getInnerArea().p().x(), y - getInnerArea().p().y());
                m_touchActionListeners[i]->actionTouchDown(TouchEvent(getID(), Position(x, y), innerPos, Position(), fingerId));
            }
        }

        void Component::base_touchMotion(float x, float y, float dx, float dy, int fingerId)
        {
            this->touchMotion(x, y, dx, dy, fingerId);
            for (unsigned i = 0; i < m_touchActionListeners.size(); i++)
            {
                Position innerPos(x - getInnerArea().p().x(), y - getInnerArea().p().y());
                m_touchActionListeners[i]->actionTouchMotion(TouchEvent(getID(), Position(x, y), innerPos, Position(dx, dy), fingerId));
            }
        }

        void Component::base_touchUp(float x, float y, int fingerId)
        {
            this->touchUp(x, y, fingerId);
            for (unsigned i = 0; i < m_touchActionListeners.size(); i++)
            {
                Position innerPos(x - getInnerArea().p().x(), y - getInnerArea().p().y());
                m_touchActionListeners[i]->actionTouchUp(TouchEvent(getID(), Position(x, y), innerPos, Position(), fingerId));
            }
        }

        void Component::base_touchLine(float x1, float y1, float x2, float y2, int fingerId)
        {
            this->touchLine(x1, y1, x2, y2, fingerId);
        }

        void Component::base_touchClick(float x, float y, int fingerId)
        {
            this->touchClick(x, y, fingerId);
            for (unsigned i = 0; i < m_touchActionListeners.size(); i++)
            {
                Position innerPos(x - getInnerArea().p().x(), y - getInnerArea().p().y());
                m_touchActionListeners[i]->actionTouchClick(TouchEvent(getID(), Position(x, y), innerPos, Position(), fingerId));
            }
        }

        void Component::base_touchScale(float scale)
        {
            this->touchScale(scale);
        }

        void Component::base_key_down_event(KeyboardEventData e)
        {
            this->key_down_event(e);
        }

        void Component::base_key_up_event(KeyboardEventData e)
        {
            this->key_up_event(e);
        }

        void Component::base_key_textedit_event(TextEditEventData e)
        {
            this->key_textedit_event(e);
        }

        void Component::base_key_textinput_event(TextInputEventData e)
        {
            this->key_textinput_event(e);
        }

        // TODO add MouseEventsListeners logic:
        void Component::base_mouse_down_event(MouseButtonEventData e)
        {
            this->mouse_down_event(e);
            // listeners logic here
        }
        void Component::base_mouse_up_event(MouseButtonEventData e)
        {
            this->mouse_up_event(e);
        }
        void Component::base_mouse_motion_event(MouseMotionEventData e)
        {
            this->mouse_motion_event(e);
        }
        void Component::base_mouse_wheel_event(MouseWheelEventData e)
        {
            this->mouse_wheel_event(e);
        }

        void Component::recalcAreas()
        {
            m_outerArea = m_area + m_margin;
            m_innerArea = m_area - m_padding;
            if (isBackgroundVisible())
                resetBackground();
        }

        void Component::resetBackground()
        {
            if (m_bBackground)
            {
                m_background.reset(new ColorRectComponent(m_backgroundColor, getArea()));
            }
        }

        void Component::setRefresh(bool value)
        {
            m_needUpdate = value;
            if (!value)
            {
                clearFlags();
            }
        }

    }
}
