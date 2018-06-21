#include <aex/PostEffect.hpp>
#include <aex/Uniforms.hpp>
#include <aex/Logger.hpp>
#include <aex/TextureManager.hpp>

#define USE_NPOT true 

namespace aex {


    PostEffect::PostEffect(ShaderDrw* shader) :
    m_texOffset(std::vector<float>{
        0.0f, 0.0f, 1.0f, 1.0f
    }, "texOffset"),
    m_shader(nullptr),
    m_shaderRaw(shader),
    m_bufferArea{
        FPoint2D
        {0.0f, 0.0f},
        FPoint2D
        {1.0f, 1.0f}
    }
    {
        init();
    }

    PostEffect::PostEffect(int x, int y, ShaderDrw* shader) :
    m_fbo(x, y, USE_NPOT),
    m_texOffset(std::vector<float>{
        0.0f, 0.0f, 1.0f, 1.0f
    }, "texOffset"),
    m_shader(nullptr),
    m_shaderRaw(shader),
    m_bufferArea{
        FPoint2D
        {0.0f, 0.0f},
        FPoint2D
        {1.0f, 1.0f}
    }
    {
        init();
    }

    PostEffect::PostEffect(int x, int y, ShaderDrw* shader, bool npotAllowed) :
    m_fbo(x, y, npotAllowed),
    m_texOffset(std::vector<float>{
        0.0f, 0.0f, 1.0f, 1.0f
    }, "texOffset"),
    m_shader(nullptr),
    m_shaderRaw(shader),
    m_bufferArea{
        FPoint2D
        {0.0f, 0.0f},
        FPoint2D
        {1.0f, 1.0f}
    }
    {
        init();
    }


    PostEffect::PostEffect(ShaderDrw_ptr shader) :
    m_texOffset(std::vector<float>{
        0.0f, 0.0f, 1.0f, 1.0f
    }, "texOffset"),
    m_shader(shader),
    m_shaderRaw(shader.get()),
    m_bufferArea{
        FPoint2D
        {0.0f, 0.0f},
        FPoint2D
        {1.0f, 1.0f}
    }
    {
        init();
    }

    PostEffect::PostEffect(int x, int y, ShaderDrw_ptr shader) :
    m_fbo(x, y, USE_NPOT),
    m_texOffset(std::vector<float>{
        0.0f, 0.0f, 1.0f, 1.0f
    }, "texOffset"),
    m_shader(shader),
    m_shaderRaw(shader.get()),
    m_bufferArea{
        FPoint2D
        {0.0f, 0.0f},
        FPoint2D
        {1.0f, 1.0f}
    }
    {
        init();

    }

    /*PostEffect& PostEffect::operator=(const PostEffect& r)
    {
        FrameBufferOGL m_fbo;
        aex::shared_ptr<UniformSampler> m_depthSamplerUniform;
        UniformMatrix2fv m_texOffset;
        m_usingDepthTexture = r.m_usingDepthTexture;
        m_usingColorTexture = r.m_usingDepthTexture;
        m_selfDraw = r.m_usingDepthTexture;
       
        m_shader = r.m_shader;
        m_shaderRaw = r.m_shaderRaw;
        m_bufferArea= r.m_bufferArea;
        
        std::vector<Texture*> m_optionalTextures;
        
        std::vector<aex::shared_ptr<UniformSampler>> m_optionalSamplerUniforms;
        
    }*/

    PostEffect::~PostEffect() noexcept
    {
        aex::TextureManager& tm = aex::TextureManager::GetInstance();
        for (uint32_t i = 0; i < m_optionalTextures.size(); ++i)
        {
            tm.releaseTexture(m_optionalTextures[i]);
        }
    }

    Texture* PostEffect::getColorAttachment(int number)
    {
        return m_fbo.getColorAttachment(number);
    }

    Texture* PostEffect::getLastColorAttachment()
    {
        return m_fbo.getColorAttachment(m_fbo.getCollorAttachmentsSize());
    }

    void PostEffect::init()
    {
        m_texture = m_fbo.getTexture();
        m_textureDepth = m_fbo.getTextureDepth();

        if (m_shader == nullptr && m_shaderRaw == nullptr)
        {
            m_shader = BasicPostEffectShader();
            m_shaderRaw = m_shader.get();
        }

        m_depthSamplerUniform = aex::make_shared<UniformSampler>(m_textureDepth, "depthBuff");
        m_hudElement = aex::make_shared<HudElement>(m_texture, 1.0f, 1.0f, 0.0f, 0.0f);
        m_hudElement->setNeedOwnShader(false);
        m_hudElement->setCanChangeDepthTestState(true);
        m_hudElement->setCanChangeBlendState(false);

    }

    void PostEffect::setShader(ShaderDrw* shader)
    {
        if (m_shader)
        {
            m_shader.reset();
        }
        m_shaderRaw = shader;
    }

    void PostEffect::setShader(ShaderDrw_ptr shader)
    {
        m_shader = shader;
        m_shaderRaw = m_shader.get();
    }

