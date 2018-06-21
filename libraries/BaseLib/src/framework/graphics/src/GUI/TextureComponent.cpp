#include "aex/gui/TextureComponent.hpp"

#include <bitset>

namespace aex
{
    namespace gui
    {
        TextureComponent::TextureComponent(const Area& area, Texture* texture) :
        Component(area)
        {
            m_batch.refillBatch(getArea().inPosition(Position(0,0)), m_batch.getFullTextureUVsArea());
            m_mesh.reset(new TextureMesh(texture, m_batch));
            m_mesh->setDelta(area.leftUp());
        }

        TextureComponent::~TextureComponent()
        {
        }

        void TextureComponent::setTexture(Texture* texture)
        {
            m_mesh->setTexture(texture);
        }

        void TextureComponent::setFlip(bool xAxis, bool yAxis)
        {
          m_batch.setFlipAreas(xAxis, yAxis);
          setFlag(FLAG::SIZE_CHANGED);
        }

        void TextureComponent::draw(aex::AexRenderer* gl)
        {
            m_mesh->draw(gl);
        }

        void TextureComponent::finishdraw(aex::AexRenderer* gl)
        {
            m_mesh->finishdraw(gl);
        }

        void TextureComponent::needsUpdate()
        {
            m_mesh->needsUpdate();
        }

        void TextureComponent::refresh(int flags)
        {
            if(checkFlag(FLAG::SIZE_CHANGED, flags) )
            {
                m_batch.refillBatch(getArea().inPosition(Position(0,0)), m_batch.getFullTextureUVsArea());
                m_mesh->setBatch(m_batch);
            }

            m_mesh->setDelta(getArea().leftUp());
        }

        ComponentPtr TextureComponent::setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe)
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
