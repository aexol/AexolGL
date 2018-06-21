#include "aex/gui/CameraView.hpp"
#include "aex/gui/Frame.hpp"

namespace aex
{
    namespace gui
    {

        CameraView::CameraView(const Area& area, aex::cam::Camera& camera, aex::cam::FILTER filter, bool fullFill) :
        Component(area),
        m_cam(camera),
        m_fullFill(fullFill)
        {
            if (camera.isOpened() == false)
            {
                throw aex::cam::camera_exception("camera is not opened, so cant create preview", camera.getCamID());
            }

            m_preview = m_cam.getPreviewView(filter);
        }

        CameraView::~CameraView()
        {
        }

        void CameraView::setFullFill(bool fullFill)
        {
            m_fullFill = fullFill;
            setRefresh(true);
        }

        void CameraView::refresh(int flags)
        {
//            if (sizeChanged())
//            {
//                // should change PreviewView renderIn method to works with just move delta so always reset vertex now 
//            }

            int widthPix = Frame::norm2Pix(getInnerArea().s().w());
            int heightPix = Frame::norm2Pix(getInnerArea().s().h());

            int posX = Frame::norm2Pix(getInnerArea().p().x());
            int posY = Frame::norm2Pix(getInnerArea().p().y());
            
            aex::cam::Size screenSize(Frame::getWidthPixels(), Frame::getHeightPixels());
            aex::cam::Size previewSize(widthPix, heightPix);
            
            m_preview->renderIn(screenSize, previewSize, posX, posY, m_fullFill);
        }

        void CameraView::draw(aex::AexRenderer* gl)
        {
            m_preview->draw(gl);
        }

        void CameraView::finishdraw(aex::AexRenderer* gl)
        {
            m_preview->finishdraw(gl);
        }

        void CameraView::needsUpdate()
        {
            m_preview->needsUpdate();
        }

        ComponentPtr CameraView::setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe)
        {
            return nullptr; // Not focusable YET - TODO
        }
    }
}


