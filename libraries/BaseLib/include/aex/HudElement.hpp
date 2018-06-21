/*
 * File:   HudElement.hpp
 * Author: mritke
 *
 * Created on December 6, 2013, 3:35 PM
 */

#ifndef HUDELEMENT_HPP
#define	HUDELEMENT_HPP

#include <aex/Common.hpp>
#include <aex/AexDrawable.hpp>
#include <aex/ShaderDrw.hpp>
#include <aex/GameEvent.hpp>
#include <array>



namespace aex
{

    class HudElement;
    class VertexData;
    class IndexData;
    class Texture;
    typedef aex::shared_ptr<HudElement> HudElement_ptr;

    enum HorizontalAlignment
    {
        LEFT_EDGE = 0,
        RIGHT_EDGE = 1,
        CENTER_H = 2
    };

    enum VeriticalAlignment
    {
        TOP_EDGE = 0,
        BOTTOM_EDGE = 1, 
        CENTER_V = 2
    };

    enum ELEMENT_TYPE
    {
        BUTTON = 0,
        RADIO_BUTTON = 1,
        CHECK_BOX = 2,
        DROP_LIST = 3,
        LABEL = 4,
        TEXT_FIELD = 5,
        SLIDER = 6,
        PROGRESS_BAR = 7
    };

    class AEX_PUBLIC_API HudHitbox
    {
        friend class HudElement;
    public:
        HudHitbox( aex::math::Vector2 posoffset, aex::math::Vector2 sizeoffset);
        HudHitbox( aex::math::Vector2 posoffset, aex::math::Vector2 sizeoffset, bool positionRelative, bool sizeRelative);
        HudHitbox();

        void SetSizeOffset(aex::math::Vector2 sizeOffset);
        aex::math::Vector2 GetSizeOffset();
        void SetPositionOffset(aex::math::Vector2 positionOffset);
        aex::math::Vector2 GetPositionOffset();
        void SetSizeRelativeToElement(bool SizeRelativeToElement);
        bool IsSizeRelativeToElement() const;
        void SetPositionRelativeToElement(bool PositionRelativeToElement);
        bool IsPositionRelativeToElement() const;
        
    protected:

        aex::math::Vector2 m_sizeOffset;
        aex::math::Vector2 m_positionOffset;
        bool m_isPositionRelativeToElement = true;
        bool m_isSizeRelativeToElement = true;
 
    };

    typedef aex::shared_ptr<HudHitbox> HudHitbox_ptr;
    
    class AEX_PUBLIC_API HudElement : public DrawObject
    {
        //typedef VertexData* VertexDataVBO;
        //typedef IndexData* IndexDataVBO;

    public:
        HudElement();
        HudElement(Texture* tex, float sizeX, float sizeY, float posX, float posy);
        HudElement(const aex::string& name, float uvOffsetX, float uvOffsetY, float uvOffsizeX, float uvOffsizeY, float sizeX, float sizeY, float posX, float posy);
        ~HudElement();
        static HudElement_ptr makeHudElement(Texture* tex, float sizeX, float sizeY, float posX, float posy);
        static HudElement_ptr makeHudElement(const aex::string& name, float uvOffsetX, float uvOffsetY, float uvOffsizeX, float uvOffsizeY, float sizeX, float sizeY, float posX, float posy);
        static HudElement_ptr makeFullScreenHudElement(Texture* tex);
        void setQuatCamera(const aex::shared_ptr<aex::QuaternionCamera>& camera);
        void SetSpriteAnchorPointToCenter();

        virtual void draw(AexRenderer* gl);
        virtual void finishdraw(AexRenderer* gl);

        void needsUpdate()
        {
        }

        void update()
        {
        }
        void printDebug();
        void setPos(float posX, float posY);
        void addPos(float posX, float posY);
        void setPosY(float posY);
        void scale(float sclX, float sclY);
        void cut(float cutX, float cutY);
        aex::math::Vector2 GetPosition();
        void setColor(float r,float g,float b,float a); 
        void setColor(aex::math::Vector3 rgb,float a);
        void setColor(aex::math::Vector3 rgb);
        void setColor(float r,float g,float b); 
        aex::math::Vector3 getColor() const; 
        float getAlpha() const;
        void  setAlpha(float alpha);
        void SetPosition(aex::math::Vector2 pos);
        void SetPosition(float posX, float posY);
        aex::math::Vector2 getUV();
        aex::math::Vector2 getUvOffset();
        float getPosY() const;
        void setPosX(float posX);
        float getPosX() const;
        void setSize(aex::math::Vector2 size);
        void setSize(float sizeX, float sizeY);
        void addSize(float sizeX, float sizeY);
        void setSizeY(float sizeY);
        float getSizeY() const;
        void setSizeX(float sizeX);
        float getSizeX() const;
        void setTextureAtlas(bool textureAtlas);
        bool isTextureAtlas() const;
        void setTouched(bool Touched);
        bool isTouched() const;
        void setShader(const ShaderDrw& shader);
        ShaderDrw& getShader();