    void PostEffect::draw(AexRenderer* gl)
    {
        if (!gl->isColorPick())
        {

            m_fbo.draw(gl);

        }

    }

    void PostEffect::finishdraw(AexRenderer* gl)
    {
        if (!gl->isColorPick())
        {

            FBox2D buffer_area = m_fbo.bufferArea();

            if (m_bufferArea != buffer_area)
            {
                m_bufferArea = buffer_area;
                std::array<float, 4> bufferAreaValue{
                    {
                        buffer_area[0][0], buffer_area[1][0],
                        buffer_area[0][1], buffer_area[1][1]
                    }};
                m_texOffset.setValue(bufferAreaValue);
            }

            if (m_texture != m_fbo.getTexture())
            {
                m_texture = m_fbo.getTexture();
                m_hudElement->setTexture(m_texture);
            }

            if (m_postEffectFunc)
            {
                m_postEffectFunc(gl);
            }

            m_fbo.finishdraw(gl);

            if (m_selfDraw)
            {
                m_shaderRaw->draw(gl);

                const ShaderDrw* shader = gl->getShader();

                if (shader)
                {
                    const auto& program = shader->GetProgram();
                    
                    if (m_usingDepthTexture)
                    {
                        Texture* depthTexture = m_fbo.getTextureDepth();
                        depthTexture->bind();
                        m_depthSamplerUniform->setValue(depthTexture);
                        m_depthSamplerUniform->setUniform(program);
                    }

                    for (uint32_t i = 0; i < m_optionalTextures.size(); ++i)
                    {
                        m_optionalTextures[i]->bind();
                    }

                    for (uint32_t i = 0; i < m_optionalSamplerUniforms.size(); ++i)
                    {
                        m_optionalSamplerUniforms[i]->setUniform(program);
                    }

                    m_texOffset.setUniform(program);
                }

                m_hudElement->draw(gl);
                m_hudElement->finishdraw(gl);

                for (uint32_t i = 0; i < m_optionalTextures.size(); ++i)
                {
                    m_optionalTextures[i]->unbind();
                }

                if (m_usingDepthTexture)
                {
                    Texture* depthTexture = m_fbo.getTextureDepth();
                    depthTexture->unbind();
                }
             
                m_shaderRaw->finishdraw(gl);
            }
        }
    }

    void PostEffect::setTextureSize(int x, int y)
    {
        m_fbo.setTextureSize(x, y);
    }

    void PostEffect::setScreenSize(int x, int y)
    {
        m_fbo.setScreenSize(x, y);
    }

    void PostEffect::setHudElement(HudElement_ptr hudElement)
    {
        m_hudElement = hudElement;
    }

    HudElement_ptr PostEffect::getHudElement() const
    {
        return m_hudElement;
    }

    void PostEffect::addOptionalTexture(const aex::string& texname, const aex::string& uniformSamplerName)
    {

        aex::TextureManager& tm = aex::TextureManager::GetInstance();

        TextureConfig::makeDefaultTextureConfig();
        Texture* newTex = tm.getTexture(texname);
        addOptionalTexture(newTex, uniformSamplerName);

    }

    bool PostEffect::addColorAttachment(const std::string& uniformSamplerName, bool selfdraw)
    {
        Texture* newTex = m_fbo.addColorAttachment();

        if (newTex)
        {
            if (selfdraw)
            {
                addOptionalTexture(newTex, uniformSamplerName);
            }
            return true;
        } else
        {
            LOG_ERR("Too much color attachement for framebuffer");
            return false;
        }
    }

    void PostEffect::addOptionalTexture(Texture* newTex, const aex::string& uniformSamplerName)
    {

        m_optionalTextures.push_back(newTex);
        aex::shared_ptr<UniformSampler> newsampler = aex::make_shared<UniformSampler>(newTex, uniformSamplerName);
        m_optionalSamplerUniforms.push_back(newsampler);
    }

    static const aex::string texturedPostEffectHeader =
            "varying vec2 v_texCoord;\n"
            "uniform vec2 position;\n"
            //"uniform mat2 texOffset;\n"
            ;

    void PostEffect::setSelfDraw(bool selfDraw)
    {
        m_selfDraw = selfDraw;
    }

    bool PostEffect::isSelfDraw() const
    {
        return m_selfDraw;
    }

    void PostEffect::setUsingDepthTexture(bool usingDepthTexture)
    {
        m_usingDepthTexture = usingDepthTexture;
    }

    bool PostEffect::isUsingDepthTexture() const
    {
        return m_usingDepthTexture;
    }

    void PostEffect::setUsingColorTexture(bool usingColorTexture)
    {
        m_usingColorTexture = usingColorTexture;
    }

    bool PostEffect::isUsingColorTexture() const
    {
        return m_usingColorTexture;
    }

    void PostEffect::setPostEffectFunc(std::function<void(AexRenderer* gl) > postEffectFunc)
    {
        m_postEffectFunc = postEffectFunc;
    }

