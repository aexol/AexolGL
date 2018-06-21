#include "aex/gui/CardsPanel.hpp"
#include "aex/gui/Frame.hpp"

namespace aex
{
    namespace gui
    {

        CardsPanel::CardsPanel(const Area& area, float swappingTimeSec, int fboWidthPix, int fboHeightPix) :
        m_currentCardID(0),
        m_swappingTimeSec(swappingTimeSec),
        m_showNext(false),
        m_timestamp(0),
        m_currentDelta(0),
        m_fbo1(fboWidthPix, fboHeightPix, true), // TODO parametrize this
        m_fbo2(fboWidthPix, fboHeightPix, true),
        m_viewsBuffered(false),
        m_firstTex(area, nullptr),
        m_secondTex(area, nullptr),
        m_movingFinished(false),
        m_easing(0.0, 1.0, aex::math::Easings::EasingFuncType::EASEING_QUADRIC),
        m_side(SIDE::RIGHT)
        {
            setArea(area);
        }

        CardsPanel::~CardsPanel()
        {
        }

        void CardsPanel::next()
        {
            next(std::function<void()>());
        }

        void CardsPanel::next(const std::function<void()>& onMoved)
        {
            if (m_showNext)
                return;

            if (m_components.size() > m_currentCardID + 1)
            {
                m_onMovedAction = onMoved;
                m_timestamp = 0;
                m_currentDelta = 0;
                m_showNext = true;
                m_viewsBuffered = false;
                m_movingFinished = false;
                m_firstComponent = m_components[m_currentCardID];
                m_secondComponent = m_components[m_currentCardID + 1];
                m_firstComponent->setVisible(true);
                m_secondComponent->setVisible(true);
            }
        }

        ComponentPtr CardsPanel::getCurrentVisible() const
        {
            return m_components.size() == 0 ? nullptr :
                m_components[m_currentCardID];
        }

        void CardsPanel::setCurrentVisible(uint id)
        {
            if(m_components.size() == 0)
                return;
            m_currentCardID = id % m_components.size();
            setRefresh(true);
        }

        void CardsPanel::setSide(CardsPanel::SIDE side)
        {
            m_side = side;
        }

        void CardsPanel::reset()
        {
            clear();
            m_currentCardID = 0;
            m_showNext = false;
            m_timestamp = 0;
            m_currentDelta = 0;
            m_viewsBuffered = false;
        }

        void CardsPanel::addComponent(ComponentPtr component)
        {
            if (m_components.size() != 0)
            {
                component->setVisible(false);
            } else
            {
                component->setVisible(true);
            }
            Panel::addComponent(component);
        }

        void CardsPanel::refresh(int flags)
        {
            if (m_showNext)
            {

                Position delta;
                Position secondPos;
                switch (m_side)
                {
                    case SIDE::RIGHT:
                        delta = Position(m_currentDelta, 0);
                        secondPos = getArea().rightUp();
                        break;
                    case SIDE::UP:
                        delta = Position(0, m_currentDelta);
                        secondPos = getArea().leftUp() - Position(0, getArea().s().h());
                        break;
                    case SIDE::DOWN:
                        delta = Position(0, m_currentDelta);
                        secondPos = getArea().leftDown();
                        break;
                    case SIDE::LEFT:
                        delta = Position(m_currentDelta, 0);
                        secondPos = getArea().leftUp() - Position(getArea().s().w(), 0);
                    default:
                        break;
                }
                m_firstTex.setSize(Size(getArea().s().w(), getArea().s().h()));
                m_secondTex.setSize(Size(getArea().s().w(), getArea().s().h()));

                m_firstTex.setPosition(getArea().leftUp() - delta);
                m_secondTex.setPosition(secondPos - delta);

                m_firstTex.forceRefresh();
                m_secondTex.forceRefresh();
            } else
            {
                for (size_t i = 0; i < m_components.size(); i++)
                {
                    if (i != m_currentCardID)
                        m_components[i]->setVisible(false);
                    else
                    {
                        m_components[m_currentCardID]->setVisible(true);
                        m_components[m_currentCardID]->setArea(this->getArea());
                    }
                }
            }
        }

        ComponentPtr CardsPanel::setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe)
        {
            return nullptr;
        }

        void CardsPanel::draw(aex::AexRenderer* gl)
        {
            if (m_showNext)
            {
                if (!m_viewsBuffered)
                {
                    m_firstComponent->setPosition(getInnerArea().leftUp());
                    m_firstComponent->forceRefresh();

                    m_secondComponent->setPosition(getInnerArea().leftUp()); // rightSide of left
                    m_secondComponent->forceRefresh();

                    m_fbo1.draw(gl);
                    aex::clearColorBuffer();
                    m_firstComponent->draw(gl);
                    m_firstComponent->finishdraw(gl);
                    m_fbo1.finishdraw(gl);

                    m_fbo2.draw(gl);
                    aex::clearColorBuffer();
                    m_secondComponent->draw(gl);
                    m_secondComponent->finishdraw(gl);
                    m_fbo2.finishdraw(gl);

                    m_firstTex.setTexture(m_fbo1.getTexture());
                    m_secondTex.setTexture(m_fbo2.getTexture());
                    m_viewsBuffered = true;
                }

                forceRefresh();
                m_firstTex.draw(gl);
                m_firstTex.finishdraw(gl);
                m_secondTex.draw(gl);
                m_secondTex.finishdraw(gl);

                if (m_movingFinished)
                {
                    m_currentCardID++;
                    m_currentDelta = 0;
                    m_showNext = false;
                    m_movingFinished = false;
                    if(m_onMovedAction)
                        m_onMovedAction(); // invocation
                    invalidate();
                } else
                {
                    if (m_timestamp == 0)
                    {
                        m_timestamp = Frame::timestampSeconds();
                    }
                    float deltaTime = Frame::timestampSeconds() - m_timestamp;
                    float perc = deltaTime / m_swappingTimeSec;
                    m_easing.compute(perc);
                    float value = m_easing.getValue();
                    float maxDelta = 0;

                    switch (m_side)
                    {
                        case SIDE::RIGHT:
                            m_currentDelta = m_firstComponent->getOuterArea().s().w() * value;
                            maxDelta = m_firstComponent->getOuterArea().s().w();
                            break;
                        case SIDE::UP:
                            m_currentDelta = -m_firstComponent->getOuterArea().s().h() * value;
                            maxDelta = -m_firstComponent->getOuterArea().s().h();
                            break;
                        case SIDE::DOWN:
                            m_currentDelta = m_firstComponent->getOuterArea().s().h() * value;
                            maxDelta = m_firstComponent->getOuterArea().s().h();
                            break;
                        case SIDE::LEFT:
                            m_currentDelta = -m_firstComponent->getOuterArea().s().w() * value;
                            maxDelta = -m_firstComponent->getOuterArea().s().w();
                        default:

                            break;

                    }

                    if (deltaTime > m_swappingTimeSec)
                    {
                        m_currentDelta = maxDelta;
                        m_movingFinished = true;
                    }
                }
            } else
            {
                Panel::draw(gl);
                Panel::finishdraw(gl);
            }

        }

        void CardsPanel::finishdraw(aex::AexRenderer* gl)
        {
        }

        void CardsPanel::needsUpdate()
        {
            Panel::needsUpdate();
        }
    }
}
