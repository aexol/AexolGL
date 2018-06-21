/* 
 * File:   SliderHudEleent.cpp
 * Author: darek
 * 
 * Created on May 31, 2015, 11:17 AM
 */

#include "aex/SliderHudElement.hpp"

namespace aex
{

    using namespace aex::gui;

    SliderHudElement::SliderHudElement(float sizeX, float sizeY, float posX, float posY, aex::AWindow& window, HudElementPtr sliderButton = nullptr) :
    HudElement(nullptr, sizeX, sizeY, posX, posY),
    position(0.0f),
    bActive(false),
    paddingPercent(0.45),
    sliderButton(sliderButton),
    bDrawBackground(true),
    m_frameSizeNormal(window.w() < window.h() ? 1.0f : (float)window.w() / (float)window.h(), window.h() < window.w() ? 1.0 : (float)window.h() / (float)window.w()),
    sliderArea(aex::FPoint2D(posX, posY), FPoint2D(sizeX, sizeY)),
    hitboxArea(aex::FPoint2D(posX, posY), FPoint2D(sizeX, sizeY)),
    backgroundRect(sliderArea.getInternalAreaWithSizeXPadding(paddingPercent * 0.90), ColorRGB(0.0f, 0.0f, 0.0f), m_frameSizeNormal),
    filledRect(sliderArea.getInternalAreaWithSizeXPadding(paddingPercent).getCuttedAreaAfterPosY(posY + sizeY / 2), ColorRGB(0.1333f, 0.8431f, 0.298f), m_frameSizeNormal),
    emptyRect(sliderArea.getInternalAreaWithSizeXPadding(paddingPercent).getCuttedAreaBeforePosY(posY + sizeY / 2), ColorRGB(0.5f, 0.5f, 0.5f), m_frameSizeNormal)
    
    {
        if (sizeX > sizeY)
        {
            bHorizontal = true;
        } else
        {
            bHorizontal = false;
        }
        
        float widthNormal = 0.0f;
        float heightNormal = 0.0f;
        
        m_frameSizeNormal = FPoint2D(widthNormal, heightNormal);

        NormalArea bNA = sliderArea.getInternalAreaWithSizeXPadding(paddingPercent * 0.90);
        bNA.setNeedNormalize(false);
        backgroundRect.setNormalArea(bNA, m_frameSizeNormal);
        updateSlider(posX + sizeX / 2, posY + sizeY / 2);
    }

    SliderHudElement::~SliderHudElement()
    {
        
    }

    void SliderHudElement::draw(AexRenderer* gl)
    {
        if(bDrawBackground)
            backgroundRect.draw(gl);
        
        filledRect.draw(gl);
        emptyRect.draw(gl);
        // sliderButton->draw(gl); rendered by Hud
    }
    
    void SliderHudElement::finishdraw(AexRenderer* gl)
    {
        // sliderButton->finishdraw(gl); // rendered by Hud
        emptyRect.finishdraw(gl);
        filledRect.finishdraw(gl);
        
        if(bDrawBackground)
            backgroundRect.finishdraw(gl);
    }

    void SliderHudElement::needsUpdate()
    {
        
    }

    void SliderHudElement::setHitbox(const aex::gui::NormalArea& hitbox)
    {
        hitboxArea = hitbox;
    }
    
    const aex::gui::NormalArea& SliderHudElement::getHitboxArea() const
    {
        return hitboxArea;
    }
    
    void SliderHudElement::setPosition(float position)
    {
        position = (position < 0.0f) ? 0.0f : position;
        position = (position > 1.0f) ? 1.0f : position;
        
        updateSlider(getPosX() + getSizeX()*position, getPosY() + (getSizeY() - getSizeY()*position) , false);
    }
    
    float SliderHudElement::getPosition() const
    {
        return position;
    }

    void SliderHudElement::onDown(float posX, float posY) {
        onDown(posX, posY, true);
    }

    void SliderHudElement::onDown(float posX, float posY, bool bUpdate)
    {
        if( ! isClickable())
            return;
        
        if (hitboxArea.isInArea(aex::FPoint2D(posX, posY)))
        {
            bActive = true;
            if(bUpdate)
                updateSlider(posX, posY);
        }
    }

    void SliderHudElement::onMotion(float posX, float posY)
    {
        if (!bActive)
            return;

        updateSlider(posX, posY);
    }

    void SliderHudElement::onUp(float posX, float posY)
    {
        bActive = false;
    }
    
    void SliderHudElement::onClick(float posX, float posY)
    {
        
    }

    bool SliderHudElement::isHorizontal() const
    {
        return bHorizontal;
    }

    void SliderHudElement::setOnChangeCallback(const std::function<void(float) >& cb)
    {
        onChangeAction = cb;
    }
    
    void SliderHudElement::setBackgroundColor(const aex::gui::ColorRGB& color)
    {
        backgroundRect.setColor(color);
    }
    void SliderHudElement::setFilledColor(const aex::gui::ColorRGB& color)
    {
        filledRect.setColor(color);
    }
    void SliderHudElement::setEmptyColor(const aex::gui::ColorRGB& color)
    {
        emptyRect.setColor(color);
    }
    void SliderHudElement::setDrawBackground(bool drawBg)
    {
        bDrawBackground = drawBg;
    }
    
    bool SliderHudElement::isActive() const
    {
        return bActive;
    }

    void SliderHudElement::updateSlider(float posX, float posY,  bool invokeCallback)
    {
        NormalArea filledArea;
        NormalArea emptyArea;
        if (bHorizontal)
        {
            filledArea = sliderArea.getInternalAreaWithSizeYPadding(paddingPercent).getCuttedAreaBeforePosX(posX);
            emptyArea = sliderArea.getInternalAreaWithSizeYPadding(paddingPercent).getCuttedAreaAfterPosX(posX);
        } else
        {
            // SWAP (odwrotnie tu)
            emptyArea = sliderArea.getInternalAreaWithSizeXPadding(paddingPercent).getCuttedAreaBeforePosY(posY);
            filledArea = sliderArea.getInternalAreaWithSizeXPadding(paddingPercent).getCuttedAreaAfterPosY(posY);

            if (sliderButton)
            {
                float sliderY = filledArea.pos.y();
                float sliderX = sliderArea.pos.x() + (sliderArea.size.x() / 2);

                sliderY -= sliderButton->getSizeY() / 2;
                sliderX -= sliderButton->getSizeX() / 2;

                sliderButton->setPos(sliderX, sliderY);
            }
        }

        filledArea.setNeedNormalize(false);
        emptyArea.setNeedNormalize(false);
        filledRect.setNormalArea(filledArea, m_frameSizeNormal);
        emptyRect.setNormalArea(emptyArea, m_frameSizeNormal);
        
        if(invokeCallback)
            calcPositionInvokeCallback();     
    }

    void SliderHudElement::calcPositionInvokeCallback()
    {
        if (bHorizontal)
        {
            position = filledRect.getNormalArea().size.x() / (filledRect.getNormalArea().size.x() + emptyRect.getNormalArea().size.x());
        } else
        {
            position = filledRect.getNormalArea().size.y() / (filledRect.getNormalArea().size.y() + emptyRect.getNormalArea().size.y());
        }

        // callback
        if (onChangeAction)
        {
            onChangeAction(position);
        }
    }
}