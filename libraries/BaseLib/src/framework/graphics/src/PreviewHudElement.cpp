/* 
 * File:   PreviewHudElement.cpp
 * Author: darek
 * 
 * Created on May 14, 2015, 11:17 AM
 */

#include <aex/PreviewHudElement.hpp>
#include "../../platform_specific/include/CamPrevView.hpp"

namespace aex {

    PreviewHudElement::PreviewHudElement(aex::AWindow& window, aex::cam::Camera& cam, aex::cam::FILTER filter, float sizeX, float sizeY, float posX, float posy, bool fullFill) throw (aex::cam::camera_exception) {
        aex::HudElement hudElem(nullptr, sizeX, sizeY, posX, posy);
        if (cam.isOpened() == false) {
            throw aex::cam::camera_exception("camera is not opened, so cant create preview", cam.getCamID());
        }

//      std::unique_ptr<PreviewView> previewUnique = cam.getPreviewView();
//      preview = aex::shared_ptr<PreviewView>(std::move(previewUnique));
        
        int posXPix = getPosXInPixels(window, hudElem);
        int posYPix = getPosYInPixels(window, hudElem);
        
        preview = cam.getPreviewView(filter);
        preview->renderIn(getWindowSize(window), getHudElementSize(getWindowSize(window), hudElem), posXPix, posYPix, fullFill);
    }

    PreviewHudElement::~PreviewHudElement() {
        
    }

    void PreviewHudElement::draw(AexRenderer* gl) {
        preview->draw(gl);
    }

    void PreviewHudElement::finishdraw(AexRenderer* gl) {
        preview->finishdraw(gl);
    }

    void PreviewHudElement::needsUpdate() {
        preview->needsUpdate();
    }
    
    void PreviewHudElement::renderIn(aex::AWindow& window, float sizeX, float sizeY, float posX, float posY, bool fullFill)
    {
        aex::HudElement hudElem(nullptr, sizeX, sizeY, posX, posY);
        int posXPix = getPosXInPixels(window, hudElem);
        int posYPix = getPosYInPixels(window, hudElem);
        preview->renderIn(getWindowSize(window), getHudElementSize(getWindowSize(window), hudElem), posXPix, posYPix, fullFill);
    }
    
    aex::cam::Size PreviewHudElement::getWindowSize(const AWindow& window) const
    {
        return aex::cam::Size(window.w(), window.h());
    }

    aex::cam::Size PreviewHudElement::getHudElementSize(const aex::cam::Size& window, const HudElement& hudElem) const
    {
        return aex::cam::Size(window.getWidth() * hudElem.getSizeX(), window.getHeight() * hudElem.getSizeY());
    }
    
    int PreviewHudElement::getPosXInPixels(const AWindow& window, const HudElement& hudElem) const
    {
        if(window.w() > window.h())
        {
            float ratio = (float)window.w() / (float)window.h();
            return (hudElem.getPosX()/ratio) * window.w();
        }else
        {
            return hudElem.getPosX() * window.w();
        }
    }
    
    int PreviewHudElement::getPosYInPixels(const AWindow& window, const HudElement& hudElem) const
    {
        if(window.h() > window.w())
        {
            float ratio = (float)window.h() / (float)window.w();
            return (hudElem.getPosY()/ratio) * window.h();
        }else
        {
            return hudElem.getPosY() * window.h();
        }
    }
    
    PreviewMovementHudElement::PreviewMovementHudElement(aex::AWindow& window, cam::Camera& cam, cam::FILTER filter, float sizeX, float sizeY, float posX, float posY, bool fullFill) throw(aex::cam::camera_exception) :
    PreviewHudElement(window, cam, filter, sizeX, sizeY, posX, posY, fullFill)
    {
        this->filter = filter;
        if(this->filter != aex::cam::FILTER::MOVEMENT)
        {
            throw std::runtime_error("cant create this preview with filter other than MOVEMENT");
        }
    }
    
    PreviewMovementHudElement::~PreviewMovementHudElement()
    {
    }
    
    void PreviewMovementHudElement::setMovement(float moveVertical, float moveHorizontal)
    {
        aex::cam::FilterMovementData data(this->filter);
        data.moveY = moveVertical;
        data.moveX = moveHorizontal;
        preview->setData(&data);
    }
    
    PixelBufferHudElement::PixelBufferHudElement(cam::Camera& camera) throw(aex::cam::camera_exception) 
    {
        m_pv = camera.getPixelBufferPV();
    }
       
    PixelBufferHudElement::~PixelBufferHudElement()
    {
    }
    
    void PixelBufferHudElement::draw(AexRenderer* gl)
    {
        m_pv->draw(gl);
    }
    
    void PixelBufferHudElement::finishdraw(AexRenderer* gl)
    {
        m_pv->finishdraw(gl);
    }
    
    void PixelBufferHudElement::needsUpdate()
    {
        m_pv->needsUpdate();
    }
    
    void PixelBufferHudElement::setAction(const std::function<void (aex::cam::PixelBuffer&)>& action)
    {
        aex::cam::PixelBufferPreviewView* pbpv = (aex::cam::PixelBufferPreviewView*)m_pv.get();
        pbpv->setAction(action);
    }
}