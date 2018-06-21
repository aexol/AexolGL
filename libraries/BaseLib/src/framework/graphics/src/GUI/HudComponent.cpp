#include "aex/gui/HudComponent.hpp"
#include <aex/Hud.hpp>
#include "aex/gui/Frame.hpp"

namespace aex
{
    namespace gui
    {

        HudComponent::HudComponent(const Hud_ptr& hud)
        {
            setHud(hud);
        }

        void HudComponent::setHud(const Hud_ptr& hud)
        {
            m_hud = hud;
            hud->setCanChangeDepthTestState(false); // Panels are managing depth test state
            float MAX = std::numeric_limits<float>::max();
            float MIN = std::numeric_limits<float>::min();

            float minY = MAX, maxY = MIN, minX = MAX, maxX = MIN;
            std::vector<HudElement_ptr>& elements = m_hud->getElements();
            for (HudElement_ptr& he : elements)
            {
                using namespace std;
                float nX = he->getPosX();
                float nY = he->getPosY();
                float nW = he->getSizeX();
                float nH = he->getSizeY();

                minY = min(minY, nY);
                maxY = max(maxY, nY + nH);
                minX = min(minX, nX);
                maxX = max(maxX, nX + nW);

                Position initPos(Frame::n2x(nX), Frame::n2y(nY));
                m_initialPositions[he.get()] = initPos;
                m_initialSizes[he.get()] = Size(he->getSizeX(), he->getSizeY());
            }

            float width = Frame::n2x(maxX - minX);
            float height = Frame::n2y(maxY - minY);

            setSize(Size(width, height));
        }

        HudComponent::HudComponent(const std::string& texMapPath, const std::string& jsonPath, aex::TextureConfig texConfig)
        {
            const Hud_ptr& hhhud = HudUtils::getHud(texMapPath, jsonPath, texConfig);
            setHud(hhhud);
        }

        HudComponent::~HudComponent()
        {

        }

        void HudComponent::refresh(int flags)
        {
            Position leftUp = getInnerArea().p();
            std::vector<HudElement_ptr>& elements = m_hud->getElements();

            if ((m_scale.sX() != 1.0) && (m_scale.sY() != 1.0))
            {
                for (HudElement_ptr& he : elements)
                {
                    Area a = Area(m_initialPositions[he.get()] + leftUp, m_initialSizes[he.get()]);
                    a.scale(m_scale);
                    he->setPos(Frame::x2N(a.p().x()), Frame::y2N(a.p().y()));
                    he->setSize(a.s().w(), a.s().h());
                }
            } else
            {
                for (HudElement_ptr& he : elements)
                {
                    Position pos = m_initialPositions[he.get()];
                    pos = pos + leftUp;
                    he->setPos(Frame::x2N(pos.x()), Frame::y2N(pos.y()));
                }
            }
        }

        void HudComponent::setScale(const Scale& s)
        {
            m_scale = s;
            setRefresh(true);
        }

        // DrawObject Interface

        void HudComponent::draw(AexRenderer* gl)
        {
            m_hud->draw(gl);
        }

        void HudComponent::finishdraw(AexRenderer* gl)
        {
            m_hud->finishdraw(gl);
        }

        void HudComponent::needsUpdate()
        {
            m_hud->needsUpdate();
        }

        void HudComponent::hideAll()
        {
            std::vector<HudElement_ptr>& elements = m_hud->getElements();
            for (HudElement_ptr& he : elements)
            {
                he->setVisible(false);
            }
        }

        void HudComponent::showAll()
        {
            std::vector<HudElement_ptr>& elements = m_hud->getElements();
            for (HudElement_ptr& he : elements)
            {
                he->setVisible(true);
            }
        }

        void HudComponent::setVisibleFor(const std::string& hudElemName, bool visible)
        {
            HudElement_ptr heptr = m_hud->getElementByName(hudElemName);
            if (heptr)
                heptr->setVisible(visible);
        }

        void HudComponent::touchDown(float x, float y, int fingerId)
        {

        }

        void HudComponent::touchMotion(float x, float y, float dx, float dy, int fingerId)
        {

        }

        void HudComponent::touchUp(float x, float y, int fingerId)
        {

        }

        void HudComponent::touchLine(float x1, float y1, float x2, float y2, int fingerId)
        {

        }

        void HudComponent::touchClick(float x, float y, int fingerId)
        {

        }

        void HudComponent::touchScale(float scale)
        {

        }

        ComponentPtr HudComponent::setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe)
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