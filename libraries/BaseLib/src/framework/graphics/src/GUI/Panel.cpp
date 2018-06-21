#include "aex/gui/Panel.hpp"
#include "aex/gui/BoundingRect.hpp"

namespace aex
{
    namespace gui
    {

        void Panel::invalidate()
        {
            setRefresh(true);
        }

        Panel::Panel()
        {
            m_debugSettings[DEBUG_RECT::HITBOX] = false;
            m_debugSettings[DEBUG_RECT::NORMAL] = false;
            m_debugSettings[DEBUG_RECT::INNER] = false;
            m_debugSettings[DEBUG_RECT::OUTER] = false;

            m_debugColors[DEBUG_RECT::HITBOX] = Color(255, 0, 0);
            m_debugColors[DEBUG_RECT::NORMAL] = Color(255, 255, 0);
            m_debugColors[DEBUG_RECT::INNER] = Color(0, 255, 0);
            m_debugColors[DEBUG_RECT::OUTER] = Color(0, 0, 255);
        }

        Panel::~Panel()
        {
        }

        void Panel::addComponent(ComponentPtr component)
        {
            m_components.push_back(component);
            invalidate();

            // before this was used method with drawPriority, so now draw method use m_drawOrder vector to get elements for drawing
            if (!m_drawOrder.empty())
            {
                putWithPriority(component, DEFAULT_PRIORITY);
            }
        }

        void Panel::setPriority(ComponentPtr component, int priority)
        {
            if (std::find(m_components.begin(), m_components.end(), component) == m_components.end())
            {
                return;
            }

            if (m_drawOrder.empty())
            {
                if (!m_components.empty())
                {
                    for (unsigned i = 0; i < m_components.size(); i++)
                    {
                        putWithPriority(m_components[i], DEFAULT_PRIORITY);
                    }
                }
            }

            removeDrawPriorityComponent(component);
            putWithPriority(component, priority);
        }

        void Panel::removeComponent(ComponentPtr component)
        {
            for (int i = 0; i < (int) m_components.size(); i++)
            {
                if (m_components[i] == component)
                {
                    m_components.erase(m_components.begin() + i);
                    i--;
                }
            }
            removeDrawPriorityComponent(component);
            invalidate();
        }

        bool Panel::contains(ComponentPtr component) const
        {
            for (unsigned i = 0; i < m_components.size(); i++)
            {
                if (component == m_components[i])
                    return true;
            }

            return false;
        }

        void Panel::clear()
        {
            m_components.clear();
            m_drawOrder.clear();
            invalidate();
        }

        void Panel::removeDrawPriorityComponent(ComponentPtr component)
        {
            for (int i = 0; i < (int) m_drawOrder.size(); i++)
            {
                if (m_drawOrder[i].component == component)
                {
                    m_drawOrder.erase(m_drawOrder.begin() + i);
                    i--;
                }
            }
        }

        void Panel::setDebugRect(DEBUG_RECT type, bool visible)
        {
            m_debugSettings[type] = visible;

            for (unsigned i = 0; i < m_components.size(); i++)
            {
                m_components[i]->setDebugRect(type, visible);
            }
        }

        void Panel::setDebugColor(DEBUG_RECT type, const Color& color)
        {
            m_debugColors[type] = color;

            for (unsigned i = 0; i < m_components.size(); i++)
            {
                m_components[i]->setDebugColor(type, color);
            }
        }

        void Panel::putWithPriority(ComponentPtr component, int priority)
        {
            CompPrior cp(component, priority);
            bool added = false;
            for (unsigned i = 0; i < m_drawOrder.size(); i++)
            {
                if (m_drawOrder[i].priority < priority)
                {
                    m_drawOrder.insert(m_drawOrder.begin() + i, cp);
                    added = true;
                    break;
                }
            }
            if (!added)
            {
                m_drawOrder.push_back(cp);
            }
        }

        void Panel::fitToContent()
        {
          if (m_components.empty())
              return;
            setArea(getFitContentArea());
        }

