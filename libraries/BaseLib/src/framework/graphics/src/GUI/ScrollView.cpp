#include "aex/gui/ScrollView.hpp"
#include "../../../include/internal_sdl.hpp"
#include "aex/gui/Frame.hpp"

namespace aex
{
    namespace gui
    {

        const float ScrollView::SCROLL_SAMPLE_TIME_S = 0.1f; // seconds
        const float ScrollView::SCROLL_EPSILON = 0.05; // normal width

        ScrollView::ScrollView() :
        m_delta(Position()),
        m_friction(Position(1.0f, 1.0f)),
        m_frictionMultiplier(1.0f),
        m_fingerID(-1),
        m_visibleID(-1),
        m_touchFocused(false),
        m_recalcBasePositions(false)
        {
            m_friction = Position(1.0f, 1.0f);
            m_lastSize = 0;
        }

        ScrollView::~ScrollView()
        {
        }

        void ScrollView::refresh(int flags)
        {
            m_recalcBasePositions = true;
            bool bCalcedContSize = false;
            if (m_lastInnerPosition != getInnerArea().p())
            {
                calcContentSize();
                bCalcedContSize = true;
                m_lastInnerPosition = getInnerArea().p();
                m_friction = Position(getInnerArea().s().w() * m_frictionMultiplier, getInnerArea().s().h() * m_frictionMultiplier);
            }

            if (!bCalcedContSize){
                if (m_lastSize != m_components.size())
                {
                    m_lastSize = m_components.size();
                    if (m_components.size() == 1)
                    {
                        m_delta.setY(0);
                    }
                    calcContentSize();
                    m_recalcBasePositions = false;
                }else{
                    if(m_recalcBasePositions)
                    calcContentSize();
                }
            }

            Position pos = getInnerArea().p();
            pos = pos + m_delta;
            // max up and down positions check

            if ((pos.y() > getMaxLeft().y()) || (getContentSize().h() <= getInnerArea().s().h()))
            {
                pos.setY(getMaxLeft().y());
                m_delta.setY(0);
                m_speedPerSecond.setY(0);
            } else if (pos.y() < getMinLeft().y())
            {
                float maxDeltaY = getContentSize().h() - getOuterArea().s().h();
                m_delta.setY(-maxDeltaY);
                m_speedPerSecond.setY(0);
                pos.setY(getMinLeft().y());
            }

            findVisibleIDIfNeeded();

            for (unsigned i = 0; i < m_elemsToDraw.size(); i++)
            {
                m_components[m_elemsToDraw[i]]->setVisible(false);
            }
            m_elemsToDraw.clear();

            Area svArea = getInnerArea();
            for (unsigned i = m_visibleID; i < m_components.size(); i++)
            {
                pos = m_basePositions[i] + m_delta;
                m_components[i]->setPosition(pos);
                if (svArea.intersectWith(m_components[i]->getOuterArea()))
                {
                    // add to visibles
                    m_elemsToDraw.push_back(i);
                    m_components[i]->setVisible(true);

                    if (!m_recalcBasePositions)
                    {
                        std::map<ComponentPtr, Size>::iterator iter = m_sizesMap.find(m_components[i]);
                        if ( iter != m_sizesMap.end())
                        {
                            if (m_sizesMap.at(m_components[i]) != m_components[i]->getOuterArea().s()){
                                iter->second = m_components[i]->getOuterArea().s();
                                m_recalcBasePositions = true;
                            }
                        } else
                        {
                            m_sizesMap[m_components[i]] = m_components[i]->getOuterArea().s();
                        }
                    }

                } else
                {
                    break;
                }
            }

            for (int i = m_visibleID - 1; i >= 0; i--)
            {
                pos = m_basePositions[i] + m_delta;
                m_components[i]->setPosition(pos);
                if (svArea.intersectWith(m_components[i]->getOuterArea()))
                {
                    m_elemsToDraw.push_back(i);
                    m_components[i]->setVisible(true);

                    if (!m_recalcBasePositions)
                    {
                        std::map<ComponentPtr, Size>::iterator iter = m_sizesMap.find(m_components[i]);
                         if ( iter != m_sizesMap.end())
                        {
                             LOG_DEBUG("prev: ", iter->second, " tmp: ", m_components[i]->getOuterArea().s());
                            if (m_sizesMap.at(m_components[i]) != m_components[i]->getOuterArea().s()){
                                iter->second = m_components[i]->getOuterArea().s();
                                m_recalcBasePositions = true;
                            }
                        } else
                        {
                            m_sizesMap[m_components[i]] = m_components[i]->getOuterArea().s();
                        }
                    }
                } else
                {
                    break;
                }
            }
        }