    Texture* PostEffect::getTexture() const
    {
        return m_texture;
    }

    void addTextureFilter(PostEffect* postEffect, TextureFilter* filter)
    {
//         TODO
//        auto func = [&](AexRenderer * gl) {
//            filter->draw(gl);
//        };
    }

    /////////////////////////////////TextureFilter \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ 

    TextureFilter::TextureFilter() :
    m_hudElement(nullptr)
    {
    }

    TextureFilter::TextureFilter(Texture* texture) :
    m_hudElement(new HudElement(texture, 1.0f, 1.0f, 0.0f, 0.0f))
    {

        if (texture)
        {
            m_size = aex::math::Vector2(texture->getSizeW(), texture->getSizeW());
        }
        m_hudElement->setNeedOwnShader(false);
        m_hudElement->setCanChangeDepthTestState(true);
        // m_hudElement->SetSpriteAnchorPointToCenter();
        setTextureToFilter(texture);
    }

    TextureFilter::TextureFilter(float x, float y) :
    m_size(x, y),
    m_hudElement(nullptr)
    {
        m_buffer.push_back(std::unique_ptr<FrameBufferOGL>(new FrameBufferOGL(x, y, false, false)));
        m_buffer.push_back(std::unique_ptr<FrameBufferOGL>(new FrameBufferOGL(x, y, false, false)));
    }

    TextureFilter::TextureFilter(float x, float y, std::vector<ShaderDrw_ptr> shaders) :
    m_size(x, y),
    m_hudElement(nullptr)
    {
        m_buffer.push_back(std::unique_ptr<FrameBufferOGL>(new FrameBufferOGL(x, y, false, false)));
        m_buffer.push_back(std::unique_ptr<FrameBufferOGL>(new FrameBufferOGL(x, y, false, false)));

        for (auto& shader : shaders)
        {
            addFilter(shader);
        }

        m_outputTexture = m_buffer[(m_filterShaders.size() - 1) % 2]->getTexture();

    }

    TextureFilter::~TextureFilter()
    {
    }

    void TextureFilter::filter(AexRenderer* gl)
    {
        m_hudElement->setTextureRaw(m_textureToFilter);

        //LOG_DEBUG("FILTER: ");
        
        for (size_t b = 0; b < m_filterShaders.size(); ++b)
        {
            int buff = b % 2;

            m_buffer[buff]->draw(gl);

            m_filterShaders[b]->draw(gl);

            if (m_textures.find(0) != m_textures.end())
            {
                std::vector<TextureUniform>& tex = m_textures[0];
                for (size_t i = 0; i < tex.size(); ++i)
                {
                    tex[i].texture->bind();
                    tex[i].uniformSampler.setUniform(*m_filterShaders[b]);
                }
            }
            
            if (m_textures.find(b + 1) != m_textures.end())
            {
                std::vector<TextureUniform>& tex = m_textures[b];
                for (size_t i = 0; i < tex.size(); ++i)
                {
                    tex[i].texture->bind();
                    tex[i].uniformSampler.setUniform(*m_filterShaders[b]);
                }

                m_hudElement->draw(gl);
                m_hudElement->finishdraw(gl);

                for (size_t i = 0; i < tex.size(); ++i)
                {
                    tex[i].texture->unbind();
                }
            } else
            {

                m_hudElement->draw(gl);
                m_hudElement->finishdraw(gl);
            }
            
            if (m_textures.find(0) != m_textures.end())
            {
                std::vector<TextureUniform>& tex = m_textures[0];
                for (size_t i = 0; i < tex.size(); ++i)
                {
                    tex[i].texture->unbind();
                    tex[i].uniformSampler.setUniform(*m_filterShaders[b]);
                }
            }


            m_filterShaders[b]->finishdraw(gl);

            m_buffer[buff]->finishdraw(gl);

            m_hudElement->setTextureRaw(m_buffer[buff]->getTexture());
            m_outputTexture = m_buffer[buff]->getTexture();
        }
        
        
        //LOG_DEBUG("END FILTER: ");
    }

    void TextureFilter::draw(AexRenderer* gl)
    {

        if (!m_filterOnFinish)
        {
            filter(gl);
        }

        //m_outputTexture =m_textureToFilter;
    }

    void TextureFilter::finishdraw(AexRenderer* gl)
    {
        if (m_filterOnFinish)
        {
            filter(gl);
        }
    }

    void TextureFilter::needsUpdate()
    {

    }

    void TextureFilter::addFilter(ShaderDrw_ptr shader)
    {

        m_filterShaders.push_back(shader);
        addFilter(shader.get());

    }

    void TextureFilter::addFilter(ShaderDrw* shader)
    {

        m_filterShadersRaw.push_back(shader);

    }

    Texture* TextureFilter::getOutputTexture() const
    {
        return m_outputTexture;
    }