        void setCanChangeDepthTestState(bool canChangeState)
        {
            m_canChangeDepthTestState = canChangeState;
        }

        bool isCanChangeDepthTestState() const
        {
            return m_canChangeDepthTestState;
        }
        void setName(const aex::string& name);
        aex::string getName() const;
        void setCanChangeBlendState(bool canChangeBlendState);
        bool isCanChangeBlendState() const;
        virtual void setClickable(bool Clickable);
        bool isClickable() const;
        void setTextureSamplerUniform(aex::string& name);

        virtual void setVisible(bool Visible)
        {
            m_isVisible = Visible;
        }

        bool isVisible() const
        {
            return m_isVisible;
        }
        
        bool toggleVisiblility() 
        {
            m_isVisible=!m_isVisible;
            return m_isVisible;
        }
        
        bool isPointIsInElement(const aex::math::Vector2& point, const aex::math::Vector2& baseOffset, const aex::math::Vector2& baseSize);
        bool isPointIsInElement(const aex::math::Vector2& point);
        void setPrintDebug(bool pdebug);
        void setNeedOwnShader(bool needOwnShader);
        bool isNeedOwnShader() const;
        void setTargetBlendState(bool targetBlendState);
        bool isTargetBlendState() const;
        void setTargetDepthTestState(bool targetDepthTestState);
        bool isTargetDepthTestState() const;
        void setTexture(Texture* t);
        void setTextureRaw(Texture* t);
        void setHudElemOnTouch(HudElement_ptr hudElem);
        void setHitbox(HudHitbox_ptr hitbox);
        HudHitbox_ptr getHitbox() const;
        void logTexture();
        void setCustomVertexData(const std::vector<float>& data);
        void setCustomUVData(const std::vector<float>& data);
        void setCustomIndexData(const std::vector<uint16_t>& tris);

        virtual void onDown(float posX, float posY);
        virtual void onMotion(float posX, float posY);
        virtual void onUp(float posX, float posY);
        virtual void onClick(float posX, float posY);
        void setUvVbo(VertexData* uvVbo);
        
    private:
        static std::atomic<uint32_t> m_nextId;
    protected:
        HudElement_ptr m_hudElemOnTouch;
        uint32_t m_elementID;
        int m_zIndex = 0;
        float m_sizeX;
        float m_sizeY;

        float m_posX;
        float m_posY;

        float m_uvOffsetX;
        float m_uvOffsetY;

        float m_uvOffsizeX;
        float m_uvOffsizeY;

        float m_WidthOriginalratio;
        float m_Heightriginalratio;


        float m_cutX = 1.0f;
        float m_cutY = 1.0f;
        bool m_isCut = false;


        aex::string m_name;
        ShaderDrw* m_shader=nullptr;
        bool m_shaderLoaded;
        VertexData* m_spriteVbo;
        VertexData* m_uvVbo=nullptr;
        IndexData* m_indexVbo;
        
        bool m_customVertex=false;
        bool m_customIndex=false;

        aex::shared_ptr<UniformFloat2> m_PosUniform;
        aex::shared_ptr<UniformFloat2> m_SizeUniform;
        aex::shared_ptr<UniformFloat2> m_UvOffsetUniform;
        aex::shared_ptr<UniformFloat2> m_UvSizeUniform;
        aex::shared_ptr<UniformFloat3> m_colorModUniform;
        aex::shared_ptr<UniformFloat>  m_alphaUniform;
        aex::shared_ptr<UniformSampler> m_textureSamplerUniform;

        bool m_textureAtlas=true;
        bool m_isDepthTestEnabledChange;
        bool m_centerPosition;
        Texture* m_hudElementTex = nullptr;
        QuaternionCamera* m_quatCamera;
        UniformMatrix4fv m_ProjectionMatrixU;
        //        static const std::array<float, 12> SimpleHudElementVertCornerRoot;
        //        static const std::array<float, 12> SimpleHudElementVertCenterRoot;
        //        static const std::array<float, 8> SimpleHudElementUV;
        //        static const std::array<uint16_t, 6> SimpleHudElementIndex;
        //        static const std::array<uint16_t, 4> SimpleHudElementIndexTriangleStrip;

        bool m_isVisible=true;
        bool m_isTouched=false;
        bool m_isClickable = true;
        bool m_canChangeDepthTestState=true;
        bool m_canChangeBlendState;
        bool m_blendStatechanged = false;
        bool m_targetBlendState = true;
        bool m_targetDepthTestState = false;
        bool m_ownTexture= false;
        bool m_gotHitbox = false;
        bool m_printDebug=false;
        HudHitbox_ptr m_hitbox;
        
        void drawWithTextureAtlas(AexRenderer* gl);
        void drawWithoutTextureAtlas(AexRenderer* gl);

        aex::shared_ptr<GameEvent> m_Event;
        bool m_needOwnShader = true;
        std::function<void(ShaderDrw*) > m_cleanup;
        int temptext = 0;
    };
}

#endif	/* HUDELEMENT_HPP */
