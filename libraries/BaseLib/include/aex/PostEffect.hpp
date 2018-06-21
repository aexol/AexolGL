/*
 * File:   PostEffect.hpp
 * Author: mritke
 *
 * Created on February 6, 2014, 4:28 PM
 */

#ifndef POSTEFFECT_HPP
#define	POSTEFFECT_HPP

#include <aex/Common.hpp>
#include <aex/AexDrawable.hpp>
#include <aex/ShaderDrw.hpp>
#include <aex/FrameBuffer.hpp>
#include <aex/HudElement.hpp>
#include <aex/UniformMatrix2fv.hpp>
#include <aex/Material.hpp>
#include <aex/Box.hpp>

namespace aex {
    class AEX_PUBLIC_API TextureFilter;
    class AEX_PUBLIC_API PostEffect;
    typedef shared_ptr<PostEffect> PostEffect_ptr;

    class AEX_PUBLIC_API PostEffect : public DrawObject {

    public:
        
        PostEffect(ShaderDrw* shader);
        PostEffect(int x, int y, ShaderDrw* shader);
        PostEffect(int x, int y, ShaderDrw* shader, bool npotAllowed );
        PostEffect(ShaderDrw_ptr shader = nullptr);
        PostEffect(int x, int y, ShaderDrw_ptr shader = nullptr);
        ~PostEffect() noexcept;
        virtual void draw(AexRenderer* gl);

        virtual void finishdraw(AexRenderer* gl);

        void needsUpdate() {
        };

        //PostEffect& operator=(const PostEffect& r);
        void init();
        void setShader(ShaderDrw* shader);
        void setShader(ShaderDrw_ptr shader);
        void setHudElement(HudElement_ptr hudElement);
        HudElement_ptr getHudElement() const;
        void addOptionalTexture(const aex::string& texname, const aex::string& uniformSamplerName);
        void addOptionalTexture(Texture* tex, const aex::string& uniformSamplerName);
        void setTextureSize(int x, int y);
        void setScreenSize(int x, int y);
        void setUsingDepthTexture(bool usingDepthTexture);
        bool isUsingDepthTexture() const;
        void setUsingColorTexture(bool usingColorTexture);
        bool isUsingColorTexture() const;
        void setPostEffectFunc( std::function<void(AexRenderer* gl)> postEffectFunc);
        void setSelfDraw(bool selfDraw);
        bool isSelfDraw() const;

        
        bool addColorAttachment(const std::string& uniformName, bool selfdraw = true);
        
        Texture* getTexture() const;
        Texture* getColorAttachment(int number);
        Texture* getLastColorAttachment();


        static ShaderDrw_ptr BasicFadeInOutShader(float r = 0.0f, float g = 0.0f, float b = 0.0f);
        static ShaderDrw_ptr BasicPostEffectShader();
        static ShaderDrw_ptr BasicDepthBufferShader();
        static ShaderDrw_ptr BasicOutlineShader();
        static ShaderDrw_ptr BasicOuterGlowShader();
        static ShaderDrw_ptr BasicBlurrShader();
        static ShaderDrw_ptr BasicContrastSatShader();
        static ShaderDrw_ptr BasicSSAO();
        
        
    protected:
        
        typedef std::function<void()> PostFunc;
        
        Texture* m_texture;
        Texture* m_textureDepth;
        FrameBufferOGL m_fbo;
        HudElement_ptr m_hudElement;
        aex::shared_ptr<UniformSampler> m_depthSamplerUniform;
        UniformMatrix2fv m_texOffset;
        bool m_usingDepthTexture = false;
        bool m_usingColorTexture = true;
        bool m_selfDraw = true;
        std::vector<Texture*> m_optionalTextures;
        std::vector<aex::shared_ptr<UniformSampler>> m_optionalSamplerUniforms;
        ShaderDrw_ptr m_shader;
        ShaderDrw* m_shaderRaw;
        FBox2D m_bufferArea;
        std::function<void(AexRenderer* gl)> m_postEffectFunc;
        
    };
    
    class AEX_PUBLIC_API TextureFilter;
    typedef aex::shared_ptr<TextureFilter> TextureFilter_ptr;
   
    class AEX_PUBLIC_API TextureFilter : public DrawObject {
  
    public:
        TextureFilter();
        TextureFilter(Texture* texture);
        TextureFilter(float x ,float y);
        TextureFilter(float x ,float y, std::vector<ShaderDrw_ptr> shaders);
        
        ~TextureFilter();
        
        virtual void draw(AexRenderer* gl);

        virtual void finishdraw(AexRenderer* gl);

        virtual void needsUpdate();
        
        void filter(AexRenderer* gl);
        
        void addFilter(ShaderDrw_ptr shader);
        void addFilter(ShaderDrw* shader);
        
        void addTexture(int filter, Texture* texture ,const std::string& name);
        
        Texture* getOutputTexture() const;
        void setTextureToFilter(Texture* textureToFilter);
        Texture* getTextureToFilter() const;
        
        static TextureFilter_ptr simpleBlurrFilter(Texture* textureToFilter, float sizeX = 2048, float sizeY = 2048);
        void setFilterOnFinish(bool filterOnFinish);
        bool isFilterOnFinish() const;
        
    protected:
        Texture* m_textureToFilter = nullptr;
        Texture* m_outputTexture = nullptr;
        aex::math::Vector2 m_size;
        HudElement_ptr m_hudElement;
        
        std::vector<ShaderDrw_ptr> m_filterShaders; 
        std::vector<ShaderDrw*> m_filterShadersRaw; 
        
        std::map<int, std::vector<TextureUniform>> m_textures; 
        
        std::vector<std::shared_ptr<FrameBufferOGL>> m_buffer; 
        void onContextDirty(){}
        
        bool m_filterOnFinish = false; 
        
    };
    
    
    void addTextureFilter(PostEffect* postEffect, TextureFilter* filter);

}

#endif	/* POSTEFFECT_HPP */