    void TextureFilter::setTextureToFilter(Texture* textureToFilter)
    {
        if (textureToFilter && m_textureToFilter != textureToFilter)
        {
            m_textureToFilter = textureToFilter;

            if (!m_hudElement)
            {
                m_hudElement = aex::make_shared<HudElement>(m_textureToFilter, 1.0f, 1.0f, 0.0f, 0.0f);
                m_hudElement->setNeedOwnShader(false);
                m_hudElement->setCanChangeDepthTestState(true);
            }

            if (m_buffer.size() == 0)
            {
                LOG_DEBUG("m_buffer.push_back(s1");
                m_buffer.push_back(std::unique_ptr<FrameBufferOGL>(new FrameBufferOGL(m_textureToFilter->getSizeW(), m_textureToFilter->getSizeH(), false, false)));
                LOG_DEBUG("m_buffer.push_back(s1");
                m_buffer.push_back(std::unique_ptr<FrameBufferOGL>(new FrameBufferOGL(m_textureToFilter->getSizeW(), m_textureToFilter->getSizeH(), false, false)));
            }
        }
    }

    Texture* TextureFilter::getTextureToFilter() const
    {
        return m_textureToFilter;
    }

    void TextureFilter::addTexture(int filter, Texture* texture, const std::string& name)
    {
        m_textures[filter].emplace_back(texture, name);
    }


