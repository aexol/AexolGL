/* 
 * File:   ShadowMap.hpp
 * Author: mritke
 *
 * Created on 25 listopada 2015, 11:11
 */

#ifndef SHADOWMAP_HPP
#define	SHADOWMAP_HPP

#include <aex/Common.hpp>
#include <aex/AexDrawable.hpp>
#include <aex/ShaderDrw.hpp>
#include <aex/FrameBuffer.hpp>
#include <aex/Material.hpp>
#include <aex/HudElement.hpp>
#include <aex/Texture.hpp>
#include <aex/UniformMatrix2fv.hpp>
#include <aex/QuaternionCamera.hpp>
#include <aex/Box.hpp>
#include <aex/PostEffect.hpp>
#include <aex/DrawNode.hpp>

namespace aex
{
    enum Light_Type
    {
       NONE_LIGHT=0, DIRECTIONAL_LIGHT=1, CONE_LIGHT=2, POINT_LIGHT=3
    };
    
    
    
    class ShadowMap;
    
    class AEX_PUBLIC_API LightFrustum
    {
        public:
            LightFrustum(){}
            LightFrustum(float resX, float resY, bool isCube, Light_Type type);
            virtual ~LightFrustum();
            virtual void computeFrustum() = 0;
            virtual void draw(AexRenderer* gl, RenderingTreeNode* parent) = 0;
            virtual void finishdraw(AexRenderer* gl) = 0;
            virtual Texture* getTexture()= 0;
            void update();
            
            void setPosition(aex::math::Vector3& position);
            void move(aex::math::Vector3& vector);

            void setRotation(aex::math::Vector3& rotation);
            void rotate(aex::math::Vector3& vector);
            
            virtual bool updateCamera(AexRenderer* gl)=0;
            void setNeedsUpdate(bool needsUpdate);
            bool isNeedsUpdate() const;
            void setType(Light_Type type);
            
            aex::math::Matrix getViewPersepcitveMatrix();
            Light_Type getType() const;
            
        protected:
            FrameBufferOGL m_fbo;
            QuaternionCamera m_camera;
            Light_Type m_type;
            bool m_needsUpdate=true;
            float m_xResolution = 512.0f,m_yResolution = 512.0f;
            float m_nearPlane,m_farPlane;
            Texture m_texture;

    };
    
    class AEX_PUBLIC_API ConeLightFrustum :public LightFrustum
    {
    protected:
    public:
        ConeLightFrustum();
        ConeLightFrustum(float resX, float resY, Vector3 possition, Vector3 direction);
        void computeFrustum();
        void draw(AexRenderer* gl, RenderingTreeNode* parent);
        void finishdraw(AexRenderer* gl);
        Texture* getTexture();
        bool updateCamera(AexRenderer* gl);
        
    };
    
    class AEX_PUBLIC_API DirectionalLightFrustum :public LightFrustum
    {  
        
    public:
        DirectionalLightFrustum();
        DirectionalLightFrustum(float resX, float resY, float ortoSizeX, float ortoSizeY, Vector3 possition, Vector3 direction);
        void computeFrustum();
        void draw(AexRenderer* gl,  RenderingTreeNode* parent);
        void finishdraw(AexRenderer* gl);
        Texture* getTexture();
        bool updateCamera(AexRenderer* gl);
        
    private:
        Vector3 m_position;
        Vector3 m_direction;
        aex::math::Vector2 m_orhoSize;
    };
    
    class AEX_PUBLIC_API PointLightFrustum :public LightFrustum
    {
    public:
        
        PointLightFrustum();
        PointLightFrustum(float resX, float resY, Vector3 possition);
        void computeFrustum();
        void draw(AexRenderer* gl, RenderingTreeNode* parent) ;
        void finishdraw(AexRenderer* gl);
        Texture* getTexture();
        bool updateCamera(AexRenderer* gl);
        
    private:
        static const std::vector<Vector3> m_directions;
        static const std::vector<Vector3> m_upVector;
        std::vector<aex::unique_ptr<FrameBufferOGL>> m_fbos;
        std::array<FrameBufferOGL*,6> m_fbosPick;
        int m_currentPlane = 0;
        Vector3 m_position;
        bool m_oneBuffer = false;
    
    };
    

    
    class AEX_PUBLIC_API ShadowMap : public DrawObject
    {
 
    public:
        ShadowMap();
        ShadowMap(int x, int y, Light_Type = Light_Type::POINT_LIGHT);
        ShadowMap(int x, int y, int type = 3);
        ShadowMap(int x, int y, float ortoSizeX, float ortoSizeY, aex::math::Vector3 position, aex::math::Vector3 direction);
        ShadowMap& operator=(const ShadowMap& shadowmap);
        ~ShadowMap();
        void init();
        void setSamplerName(const std::string& mapName);
        void draw(AexRenderer* gl);
        void finishdraw(AexRenderer* gl);

        void needsUpdate()
        {
            
        }
        
        ShaderDrw* makeShadowMapShader(const ShaderDrw* shdr);
        void setUseBlur(bool isblur);
        void setSelfDraw(bool selfDraw);
        void setCullFrontFace(bool cullface);
        bool isSelfDraw() const;
        void setLightFrustrum(LightFrustum* lightFrustrum);
        LightFrustum* getLightFrustrum() const;
        void setDiffuseInMaterial(Material& material);
        UniformSampler* getSampler();
        void setUpdateStatic(bool updateStatic);
        void updateStatic();
        bool isUpdateStatic() const;
        void setStatic(bool Static);
        bool isStatic() const;
        void setCustomFragmentShader(const aex::string& customFragmentShader);
        aex::string getCustomFragmentShader() const;
        void setPassID(int passID);
        int getPassID() const;
        std::string getPassName() const;
        void setFirstPass(bool firstPass);
        bool isFirstPass() const;
        void setShadowMask(Texture* shadowMask);
        Texture* getShadowMask() const;
        Texture* getTexture(); 
        TextureFilter*  getBlurrFilter() const;
        //operator=(ShadowMap& map);
    private:    
    
    protected:
        bool m_firstPass=true;
        bool m_selfDraw=true;
        bool m_initialized=false;
        bool m_mapInProgress=false;
        bool m_isStatic=false;
        bool m_updateStatic=true;
        bool m_cullFrontFace = true;
        bool m_useBlur=true;
        int m_passID = -1;
        std::string m_passname = "shadowMap";
        Texture* m_textureDepth = nullptr;;
        Texture* m_shadowMask = nullptr;
        aex::unique_ptr<Texture> m_blackTexture=nullptr;
        LightFrustum* m_lightFrustrum = nullptr;
        UniformSampler m_depthSamplerUniform;
        UniformSampler m_maskUniform;
        UniformMatrix4fv m_lightMatrix;
        aex::unique_ptr<ShaderDrw> m_shadowMapShader=nullptr;
        ShaderDrw* m_oldShader;
        QuaternionCamera* m_oldQuatCamera;
        TextureFilter_ptr m_blurrFilter;
        aex::string m_customFragmentShader;
        
        bool m_debug=false;
        
        void onContextDirty();
        
        static const aex::math::Matrix m_biasMatrix;
        static const aex::string m_fragmentShader;   
    };
    
}

#endif	/* SHADOWMAP_HPP */

