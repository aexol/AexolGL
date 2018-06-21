#include "aex/gui/Sprite.hpp"

namespace aex
{
    namespace gui
    {

        Sprite::~Sprite()
        {
        }

        Sprite::Sprite(const aex::gui::Area& area, const aex::gui::Area& UVs, const std::string& tMapPath, aex::TextureConfig tCfg) :
        Component(area),
        m_tMapHandler(aex::gui::TextureManager::get(tMapPath, tCfg))
        {
            m_UVs = UVs;
            m_batch.refillBatch(getArea().inPosition(Position(0,0)), m_UVs);
            m_tMap = m_tMapHandler.get();
            m_mesh.reset(new TextureMesh(m_tMap, m_batch));
            m_mesh->setDelta(area.leftUp());
        }

        void Sprite::setColorMod(const Color& color)
        {
          m_mesh->setColorMod(color);
        }

        const Color& Sprite::getColorMod() const
        {
          return m_mesh->getColorMod();
        }

        void Sprite::refresh(int flags)
        {
            if (checkFlag(FLAG::SIZE_CHANGED, flags))
            {
                m_batch.refillBatch(getArea().inPosition(Position(0,0)), m_UVs);
                m_mesh->setBatch(m_batch);
            }

            m_mesh->setDelta(getArea().leftUp());
        }

        void Sprite::draw(AexRenderer* gl)
        {
            m_mesh->draw(gl);
        }

        void Sprite::finishdraw(AexRenderer* gl)
        {
            m_mesh->finishdraw(gl);
        }

        void Sprite::needsUpdate()
        {
        }

        ComponentPtr Sprite::setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe)
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