        void ScrollView::draw(AexRenderer* gl)
        {
            double timeNow = Frame::timestampSeconds();
            if (m_speedPerSecond.y() != 0 && !SDL_IsTextInputActive() && !m_touchFocused)
            {
                double deltaTime = timeNow - m_drawTime;
                Position tmpSpeed(0.0f, m_speedPerSecond.y() * deltaTime);
                m_delta = m_delta + tmpSpeed;

                Position tmpFriction;
                tmpFriction.setX(m_speedPerSecond.x() > 0 ? m_friction.x() * deltaTime : -m_friction.x() * deltaTime);
                tmpFriction.setY(m_speedPerSecond.y() > 0 ? m_friction.y() * deltaTime : -m_friction.y() * deltaTime);

                bool xBiggerThan0 = m_speedPerSecond.x() > 0;
                bool yBiggerThan0 = m_speedPerSecond.y() > 0;

                m_speedPerSecond = m_speedPerSecond - tmpFriction;

                if (xBiggerThan0 != (m_speedPerSecond.x() > 0))
                {
                    m_speedPerSecond.setX(0); // stop
                }
                if (yBiggerThan0 != (m_speedPerSecond.y() > 0))
                {
                    m_speedPerSecond.setY(0); // stop
                }

                invalidate();
            } else
            {
                m_speedPerSecond = Position(0, 0);
            }
            m_drawTime = timeNow;
            Panel::draw(gl);
        }

        void ScrollView::finishdraw(AexRenderer* gl)
        {
            Panel::finishdraw(gl);
        }

        void ScrollView::touchDown(float x, float y, int fingerId)
        {
            // it must be just one finger to move it (remember witch one it was)
            if (m_fingerID < 0)
            {
                m_fingerID = fingerId;
            } else
            {
                return;
            }

            m_touchFocused = true;
            m_touchTimestamp = Frame::timestampSeconds();
            m_sampleTimestamp = m_touchTimestamp;
            m_touchPoint = Position(x, y);
            m_samplePoint = Position(x, y);
            m_maxMotionDelta = 0;
            m_speedPerSecond = Position(0, 0);
            m_downPos = Position(x, y);
            m_lastDelta = m_delta;
        }

        void ScrollView::touchMotion(float x, float y, float dx, float dy, int fingerId)
        {
            if (m_fingerID != fingerId)
                return;

            m_motionTimestamp = Frame::timestampSeconds();
            m_motionPoint = Position(x, y);
            Position tmpDelta = m_motionPoint - m_touchPoint;
            m_maxMotionDelta = std::max(m_maxMotionDelta,  Position::length(m_motionPoint, m_touchPoint) );
            // check that user is try to scrollin or just use inner component:
            if (std::abs(tmpDelta.x()) > std::abs(tmpDelta.y())) // TODO implement another axis too
            {
                if (std::abs(tmpDelta.x()) > 0.05)
                {
                    resendFocus(m_focusPoint, FOCUS_TYPE::TOUCH, m_focusMeAction);
                    return;
                }
            }

            m_delta = m_lastDelta;
            m_delta.move(0, tmpDelta.y());


            // ScrollView scrolling calcs:
            if ((m_motionTimestamp - m_sampleTimestamp) >= ScrollView::SCROLL_SAMPLE_TIME_S)
            {
                double deltaTime = m_motionTimestamp - m_sampleTimestamp;
                Position deltaVec = m_motionPoint - m_samplePoint;
                m_sampleTimestamp = m_motionTimestamp;
                m_samplePoint = m_motionPoint;
                calcSpeed(deltaTime, deltaVec);
            }

            invalidate();
        }

        void ScrollView::touchUp(float x, float y, int fingerId)
        {
            if (m_fingerID != fingerId)
                return;

            m_upTimestamp = Frame::timestampSeconds();
            m_lastDelta = m_delta;
            m_upPoint = Position(x, y);

            Position tmpDelta = m_upPoint - m_touchPoint;
            if ((std::abs(tmpDelta.x()) < SCROLL_EPSILON) && (std::abs(tmpDelta.y()) < SCROLL_EPSILON) && m_maxMotionDelta < SCROLL_EPSILON)
            {
                ComponentPtr comp = resendFocus(m_focusPoint, FOCUS_TYPE::TOUCH, m_focusMeAction);
                if (comp != nullptr)
                {
                    comp->base_touchUp(x, y, fingerId);
                }
                return;
            }

            if ((m_sampleTimestamp == m_touchTimestamp) || ((m_upTimestamp - m_sampleTimestamp) >= ScrollView::SCROLL_SAMPLE_TIME_S)) // if really fast move (duration smaller than ScrollView::SCROLL_SAMPLE_TIME_S)
            {
                double deltaTime = m_upTimestamp - m_sampleTimestamp;
                Position deltaVec = m_upPoint - m_samplePoint;
                calcSpeed(deltaTime, deltaVec);
            }
        }

        void ScrollView::calcSpeed(double deltaTime, Position deltaVec)
        {
            float speedY = deltaVec.y() / deltaTime;
            float speedX = deltaVec.x() / deltaTime;
            m_speedPerSecond = Position(speedX, speedY);
        }

        void ScrollView::touchLine(float x1, float y1, float x2, float y2, int fingerId)
        {

        }

        void ScrollView::touchClick(float x, float y, int fingerId)
        {
            // always resend on click
            ComponentPtr comp = resendFocus(m_focusPoint, FOCUS_TYPE::TOUCH, m_focusMeAction);
            if (comp != nullptr)
            {
                comp->base_touchClick(x, y, fingerId);
            }
            return;
        }