    TextureFilter_ptr TextureFilter::simpleBlurrFilter(Texture* textureToFilter, float sizeX, float sizeY)
    {

        ShaderDrw_ptr blurrH = aex::make_shared<ShaderDrw>(
                "varying vec2 vTexCoord;\n"
                "uniform vec2 position;\n"
                "uniform vec2 size;\n"
                "void main(void)"
                "{"
                "vTexCoord = vec2(gl_Vertex.x, gl_Vertex.y);\n"
                "vec2 Pos=vec2( gl_Vertex.x*size.x, gl_Vertex.y*size.y);\n"
                "mat4 MVP_mat = gl_ProjectionMatrix;\n"
                "gl_Position = MVP_mat*vec4(-Pos.xy-position.xy,0.0,1.0);"

                "}",

                
                "uniform sampler2D hudElement;\n"
                "uniform sampler2D shadowMaskToBlurr;\n"
                "varying vec2 vTexCoord;\n"
                "vec4 blur5(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {"
                "vec4 color = vec4(0.0);"
                "vec2 off1 =  direction;"
                "color += texture2D(image, uv) * 0.29411764705882355;"
                "color += texture2D(image, uv + (off1 / resolution)) * 0.35294117647058826;"
                "color += texture2D(image, uv - (off1 / resolution)) * 0.35294117647058826;"
                "return color;}"

                "float blur9(sampler2D image  , vec2 uv, vec2 resolution, vec2 direction) {"
                "vec2 off1 = vec2(1.3846153846) * direction;"
                "vec2 off2 = vec2(3.2307692308) * direction;"
                "float color = texture2D(image, uv).r * 0.2270270270;"
                "color += texture2D(image, uv + (off1 / resolution)).r * 0.3162162162;"
                "color += texture2D(image, uv - (off1 / resolution)).r * 0.3162162162;"
                "color += texture2D(image, uv + (off2 / resolution)).r * 0.0702702703;"
                "color += texture2D(image, uv - (off2 / resolution)).r * 0.0702702703;"
                "return color;"
                "}"

                "float blur9_2(sampler2D image , sampler2D image2, vec2 uv, vec2 resolution, vec2 direction) {"
                "vec2 off1 = vec2(1.3846153846) * direction;"
                "vec2 off2 = vec2(3.2307692308) * direction;"
                "float color = min(texture2D(image, uv).r,texture2D(image2, uv).r) * 0.2270270270;"
                "color += min(texture2D(image, uv + (off1 / resolution)).r, texture2D(image2, uv + (off1 / resolution)).r) * 0.3162162162;"
                "color += min(texture2D(image, uv - (off1 / resolution)).r, texture2D(image2, uv - (off1 / resolution)).r) * 0.3162162162;"
                "color += min(texture2D(image, uv + (off2 / resolution)).r, texture2D(image2, uv + (off2 / resolution)).r) * 0.0702702703;"
                "color += min(texture2D(image, uv - (off2 / resolution)).r, texture2D(image2, uv - (off2 / resolution)).r) * 0.0702702703;"
                "return color;"
                "}"

                "vec3 unpackColor(float f) {"
                "vec3 color;"
                "f*=256.0*256.0*256.0;"
                "color.b = floor((f / 256.0) / 256.0);"
                "color.g = floor((f - (color.b * 256.0 * 256.0)) / 256.0);"
                "color.r = floor(f - (color.b * 256.0 * 256.0) - (color.g * 256.0));"
                // now we have a vec3 with the 3 components in range [0..256]. Let's normalize it!
                "return color/256.0;\n"
                "}"

                "void main()"
                "{"
                //"float ret = blur9(hudElement, vTexCoord, vec2(" + aex::to_string(sizeX) + ", " + aex::to_string(sizeY) + "),vec2(0.0,1.0));"
                "float ret = blur9_2(hudElement, shadowMaskToBlurr, vTexCoord, vec2(" + aex::to_string(sizeX) +", "+aex::to_string(sizeY) + "),vec2(0.0,1.0));"
                //"float ret = max(color.r, color2.r);"
                "vec4 color =vec4(1.0);"
                "color.rgb= unpackColor(ret);"
                "gl_FragColor = color;"
                "}"
                );

        ShaderDrw_ptr blurrW = aex::make_shared<ShaderDrw>(
                "varying vec2 vTexCoord;\n"
                "uniform vec2 position;\n"
                "uniform vec2 size;\n"
                "void main()"
                "{"
                "vTexCoord = vec2(gl_Vertex.x, gl_Vertex.y);\n"
                "vec2 Pos=vec2( gl_Vertex.x*size.x, gl_Vertex.y*size.y);\n"
                "mat4 MVP_mat =gl_ProjectionMatrix;\n"
                "gl_Position =MVP_mat*vec4(-Pos.xy-position.xy,0.0,1.0);"
                "}",

                "uniform sampler2D hudElement;\n"
    
                "varying vec2 vTexCoord;\n"
                "vec4 blur5(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {"
                "vec4 color = vec4(0.0);"
                "vec2 off1 =  direction;"
                "color += texture2D(image, uv) * 0.29411764705882355;"
                "color += texture2D(image, uv + (off1 / resolution)) * 0.35294117647058826;"
                "color += texture2D(image, uv - (off1 / resolution)) * 0.35294117647058826;"
                "return color;}"

                "float packColor(vec3 color) {"
                "return color.r + color.g * 256.0 + color.b * 256.0 * 256.0;"
                "}\n"

                "float blur9(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {\n"
                "vec2 off1 = vec2(1.3846153846) * direction;\n"
                "vec2 off2 = vec2(3.2307692308) * direction;\n"
                "float color = packColor(texture2D(image, uv).rgb) * 0.2270270270;\n"
                "color += packColor(texture2D(image, uv + (off1 / resolution)).rgb) * 0.3162162162;\n"
                "color += packColor(texture2D(image, uv - (off1 / resolution)).rgb) * 0.3162162162;\n"
                "color += packColor(texture2D(image, uv + (off2 / resolution)).rgb) * 0.0702702703;\n"
                "color += packColor(texture2D(image, uv - (off2 / resolution)).rgb) * 0.0702702703;\n"
                "return color;"
                "}\n"
                
                "vec3 unpackColor(float f) {"
                "vec3 color;"
                "color.b = floor((f / 256.0) / 256.0);"
                "color.g = floor((f - (color.b * 256.0 * 256.0)) / 256.0);"
                "color.r = floor(f - (color.b * 256.0 * 256.0 )- (color.g * 256.0));"
                // now we have a vec3 with the 3 components in range [0..256]. Let's normalize it!
                "return color / 256.0;"
                "}"

                "void main()"
                "{"
                "float ret = blur9(hudElement, vTexCoord,vec2(" + aex::to_string(sizeX) + ", " + aex::to_string(sizeY) + "),vec2(1.0,0.0));"
                "vec4 color = vec4(1.0);"
                "color.rgb = unpackColor(ret);"
                "gl_FragColor = color;"
                "}");

        std::vector<ShaderDrw_ptr> filters;
        filters.push_back(blurrH);
        filters.push_back(blurrW);
        TextureFilter_ptr ret = aex::make_shared<TextureFilter>(int(sizeX), int(sizeY), filters);


        return ret;
    }

    void TextureFilter::setFilterOnFinish(bool filterOnFinish)
    {
        m_filterOnFinish = filterOnFinish;
    }

    bool TextureFilter::isFilterOnFinish() const
    {
        return m_filterOnFinish;
    }

    /////////////////////////////////TextureFilter /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\| 
    static const aex::string getUVcords = ""

            /*"vec2 getUVCorrds() {\n"
             *"   vec2 tt = vec2(gl_Vertex.x, -gl_Vertex.y);\n"
             *"   tt.x = (tt.x+texOffset[0].x)*texOffset[1].x;\n"
             *"   tt.y = (tt.y+texOffset[0].y)*texOffset[1].y;\n"
             *"}\n"*/;

    ShaderDrw_ptr PostEffect::BasicFadeInOutShader(float r, float g, float b)
    {
        return aex::make_shared<ShaderDrw>(
                "uniform vec2 position;\n"
                "uniform vec2 size;\n"
                "void main(void)\n"
                "{\n"
                "vec2 Pos=vec2( gl_Vertex.x*size.x, gl_Vertex.y*size.y);\n"
                "mat4 MVP_mat =gl_ProjectionMatrix;\n"
                "gl_Position =MVP_mat*vec4(-Pos.xy-position.xy,0.0,1.0);"
                "}\n"
                ,
                "uniform float fadeValue;\n"
                "void main(void)\n"
                "{\n"
                "gl_FragColor = vec4(" + aex::to_string(r) + "," + aex::to_string(g) + "," + aex::to_string(b) + ",fadeValue);\n"
                "}\n"
                );
    }