        Area Panel::getFitContentArea() const
        {
          using namespace std;

          if (m_components.empty())
              return Area();

          float left, top, right, bottom;
          left = right = m_components[0]->getOuterArea().p().x();
          top = bottom = m_components[0]->getOuterArea().p().y();

          for (unsigned i = 0; i < m_components.size(); i++)
          {
              float tmpLeft = m_components[i]->getOuterArea().p().x();
              float tmpRight = m_components[i]->getOuterArea().rightUp().x();
              float tmpTop = m_components[i]->getOuterArea().p().y();
              float tmpBottom = m_components[i]->getOuterArea().leftDown().y();

              left = min(left, tmpLeft);
              right = max(right, tmpRight);
              top = min(top, tmpTop);
              bottom = max(bottom, tmpBottom);
          }

          Area newArea(Position(left, top), Size(right - left, bottom - top));
          return newArea;
        }

        void Panel::draw(AexRenderer* gl)
        {
            bool withPriority = !m_drawOrder.empty();
            unsigned size = 0;
            if (withPriority)
                size = m_drawOrder.size();
            else
                size = m_components.size();

            for (unsigned i = 0; i < size; i++)
            {
                ComponentPtr comp = nullptr;
                if (withPriority)
                {
                    comp = m_drawOrder[i].component;
                } else
                {
                    comp = m_components[i];
                }

                if (!comp->isVisible())
                    continue;

                if (comp->needRefresh())
                {
                    comp->forceRefresh();
                }

                comp->drawBackground(gl);
                comp->draw(gl);
                comp->finishdraw(gl);

                if (m_debugSettings[DEBUG_RECT::HITBOX])
                {
                    m_debugRect.reset(new BoundingRect(m_debugColors[DEBUG_RECT::HITBOX], comp->getHitbox(), 0.005f));
                    m_debugRect->forceRefresh();
                    m_debugRect->draw(gl);
                    m_debugRect->finishdraw(gl);
                }
                if (m_debugSettings[DEBUG_RECT::NORMAL])
                {
                    m_debugRect.reset(new BoundingRect(m_debugColors[DEBUG_RECT::NORMAL], comp->getArea(), 0.005f));
                    m_debugRect->forceRefresh();
                    m_debugRect->draw(gl);
                    m_debugRect->finishdraw(gl);
                }
                if (m_debugSettings[DEBUG_RECT::INNER])
                {
                    m_debugRect.reset(new BoundingRect(m_debugColors[DEBUG_RECT::INNER], comp->getInnerArea(), 0.005f));
                    m_debugRect->forceRefresh();
                    m_debugRect->draw(gl);
                    m_debugRect->finishdraw(gl);
                }
                if (m_debugSettings[DEBUG_RECT::OUTER])
                {
                    m_debugRect.reset(new BoundingRect(m_debugColors[DEBUG_RECT::OUTER], comp->getOuterArea(), 0.005f));
                    m_debugRect->forceRefresh();
                    m_debugRect->draw(gl);
                    m_debugRect->finishdraw(gl);
                }
            }
        }

        void Panel::finishdraw(AexRenderer* gl)
        {
        }

        void Panel::needsUpdate()
        {
            for (unsigned i = 0; i < m_components.size(); i++)
            {
                m_components[i]->needsUpdate();
            }
        }

        const std::vector<ComponentPtr>& Panel::getComponents() const
        {
            return m_components;
        }

        void Panel::modifyComponents(std::function<void(ComponentPtr) > action)
        {
            for (unsigned i = 0; i < m_components.size(); i++)
            {
                action(m_components[i]);
            }
        }

        ComponentPtr Panel::setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe)
        {
            if (isHitted(point))
            {
                for (int i = m_components.size() - 1; i >= 0; i--)
                {
                    if (m_components[i]->isVisible())
                        if (m_components[i]->isFocusableFor(type))
                        {
                            ComponentPtr comp = m_components[i]->setFocus(point, type, focusMe);
                            if (comp != nullptr)
                            {
                                return comp;
                            }
                        }
                }
            }
            return nullptr;
        }
    }
}