        void ScrollView::touchScale(float scale)
        {

        }

        void ScrollView::focusCatched(FOCUS_TYPE type)
        {

        }

        void ScrollView::focusReleased(FOCUS_TYPE type)
        {
            m_fingerID = -1;
            m_touchFocused = false;
        }

        void ScrollView::setFriction(float friction)
        {
            m_frictionMultiplier = friction;
        }

        void ScrollView::setScroll(float percent0_100)
        {
            percent0_100 = std::max(0.0f, std::min(100.0f, percent0_100));
            percent0_100 /= 100.0f;
            m_speedPerSecond.setY(0);
            float maxDeltaY = getContentSize().h() - getOuterArea().s().h();
            float moveY = maxDeltaY * percent0_100;
            m_delta.setY(-moveY);

            invalidate();
        }

        ComponentPtr ScrollView::setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe)
        {
            if (isHitted(point))
            {
                if (type == FOCUS_TYPE::TOUCH)
                {
                    m_focusMeAction = focusMe;
                    m_focusPoint = point;
                    focusMe(this);
                    return this;
                }
                if (type == FOCUS_TYPE::INPUT)
                {
                    for (unsigned i = 0; i < m_components.size(); i++)
                    {
                        if (m_components[i]->getHitbox().intersectWith(getHitbox()))
                        {
                            ComponentPtr comp = nullptr;
                            comp = m_components[i]->setFocus(point, type, focusMe);
                            if (comp != nullptr)
                                return comp;
                        }
                    }
                }
            }
            return nullptr;
        }

        ComponentPtr ScrollView::resendFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe)
        {
            if (type == FOCUS_TYPE::TOUCH)
            {
                for (unsigned i = 0; i < m_components.size(); i++)
                {
                    ComponentPtr comp = nullptr;
                    comp = m_components[i]->setFocus(point, type, focusMe);
                    if (comp != nullptr)
                    {
                        this->focusReleased(type);
                        comp->base_touchDown(m_focusPoint.x(), m_focusPoint.y(), m_fingerID);
                        return comp;
                    }
                }
            }
            if (type == FOCUS_TYPE::INPUT)
            {

            }
            return nullptr;
        }

        void ScrollView::calcContentSize()
        {
            m_basePositions.clear();
            Position pos = getInnerArea().p();
            float maxW = 0;
            float sumH = 0;
            for (unsigned i = 0; i < m_components.size(); i++)
            {
                ComponentPtr& comp = m_components[i];
                m_basePositions.push_back(pos);

                Area outer = comp->getOuterArea();
                pos.setY(pos.y() + outer.s().h());

                if (maxW < outer.s().w())
                    maxW = outer.s().w();
                sumH += outer.s().h();

                comp->setVisible(false);
            }

            m_contentSize = Size(maxW, sumH);
        }

        const Size& ScrollView::getContentSize() const
        {
            return m_contentSize;
        }

        Position ScrollView::getMaxLeft() const
        {
            return getArea().p();
        }

        Position ScrollView::getMinLeft() const
        {
            float deltaY = getContentSize().h() - getArea().s().h();
            Position minLeft(0.0f, getArea().p().y() - deltaY);
            return minLeft;
        }

        void ScrollView::findVisibleIDIfNeeded()
        {
            if (m_components.empty())
                return;

            const Area& svArea = getArea();
            if (m_visibleID < 0 || m_visibleID >= (int) m_components.size())
            {
                for (unsigned i = 0; i < m_components.size(); i++)
                {
                    Area area = m_components[i]->getArea();
                    area.setPosition(m_basePositions[i] + m_delta);
                    if (svArea.intersectWith(area))
                    {
                        m_visibleID = i;
                        return;
                    }
                }
                m_visibleID = 0;
            }


            Area lastVisible = m_components[m_visibleID]->getArea();
            lastVisible.setPosition(m_basePositions[m_visibleID] + m_delta);
            if (svArea.intersectWith(lastVisible))
                return; // is ok, still visible

            bool finded = false;
            if (lastVisible.p().y() < svArea.p().y()) // last visible is above view, go down
            {
                for (unsigned i = m_visibleID; i < m_components.size(); i++)
                {
                    Area area = m_components[i]->getArea();
                    area.setPosition(m_basePositions[i] + m_delta);
                    if (svArea.intersectWith(area))
                    {
                        m_visibleID = i;
                        finded = true;
                        break;
                    }
                }
            } else
            { // lastVisible is under view // go up
                for (int i = m_visibleID; i >= 0; i--)
                {
                    Area area = m_components[i]->getArea();
                    area.setPosition(m_basePositions[i] + m_delta);
                    if (svArea.intersectWith(area))
                    {
                        m_visibleID = i;
                        finded = true;
                        break;
                    }
                }
            }

            if (!finded)
            {
                m_visibleID = 0;
                LOG_DEBUG("NOT FIDNED !!! so no render ");
            }
        }
    }
}