    ShaderDrw_ptr PostEffect::BasicDepthBufferShader()
    {
        return aex::make_shared<ShaderDrw>(
                texturedPostEffectHeader +
                getUVcords +
                "void main(void)\n"
                "{\n"
                "   v_texCoord = getUVCoords();\n"//vec2(gl_Vertex.x, -gl_Vertex.y);\n"
                "vec2 Pos=vec2( gl_Vertex.x, gl_Vertex.y);\n"
                "gl_Position =gl_ProjectionMatrix*vec4(-Pos.xy-position.xy,0.0,1.0);"

                "}\n"
                ,
                "uniform sampler2D depthBuff;\n"
                "varying vec2 v_texCoord;\n"

                "void main(void)\n"
                "{\n"
                "vec4 clr = texture2D( depthBuff, v_texCoord );\n"
                "gl_FragColor=clr;"
                "}\n"
                );
    }

    ShaderDrw_ptr PostEffect::BasicPostEffectShader()
    {
        return aex::make_shared<ShaderDrw>(
                texturedPostEffectHeader +
                getUVcords +
                "void main(void)\n"
                "{\n"
                "v_texCoord = vec2(gl_Vertex.x, -gl_Vertex.y);\n"
                "vec2 Pos=vec2( gl_Vertex.x, gl_Vertex.y);\n"
                "gl_Position =gl_ProjectionMatrix*vec4(-Pos.xy-position.xy,0.0,1.0);"
                "}\n"
                ,
                "uniform sampler2D hudElement;\n"
                "varying vec2 v_texCoord;\n"

                "void main(void)\n"
                "{\n"
                "vec4 clr = texture2D( hudElement, v_texCoord );\n"
                "gl_FragColor=clr;"
                "}\n"
                );
    }

    ShaderDrw_ptr PostEffect::BasicOutlineShader()
    {
        return aex::make_shared<ShaderDrw>(
                texturedPostEffectHeader +
                "uniform vec2 size;\n"
                "varying vec2 v_blurTexCoords[14];" +
                getUVcords +
                "void main(void)\n"
                "{\n"
                "v_texCoord = vec2(gl_Vertex.x, -gl_Vertex.y);\n"
                "vec2 Pos=vec2( gl_Vertex.x*size.x, gl_Vertex.y*size.y);\n"
                "mat4 MVP_mat =gl_ProjectionMatrix;\n"
                "gl_Position =MVP_mat*vec4(-Pos.xy-position.xy,0.0,1.0);"
                "}\n"
                ,
                "uniform sampler2D hudElement;\n"
                "uniform sampler2D depthBuff;\n"
                "varying vec2 v_texCoord;\n"
                "void main(void)\n"
                "{\n"
                "vec4 clr = texture2D( hudElement, v_texCoord );\n"
                "clr += texture2D( hudElement, vec2(v_texCoord.x+1.0/1000.0,v_texCoord.y) );\n"
                "clr += texture2D( hudElement, vec2(v_texCoord.x-1.0/1000.0,v_texCoord.y) );\n"
                "clr += texture2D( hudElement, vec2(v_texCoord.x,v_texCoord.y+1.0/1000.0) );\n"
                "clr += texture2D( hudElement, vec2(v_texCoord.x,v_texCoord.y-1.0/1000.0) );\n"

                "clr = clr/5.0;"
                "   if(clr.a>0.0&&clr.a<1.0) {\n"
                "       clr = 5.0*vec4(1.0,clr.a,0.0,clr.a);\n"
                "       gl_FragColor = clr;\n"
                "   } else {\n"
                "gl_FragColor = vec4(1.0,0.0,0.0,0.0);\n"
                "   }"
                "}\n"
                );
    }

    ShaderDrw_ptr PostEffect::BasicOuterGlowShader()
    {
        return aex::make_shared<ShaderDrw>(
                texturedPostEffectHeader +
                "uniform vec2 size;\n"
                "varying vec2 v_blurTexCoords[14];" +
                getUVcords +
                "void main(void)\n"
                "{\n"
                "v_texCoord = vec2(gl_Vertex.x, -gl_Vertex.y);\n"
                "vec2 Pos=vec2( gl_Vertex.x*size.x, gl_Vertex.y*size.y);\n"
                "mat4 MVP_mat =gl_ProjectionMatrix;\n"
                "   gl_Position =MVP_mat*vec4(-Pos.xy-position.xy,0.0,1.0);\n"
                "}\n"
                ,
                "uniform sampler2D hudElement;\n"
                "uniform sampler2D depthBuff;\n"
                "varying vec2 v_texCoord;\n"
                "const float blurSizeH = 1.0 / 300.0;"
                "const float blurSizeV = 1.0 / 200.0;"
                "void main(void)\n"
                "{\n"
                "   vec4 clr = vec4(0.0);\n"
                "   for (int x = -4; x <= 4; x++)\n"
                "       for (int y = -4; y <= 4; y++)\n"
                "clr += texture2D(   hudElement,  vec2(v_texCoord.x + x * blurSizeH, v_texCoord.y + y * blurSizeV)"
                "               ) / 81.0;\n"
                "   float w = 1.0 -clr.a;\n"
                "clr = vec4(0.5*w,1.0*w,0.5*w,clr.a*w);\n"
                // "clr.a= 0.1;\n"
                "   gl_FragColor = clr*2.0;\n"
                "}\n"
                );

    }

