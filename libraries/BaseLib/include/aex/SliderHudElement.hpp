/* 
 * File:   PreviewHudElement.hpp
 * Author: darek
 *
 * Created on May 14, 2015, 11:17 AM
 */

#ifndef SLIDERHUDELEMENT_HPP
#define	SLIDERHUDELEMENT_HPP

#include <array>
#include <functional>

#include <aex/AexolGL.hpp>
#include <aex/HudElement.hpp>
#include <aex/Common.hpp>
#include <aex/AexDrawable.hpp>
#include <aex/ShaderDrw.hpp>
#include <aex/GameEvent.hpp>
#include <aex/HudElement.hpp>
#include <aex/VertexData.hpp>
#include <aex/UniformMatrix4fv.hpp>
#include <aex/UniformSampler.hpp>
#include <aex/ShaderProgram.hpp>

#include <aex/Area.hpp>

namespace aex {
    class SliderHudElement : public HudElement {
    public:
        typedef float clampf;
    public:
        SliderHudElement(float sizeX, float sizeY, float posX, float posY, aex::AWindow& window, HudElementPtr sliderButton);
        virtual ~SliderHudElement();

        virtual void draw(AexRenderer* gl);
        virtual void finishdraw(AexRenderer* gl);
        virtual void needsUpdate();
        
        void setPosition(float position);
        float getPosition() const;
       
        void setHitbox(const aex::gui::NormalArea& hitbox);
        const aex::gui::NormalArea& getHitboxArea() const;
        
        virtual void onDown(float posX, float posY);
        virtual void onDown(float posX, float posY, bool bUpdate);
        virtual void onMotion(float posX, float posY);
        virtual void onUp(float posX, float posY);
        virtual void onClick(float posX, float posY);
        
        bool isHorizontal() const;
        void setOnChangeCallback(const std::function<void(float)>& cb);
        void setBackgroundColor(const aex::gui::ColorRGB& color);
        void setFilledColor(const aex::gui::ColorRGB& color);
        void setEmptyColor(const aex::gui::ColorRGB& color);
        void setDrawBackground(bool drawBg);
        bool isActive() const;
        void setClickable(bool Clickable)
        {
            HudElement::setClickable(Clickable);
            sliderButton->setClickable(Clickable);
        }
        void setVisible(bool Visible)
        {
            HudElement::setVisible(Visible);
            sliderButton->setVisible(Visible);
        }
    private:
        void updateSlider(float posX, float posY, bool invokeCallback = true);
        void calcPositionInvokeCallback();
    private:
        // Logic:
        bool bHorizontal;
        clampf position; // 0.0 to 1.0
        bool bActive;
        clampf paddingPercent;
        HudElementPtr sliderButton;
        bool bDrawBackground;
        
        FPoint2D m_frameSizeNormal;        
        
        aex::gui::NormalArea sliderArea;
        aex::gui::NormalArea hitboxArea;
        aex::gui::GUIRectRGB backgroundRect;
        aex::gui::GUIRectRGB filledRect;
        aex::gui::GUIRectRGB emptyRect;
        

        std::function<void(float)> onChangeAction;
    };
}

#endif	/* SLIDERHUDELEMENT_HPP */

