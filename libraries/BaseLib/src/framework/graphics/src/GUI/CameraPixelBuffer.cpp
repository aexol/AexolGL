#include "aex/gui/CameraPixelBuffer.hpp"

namespace aex
{
    namespace gui
    {
        CameraPixelBuffer::CameraPixelBuffer(
                aex::cam::Camera& camera,
                bool isOneShot,
                bool takeShot,
                aex::cam::FILTER filter,
                const aex::cam::Size& fboSize
                ):
            m_bIsOneShot(isOneShot),
            m_bTakeShot(takeShot) 
        {
            m_preview = camera.getPixelBufferPV(fboSize, filter);
        }

        CameraPixelBuffer::CameraPixelBuffer(aex::cam::Camera& camera,
                aex::cam::FILTER filter,
                const aex::cam::Size& fboSize) : 
        CameraPixelBuffer(camera,
                false,
                false,
                filter,
                fboSize)
        {
        }
        
        CameraPixelBuffer::CameraPixelBuffer(aex::cam::Camera& camera,
                bool isOneShot) :
        CameraPixelBuffer(camera,
                isOneShot,
                false,
                aex::cam::FILTER::NONE,
                aex::cam::Size(0,0))
        {
        }

        CameraPixelBuffer::~CameraPixelBuffer()
        {
            
        }

        void CameraPixelBuffer::setAction(const std::function<void (aex::cam::PixelBuffer&)>& action)
        {
            m_preview->setAction(action);
        }

        void CameraPixelBuffer::refresh(int flags)
        {
            // Nothing here, PixelBuffer is just virtual "component"
        }
        
        void CameraPixelBuffer::takeShot()
        {
            m_bTakeShot = true;
        }

        ComponentPtr CameraPixelBuffer::setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe)
        {
            return nullptr;
        }

        void CameraPixelBuffer::draw(aex::AexRenderer* gl)
        {
            if(!m_bIsOneShot || m_bTakeShot)
            {
                m_preview->draw(gl);
            }
        }

        void CameraPixelBuffer::finishdraw(aex::AexRenderer* gl)
        {
            m_preview->finishdraw(gl);
            m_bTakeShot = false;
        }

        void CameraPixelBuffer::needsUpdate()
        {
            m_preview->needsUpdate();
        }
    }
}