    ShaderDrw_ptr PostEffect::BasicBlurrShader()
    {
        return aex::make_shared<ShaderDrw>(
                texturedPostEffectHeader +
                "uniform vec2 size;\n"
                "varying vec4 v_blurTexCoords[4];" +
                getUVcords +
                "void main(void)\n"
                "{\n"
                "v_texCoord = vec2(gl_Vertex.x, -gl_Vertex.y);\n"
                "vec2 Pos=vec2( gl_Vertex.x*size.x, gl_Vertex.y*size.y);\n"
                "mat4 MVP_mat =gl_ProjectionMatrix;\n"

                "   const float blurSizeH = 1.0 / 1200.0;\n"
                "   const float blurSizeV = 1.0 / 800.0;\n"
                "   v_blurTexCoords[0] = vec4(v_texCoord.x - blurSizeH, v_texCoord.y, v_texCoord.x - blurSizeH, v_texCoord.y - blurSizeV);\n"
                "   v_blurTexCoords[1] = vec4(v_texCoord.x + blurSizeH, v_texCoord.y, v_texCoord.x + blurSizeH, v_texCoord.y + blurSizeV);\n"
                "   v_blurTexCoords[2] = vec4(v_texCoord.x, v_texCoord.y - blurSizeV, v_texCoord.x + blurSizeH, v_texCoord.y - blurSizeV);\n"
                "   v_blurTexCoords[3] = vec4(v_texCoord.x, v_texCoord.y + blurSizeV, v_texCoord.x - blurSizeH, v_texCoord.y + blurSizeV);\n"

                "   gl_Position =MVP_mat*vec4(-Pos.xy-position.xy,0.0,1.0);\n"

                "}\n"
                ,
                "uniform sampler2D hudElement;\n"
                "uniform sampler2D depthBuff;\n"
                "varying vec2 v_texCoord;\n"
                "varying vec4 v_blurTexCoords[4];"
                // "layout (depth_unchanged​) out float gl_FragDepth;"



                "void main(void)\n"
                "{\n"

                // "vec4 clr = texture2D( hudElement, v_texCoord );\n"
                "vec4 colourFromDepthMap = texture2D(depthBuff, v_texCoord.xy);"
                "if(colourFromDepthMap.r > gl_FragCoord.w) discard;"

                "vec4 clr =texture2D(   hudElement, v_texCoord);"
                //"vec4 clr= vec4(0.0);"
                "clr -= texture2D( hudElement,  v_blurTexCoords[0].xy )*0.25;"
                "clr += texture2D( hudElement,  v_blurTexCoords[1].xy )*0.25;"
                "clr -= texture2D( hudElement,  v_blurTexCoords[2].xy )*0.25;"
                "clr += texture2D( hudElement,  v_blurTexCoords[3].xy )*0.25;"
                "clr += texture2D( hudElement,  v_blurTexCoords[0].zw )*0.25;"
                "clr -= texture2D( hudElement,  v_blurTexCoords[1].zw )*0.25;"
                "clr -= texture2D( hudElement,  v_blurTexCoords[2].zw )*0.25;"
                "clr += texture2D( hudElement,  v_blurTexCoords[3].zw )*0.25;"

                // "clr /=3.0;"


                "gl_FragColor = clr;"
                //"gl_FragDepth=texture2D( depthBuff, v_texCoord )-0.1;\n"
                "}\n"
                );

    }

