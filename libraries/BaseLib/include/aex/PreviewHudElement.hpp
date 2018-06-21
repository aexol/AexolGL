/* 
 * File:   PreviewHudElement.hpp
 * Author: darek
 *
 * Created on May 14, 2015, 11:17 AM
 */

#ifndef PREVIEWHUDELEMENT_HPP
#define	PREVIEWHUDELEMENT_HPP

#include <aex/Common.hpp>
#include <aex/AexolGL.hpp>
#include <aex/Camera.hpp>
#include <aex/HudElement.hpp>
#include <aex/AWindow.hpp>

namespace aex {
    class PreviewHudElement : public HudElement {
    public:
        PreviewHudElement(aex::AWindow& window, cam::Camera& cam, cam::FILTER filter, float sizeX, float sizeY, float posX, float posy, bool fullFill = false) throw (aex::cam::camera_exception);
        virtual ~PreviewHudElement();

        virtual void draw(AexRenderer* gl);
        virtual void finishdraw(AexRenderer* gl);
        virtual void needsUpdate();
        virtual void renderIn(aex::AWindow& window, float sizeX, float sizeY, float posX, float posY, bool fullFill);
                    
        aex::cam::Size getWindowSize(const AWindow& window) const;
        aex::cam::Size getHudElementSize(const aex::cam::Size& window, const HudElement& hudElem) const;
        int getPosXInPixels(const AWindow& window, const HudElement& hudElem) const;
        int getPosYInPixels(const AWindow& window, const HudElement& hudElem) const;
        
    protected:
        //aex::shared_ptr<PreviewView> preview;
        std::unique_ptr<aex::cam::PreviewView> preview;
    };
    
    class PreviewMovementHudElement : public PreviewHudElement
    {
    public:
        PreviewMovementHudElement(aex::AWindow& window, cam::Camera& cam, cam::FILTER filter, float sizeX, float sizeY, float posX, float posY, bool fullFill = false) throw(aex::cam::camera_exception);
        virtual ~PreviewMovementHudElement();
        void setMovement(float moveVertical, float moveHorizontal);
    protected:
        aex::cam::FILTER filter; // to check that can send data to specific preview
    };
    
    class PixelBufferHudElement : public HudElement
    {
    public:
        PixelBufferHudElement(cam::Camera& camera) throw(aex::cam::camera_exception);
        virtual ~PixelBufferHudElement();
        
        virtual void draw(AexRenderer* gl);
        virtual void finishdraw(AexRenderer* gl);
        virtual void needsUpdate();
        
        void setAction(const std::function<void (aex::cam::PixelBuffer&)>& action);
        
        // tu dodac ustawianie 
    protected:
        std::unique_ptr<aex::cam::PreviewView> m_pv;
    };
}

#endif	/* PREVIEWHUDELEMENT_HPP */