    ShaderDrw_ptr PostEffect::BasicContrastSatShader()
    {
        return aex::make_shared<ShaderDrw>(
                texturedPostEffectHeader +
                "uniform vec2 size;\n" +
                getUVcords +
                "void main(void)\n"
                "{\n"
                "v_texCoord = vec2(gl_Vertex.x, -gl_Vertex.y);\n"
                "vec2 Pos=vec2( gl_Vertex.x*size.x, gl_Vertex.y*size.y);\n"
                "mat4 MVP_mat =gl_ProjectionMatrix;\n"
                "gl_Position =MVP_mat*vec4(-Pos.xy-position.xy,0.0,1.0);"

                "}\n"
                ,

                "uniform float SatValue;"
                "uniform float ConValue;"
                "uniform sampler2D hudElement;"
                "varying vec2 v_texCoord;\n"
                "out varying vec4 color1,color2;"
                "void main()"
                "{"
                "vec4 col = texture2D(hudElement, v_texCoord);"

                // Increase or decrease theese values to adjust r, g and b color channels seperately
                "const float AvgLumR = 0.5;"
                "const float AvgLumG = 0.5;"
                "const float AvgLumB = 0.5;"
                "const vec4 LumCoeff = vec4(0.2125, 0.7154, 0.0721, 1.0);"

                "vec4 AvgLumin = vec4(AvgLumR, AvgLumG, AvgLumB, 1.0);"
                "vec4 brtColor = col * 1.0;"
                "vec4 intensity = vec4(dot(brtColor, LumCoeff));"
                "vec4 satColor = mix(intensity, brtColor, 1.0+SatValue);"
                "vec4 conColor = mix(AvgLumin, satColor, 1.0+ConValue);"

                "gl_FragColor = mix(gl_FragColor, conColor, 1.0);"
                "}"
                );
    }

    ShaderDrw_ptr PostEffect::BasicSSAO()
    {
        return aex::make_shared<ShaderDrw>(
                texturedPostEffectHeader +
                "uniform vec2 size;\n"
                "varying vec2 uv;" +
                getUVcords +
                "void main(void)\n"
                "{\n"
                "uv = vec2(gl_Vertex.x, -gl_Vertex.y);\n"
                "vec2 Pos=vec2( gl_Vertex.x*size.x, gl_Vertex.y*size.y);\n"
                "mat4 MVP_mat =gl_ProjectionMatrix;\n"
                "gl_Position =MVP_mat*vec4(-Pos.xy-position.xy,0.0,1.0);"

                "}\n"
                ,
                "uniform sampler2D hudElement;"
                "uniform sampler2D depthBuff;"
                "uniform sampler2D vignette;"
                "varying vec2 uv;"
                "const float totStrength = 1.38;"
                "const float strength = 0.15;"
                "const float offset = 18.0;"
                "const float falloff = 0.000002;"
                "const float rad = 0.006;"
                "const float invSamples = -1.38/10.0;"
                "void main(void)"
                "{"
                // these are the random vectors inside a unit sphere
                "vec3 pSphere[10] = vec3[](vec3(-0.010735935, 0.01647018, 0.0062425877),"
                "vec3(-0.06533369, 0.3647007, -0.13746321),"
                "vec3(-0.6539235, -0.016726388, -0.53000957),"
                "vec3(0.40958285, 0.0052428036, -0.5591124),"
                "vec3(-0.1465366, 0.09899267, 0.15571679),"
                "vec3(-0.44122112, -0.5458797, 0.04912532),"
                "vec3(0.03755566, -0.10961345, -0.33040273),"
                "vec3(0.019100213, 0.29652783, 0.066237666),"
                "vec3(0.8765323, 0.011236004, 0.28265962),"
                "vec3(0.29264435, -0.40794238, 0.15964167));"

                // grab a normal for reflecting the sample rays later on
                "vec3 fres = normalize((texture2D(vignette,uv*offset).xyz*2.0) - vec3(1.0));\n"
                "vec4 clr = texture2D(hudElement,uv);\n"
                "vec4 currentPixelSample = texture2D(hudElement,uv);\n"

                "float currentPixelDepth = texture2D(depthBuff,uv).r;\n"

                // current fragment coords in screen space
                "vec3 ep = vec3(uv.xy,currentPixelDepth);\n"
                // get the normal of current fragment
                "vec3 norm = currentPixelSample.xyz;\n"

                "float bl = 0.0;"
                // adjust for the depth ( not shure if this is good..)
                "float radD = rad/currentPixelDepth;\n"

                //vec3 ray, se, occNorm;
                "float occluderDepth, depthDifference;\n"
                "float dph;\n"
                "vec4 occluderFragment;\n"
                "vec3 ray;"
                "for(int i=0; i<10;++i)"
                "{"
                // get a vector (randomized inside of a sphere with radius 1.0) from a texture and reflect it
                "ray = radD*reflect(pSphere[i],fres);\n"

                // get the depth of the occluder fragment
                "occluderFragment = texture2D(hudElement,ep.xy + sign(dot(ray,norm) )*ray.xy);\n"
                "dph = texture2D(depthBuff,ep.xy + sign(dot(ray,norm) )*ray.xy).r;\n"
                // if depthDifference is negative = occluder is behind current fragment
                "depthDifference = currentPixelDepth-dph;\n"

                // calculate the difference between the normals as a weight
                // the falloff equation, starts at falloff and is kind of 1/x^2 falling
                "bl += step(falloff,depthDifference)*(1.0-dot(occluderFragment.xyz,norm))*(1.0-smoothstep(falloff,strength,depthDifference));\n"
                "}"

                // output the result
                "float value =1.0f-bl*invSamples;\n"
                "gl_FragColor = clr * value;}\n"
                );
    }



}
