#include <aex/HudElement.hpp>
#include <aex/TextureIncludes.hpp>
#include <aex/TextureManager.hpp>
#include <aex/Logger.hpp>
#include <aex/ShaderProgram.hpp>
#include <aex/VertexData.hpp>

namespace aex {
    std::atomic<uint32_t> HudElement::m_nextId(0);

    static const std::vector<float> SimpleHudElementVertCornerRoot = {
        {
            0.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0
        }
    };
    static const std::vector<float> SimpleHudElementVertCenterRoot = {
        {
            -0.5, -0.5, 0.0,
            -0.5, 0.5, 0.0,
            0.5, 0.5, 0.0,
            0.5, -0.5, 0.0
        }
    };
    static const std::vector<uint16_t> SimpleHudElementIndexTriangleStrip = {
        {1, 2, 0, 3}
    };

    static std::unique_ptr<VertexData> SimpleHudElementVertCornerRootCache{nullptr};
    static uint32_t CornerRootCacheReferences;
    static std::mutex CornerRootLock;
    static std::unique_ptr<VertexData> SimpleHudElementVertCenterRootCache{nullptr};
    static uint32_t CenterRootCacheReferences;
    static std::mutex CenterRootLock;
    static std::unique_ptr<IndexData> SimpleHudElementIndexTriangleStripCache{nullptr};
    static uint32_t IndexStripCacheReferences;
    static std::mutex IndexStripLock;

    static VertexData* getVertCornerRoot()
    {
        std::lock_guard<std::mutex> lock{CornerRootLock};

        if (!SimpleHudElementVertCornerRootCache)
        {
            SimpleHudElementVertCornerRootCache.reset(
                    VertexData::createVertexData(SimpleHudElementVertCornerRoot, "gl_Vertex", 3)
                    );
        }

        CornerRootCacheReferences++;
        return SimpleHudElementVertCornerRootCache.get();
    }

    static void releaseVertCornerRoot()
    {
        std::lock_guard<std::mutex> lock{CornerRootLock};
        CornerRootCacheReferences--;

        if (CornerRootCacheReferences == 0)
        {
            SimpleHudElementVertCornerRootCache.reset();
        }
    }

    static VertexData* getVertCenterRoot()
    {
        std::lock_guard<std::mutex> lock{CenterRootLock};

        if (!SimpleHudElementVertCenterRootCache)
        {
            SimpleHudElementVertCenterRootCache.reset(
                    VertexData::createVertexData(SimpleHudElementVertCenterRoot, "gl_Vertex", 3)
                    );
        }

        CenterRootCacheReferences++;
        return SimpleHudElementVertCenterRootCache.get();
    }

    static void releaseVertCenterRoot()
    {
        std::lock_guard<std::mutex> lock{CenterRootLock};
        CenterRootCacheReferences--;

        if (CenterRootCacheReferences == 0)
        {
            SimpleHudElementVertCenterRootCache.reset();
        }
    }

    static IndexData* getIndexStripVBO()
    {
        std::lock_guard<std::mutex> lock{IndexStripLock};

        if (!SimpleHudElementIndexTriangleStripCache)
        {
            SimpleHudElementIndexTriangleStripCache.reset(
                    new IndexData(IndexData::createTraingleStrip(SimpleHudElementIndexTriangleStrip))
                    );
        }

        IndexStripCacheReferences++;
        return SimpleHudElementIndexTriangleStripCache.get();
    }

    static void releaseIndexStrip()
    {
        std::lock_guard<std::mutex> lock{IndexStripLock};
        IndexStripCacheReferences--;

        if (IndexStripCacheReferences == 0)
        {
            SimpleHudElementIndexTriangleStripCache.reset();
        }
    }

    class sharedHudElementForward : public HudElement
    {
    public:

        sharedHudElementForward(Texture* tex, float sizeX, float sizeY, float posX, float posy) :
        HudElement(tex, sizeX, sizeY, posX, posy)
        {
        }

        sharedHudElementForward(aex::string name, float uvOffsetX, float uvOffsetY, float uvOffsizeX, float uvOffsizeY, float sizeX, float sizeY, float posX, float posy) :
        HudElement(name, uvOffsetX, uvOffsetY, uvOffsizeX, uvOffsizeY, sizeX, sizeY, posX, posy)
        {
        }
    };

    HudHitbox::HudHitbox(aex::math::Vector2 posoffset, aex::math::Vector2 sizeoffset) :
    m_sizeOffset(sizeoffset),
    m_positionOffset(posoffset)
    {

    }

    HudHitbox::HudHitbox(aex::math::Vector2 posoffset, aex::math::Vector2 sizeoffset, bool positionRelative, bool sizeRelative) :
    m_sizeOffset(sizeoffset),
    m_positionOffset(posoffset),
    m_isPositionRelativeToElement(positionRelative),
    m_isSizeRelativeToElement(sizeRelative)
    {

    }

    HudHitbox::HudHitbox()
    {

    }

    void HudHitbox::SetSizeOffset(aex::math::Vector2 sizeOffset)
    {
        m_sizeOffset = sizeOffset;
    }

    aex::math::Vector2 HudHitbox::GetSizeOffset()
    {
        return m_sizeOffset;
    }

    void HudHitbox::SetPositionOffset(aex::math::Vector2 positionOffset)
    {
        m_positionOffset = positionOffset;
    }

    aex::math::Vector2 HudHitbox::GetPositionOffset()
    {
        return m_positionOffset;
    }

    void HudHitbox::SetSizeRelativeToElement(bool SizeRelativeToElement)
    {
        m_isSizeRelativeToElement = SizeRelativeToElement;
    }

    bool HudHitbox::IsSizeRelativeToElement() const
    {
        return m_isSizeRelativeToElement;
    }

    void HudHitbox::SetPositionRelativeToElement(bool PositionRelativeToElement)
    {
        m_isPositionRelativeToElement = PositionRelativeToElement;
    }

    bool HudHitbox::IsPositionRelativeToElement() const
    {
        return m_isPositionRelativeToElement;
    }

    HudElement_ptr
    HudElement::makeHudElement(Texture* tex, float sizeX, float sizeY, float posX, float posy)
    {
        HudElement_ptr ret = aex::make_shared<sharedHudElementForward>(tex, sizeX, sizeY, posX, posy);
        return ret;
    }

    HudElement_ptr
    HudElement::makeFullScreenHudElement(Texture* tex)
    {
        HudElement_ptr ret = aex::make_shared<sharedHudElementForward>(tex, 1.0f, 1.0f, 0.0f, 0.0f);
        return ret;
    }

    HudElement_ptr
    HudElement::makeHudElement(const aex::string& name, float uvOffsetX, float uvOffsetY, float uvOffsizeX, float uvOffsizeY, float sizeX, float sizeY, float posX, float posy)
    {
        HudElement_ptr ret = aex::make_shared<sharedHudElementForward>(name, uvOffsetX, uvOffsetY, uvOffsizeX, uvOffsizeY, sizeX, sizeY, posX, posy);
        return ret;
    }

    HudElement::HudElement() :
    m_hudElemOnTouch(nullptr),
    m_sizeX(0.0f),
    m_sizeY(0.0f),
    m_posX(0.0f),
    m_posY(0.0f),
    m_uvOffsetX(0.0f),
    m_uvOffsetY(0.0f),
    m_uvOffsizeX(0.0f),
    m_uvOffsizeY(0.0f),
    m_WidthOriginalratio(4.0f),
    m_Heightriginalratio(3.0f),
    m_name(""),
    m_shader(nullptr),
    m_spriteVbo(nullptr),
    m_indexVbo(nullptr),
    m_textureAtlas(true),
    m_isDepthTestEnabledChange(false),
    m_centerPosition(false),
    m_hudElementTex(nullptr),
    m_isVisible(true),
    m_isTouched(false),
    m_canChangeDepthTestState(true),
    m_canChangeBlendState(true)
    {

    }

    HudElement::HudElement(const aex::string& name, float uvOffsetX, float uvOffsetY, float uvOffsizeX, float uvOffsizeY, float sizeX, float sizeY, float posX, float posY) :
    //m_shader(ShaderDrw::BasicHudElementShaderOrthoOffCenter()),
    m_hudElemOnTouch(nullptr),
    m_sizeX(sizeX),
    m_sizeY(sizeY),
    m_posX(posX),
    m_posY(posY),
    m_uvOffsetX(uvOffsetX),
    m_uvOffsetY(uvOffsetY),
    m_uvOffsizeX(uvOffsizeX),
    m_uvOffsizeY(uvOffsizeY),
    m_WidthOriginalratio(4.0f),
    m_Heightriginalratio(3.0f),
    m_name(name),
    m_shader(nullptr),
    m_spriteVbo(nullptr),
    m_indexVbo(nullptr),
    m_textureAtlas(true),
    m_isDepthTestEnabledChange(false),
    m_centerPosition(false),
    m_hudElementTex(nullptr),
    m_isVisible(true),
    m_isTouched(false),
    m_canChangeDepthTestState(true),
    m_canChangeBlendState(true)
    {

        m_PosUniform = aex::make_shared<UniformFloat2>("position");
        m_SizeUniform = aex::make_shared<UniformFloat2>("size");
        m_UvOffsetUniform = aex::make_shared<UniformFloat2>("UvOffset");
        m_UvSizeUniform = aex::make_shared<UniformFloat2>("UvSize");
        m_colorModUniform = aex::make_shared<UniformFloat3>("colorMod");
        m_alphaUniform = aex::make_shared<UniformFloat>(1.0, "alphaMod");
    }

    HudElement::HudElement(Texture* tex, float sizeX, float sizeY, float posX, float posy) :
    m_hudElemOnTouch(nullptr),
    m_sizeX(sizeX),
    m_sizeY(sizeY),
    m_posX(posX),
    m_posY(posy),
    m_name(""),
    m_shader(nullptr),
    m_shaderLoaded(false),
    m_spriteVbo(nullptr),
    m_indexVbo(nullptr),
    m_textureAtlas(false),
    m_isDepthTestEnabledChange(false),
    m_centerPosition(false),
    m_hudElementTex(tex),
    m_isVisible(true),
    m_isTouched(false),
    m_isClickable(true),
    m_canChangeDepthTestState(true),
    m_canChangeBlendState(true),
    m_blendStatechanged(false)
    {

        m_spriteVbo = getVertCornerRoot();
        m_indexVbo = getIndexStripVBO();
        m_ProjectionMatrixU.setName("_gl_ProjectionMatrix");
        
        m_UvOffsetUniform = aex::make_shared<UniformFloat2>("UvOffset");
        m_UvSizeUniform = aex::make_shared<UniformFloat2>("UvSize");

        m_PosUniform = aex::make_shared<UniformFloat2>(0.0f, 0.0f, "position");
        m_SizeUniform = aex::make_shared<UniformFloat2>(1.0f, 1.0f, "size");

        m_colorModUniform = aex::make_shared<UniformFloat3>("colorMod");
        m_alphaUniform = aex::make_shared<UniformFloat>(1.0f,  "alphaMod");

        if (m_hudElementTex)
        {
            m_textureSamplerUniform = aex::make_shared<UniformSampler>(m_hudElementTex, "hudElement");
        } else
        {
            m_textureSamplerUniform = aex::make_shared<UniformSampler>(nullptr, "hudElement");
        }

    }

    void HudElement::setCustomVertexData(const std::vector<float>& data)
    {
        if (!m_customVertex)
        {
            if (m_centerPosition)
            {
                releaseVertCenterRoot();
            } else
            {
                releaseVertCornerRoot();
            }
        } else
        {
            delete m_spriteVbo;
        }
        m_spriteVbo = VertexData::createVertexData(data, "gl_Vertex", 3);
        m_customVertex = true;
    }

    void HudElement::setCustomUVData(const std::vector<float>& data)
    {
        if (!m_uvVbo)
        {

        } else
        {
            delete m_uvVbo;
        }
        m_uvVbo = VertexData::createVertexData(data, "gl_TexCoord", 2);
    }

    void HudElement::setCustomIndexData(const std::vector<uint16_t>& tris)
    {
        if (m_indexVbo)
        {
            if (!m_customIndex)
            {
                releaseIndexStrip();
            } else
            {
                delete m_indexVbo;
            }
        }
        m_customIndex = true;
        m_indexVbo = new IndexData(IndexData::createTraingleStrip(SimpleHudElementIndexTriangleStrip));
    }

    HudElement::~HudElement()
    {
        try
        {
            if (!m_textureAtlas)
            {

                if (m_spriteVbo)
                {

                    if (!m_customVertex)
                    {
                        if (m_centerPosition)
                        {
                            releaseVertCenterRoot();
                        } else
                        {
                            releaseVertCornerRoot();
                        }
                    } else
                    {
                        delete m_spriteVbo;
                    }
                }

                if (m_uvVbo)
                {
                    delete m_uvVbo;
                }

                if (m_indexVbo)
                {
                    if (!m_customIndex)
                    {
                        releaseIndexStrip();
                    } else
                    {
                        delete m_indexVbo;
                    }
                }

                if (m_cleanup && m_shader)
                {
                    m_cleanup(m_shader);
                }

                if (m_hudElementTex)
                {
                    TextureManager::GetInstance().releaseTexture(m_hudElementTex);
                }

            } else
            {


            }
            
        } catch (std::exception& e)
        {
            LOG_ERR(e.what());
        }
    }

    void HudElement::setUvVbo(VertexData* uvVbo)
    {
        if (m_uvVbo != uvVbo)
        {
            delete m_uvVbo;
        }

        m_uvVbo = uvVbo;
    }

    void
    HudElement::SetSpriteAnchorPointToCenter()
    {
        m_centerPosition = true;

        if (!m_centerPosition)
        {
            if (m_spriteVbo)
            {
                releaseVertCornerRoot();
            }

            m_spriteVbo = getVertCenterRoot();
        }

        //        .reset(
        //          VertexData::createVertexData(SimpleHudElementVertCenterRoot, "gl_Vertex", 3)
        //          );
        //        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_spriteVbo));
        //        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, SimpleHudElementVertCenterRoot.size() * sizeof (float), SimpleHudElementVertCenterRoot.data(), GL_STATIC_DRAW));
        //        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    void
    HudElement::draw(AexRenderer* gl)
    {
        if (m_hudElemOnTouch)
        {
            if (isTouched())
            {
                return m_hudElemOnTouch->draw(gl);
            }
        }

        if (!m_textureAtlas)
        {
            drawWithoutTextureAtlas(gl);
        } else
        {
            if (m_isVisible)
            {
                drawWithTextureAtlas(gl);
            }
        }
    }

    void
    HudElement::drawWithoutTextureAtlas(AexRenderer* gl)
    {
        if (m_hudElemOnTouch)
        {
            if (isTouched())
            {
                return m_hudElemOnTouch->drawWithoutTextureAtlas(gl);

            }
        }

        if (m_needOwnShader)
        {
            if (!m_shader)
            {
                m_shader = ShaderDrw::BasicHudElementShaderOrthoOffCenter();
                m_cleanup = [](ShaderDrw * s) {
                    UNUSED(s);
                    ShaderDrw::FreeHudElementShaderOrthoOffCenter();
                };
                m_shaderLoaded = true;
            }
            m_shader->draw(gl);
            //gl->estShader(m_shader);
        }

        const auto& shader = gl->getShader();
        if (shader)
        {
            const auto& program = shader->GetProgram();

            m_PosUniform->setValue(m_posX, m_posY);
            m_SizeUniform->setValue(m_sizeX, m_sizeY);
            //m_colormod->setValue((float) m_isTouched, 0.0f, 0.0f);
          

            m_PosUniform->setUniform(program);
            m_SizeUniform->setUniform(program);
            m_alphaUniform->setUniform(program);
            
            if (m_hudElementTex)
            {
                m_hudElementTex->bind();
                m_textureSamplerUniform->setValue(m_hudElementTex);
                m_textureSamplerUniform->setUniform(program);
                m_UvSizeUniform->setValue((float)m_hudElementTex->getSizeW(),(float)m_hudElementTex->getSizeH());
                m_UvSizeUniform->setUniform(program);
            }

            m_ProjectionMatrixU.setValue(gl->getQuatCamera()->get_ortho_matrix().getM());
            //LOG_DEBUG("gl->getQuatCamera()->get_ortho_matrix(): ", gl->getQuatCamera()->get_ortho_matrix());
            m_ProjectionMatrixU.setUniform(program);


            if (m_canChangeDepthTestState)
            {
                m_isDepthTestEnabledChange = gl->setDepthTestState(m_targetDepthTestState);
            }

            if (m_canChangeBlendState)
            {
                m_blendStatechanged = gl->setBlendState(m_targetBlendState);
            }

            program->setVertexData(*m_spriteVbo);

            if (m_uvVbo)
            {
                program->setVertexData(*m_uvVbo);
            }

            program->setIndices(*m_indexVbo);
            program->draw();
        }
    }

    void
    HudElement::drawWithTextureAtlas(AexRenderer* gl)
    {
        if (m_hudElemOnTouch)
        {
            if (isTouched())
            {
                return m_hudElemOnTouch->drawWithTextureAtlas(gl);
            }
        }

        const ShaderDrw* shader = gl->getShader();

        if (shader)
        {
            const AexShader& program = shader->GetProgram();
            m_PosUniform->setValue(m_posX, m_posY);
            m_UvOffsetUniform->setValue(m_uvOffsetX, m_uvOffsetY);

            if (!m_isCut)
            {
                m_SizeUniform->setValue(m_sizeX, m_sizeY);
                m_UvSizeUniform->setValue(m_uvOffsizeX, m_uvOffsizeY);
            } else
            {
                m_SizeUniform->setValue(m_sizeX * m_cutX, m_sizeY * m_cutY);
                m_UvSizeUniform->setValue(m_uvOffsizeX * m_cutX, m_uvOffsizeY * m_cutY);
            }

            if (m_spriteVbo)
            {
                program->setVertexData(*m_spriteVbo);
            }

            if (m_indexVbo)
            {
                program->setIndices(*m_indexVbo);
            }

            m_PosUniform->setUniform(program);
            m_SizeUniform->setUniform(program);
            m_UvOffsetUniform->setUniform(program);
            m_UvSizeUniform->setUniform(program);
            m_colorModUniform->setUniform(program);
            m_alphaUniform->setUniform(program);
            
            program->draw();

            if (m_printDebug)
            {
                printDebug();
            }
        }
    }
    
    bool HudElement::isPointIsInElement(const aex::math::Vector2& point)
    {
        return isPointIsInElement(point, aex::math::Vector2(), aex::math::Vector2(1.0f,1.0f));
    }
    
    bool HudElement::isPointIsInElement(const aex::math::Vector2& point, const aex::math::Vector2& baseOffset, const aex::math::Vector2& baseSize)
    {
            float posX = getPosX() + baseOffset.x;
            float posY = getPosY() + baseOffset.y;

            float sizeX = getSizeX();
            float sizeY = getSizeY();

            HudHitbox_ptr hitbox = getHitbox();

            if (hitbox != nullptr)
            {

                if (hitbox->IsPositionRelativeToElement())
                {
                    posX += hitbox->GetPositionOffset().x* baseSize.x;
                    posY += hitbox->GetPositionOffset().y* baseSize.y;
                } else
                {
                    posX = hitbox->GetPositionOffset().x* baseSize.x;
                    posY = hitbox->GetPositionOffset().y* baseSize.y;
                }

                if (hitbox->IsSizeRelativeToElement())
                {
                    sizeX *= hitbox->GetSizeOffset().x;
                    sizeY *= hitbox->GetSizeOffset().y;
                } else
                {
                    sizeX = hitbox->GetSizeOffset().x* baseSize.x;
                    sizeY = hitbox->GetSizeOffset().y* baseSize.y;
                }

            }
            
            if (point.x > posX && point.x < (posX + sizeX) && point.y > posY && point.y < (posY + sizeY)
                    && isVisible() && isClickable())
            {
                return true;
            }
            
            return false;
    }

    void HudElement::setPrintDebug(bool pdebug)
    {
        m_printDebug = pdebug;
    }

    void HudElement::printDebug()
    {

        LOG_DEBUG("name :", m_name, ": ", this, ", ", m_textureAtlas);
        LOG_DEBUG("m_PosUniform: ", m_PosUniform->getValue());
        LOG_DEBUG("m_SizeUniform: ", m_SizeUniform->getValue());
        LOG_DEBUG("m_UvOffsetUniform: ", m_UvOffsetUniform->getValue());
        LOG_DEBUG("m_UvSizeUniform: ", m_UvSizeUniform->getValue());

    }

    void
    HudElement::finishdraw(AexRenderer* gl)
    {
        if (m_hudElemOnTouch)
        {
            if (isTouched())
            {
                return m_hudElemOnTouch->finishdraw(gl);
            }
        }

        if (m_blendStatechanged)
        {
            gl->setBlendState(!m_targetBlendState);
            m_blendStatechanged = false;
        }

        if (m_isDepthTestEnabledChange)
        {
            gl->setDepthTestState(!m_targetDepthTestState);
            m_isDepthTestEnabledChange = false;
        }

        if (!m_textureAtlas)
        {

            if (m_hudElementTex)
            {
                m_hudElementTex->unbind();
            }

            const auto& shader = gl->getShader();

            if (shader)
            {
                const auto& program = shader->GetProgram();

                if (m_spriteVbo)
                {
                    program->releaseVertexData(*m_spriteVbo);
                }

                if (m_uvVbo)
                {
                    program->releaseVertexData(*m_uvVbo);
                }
            }

            if (m_needOwnShader)
            {
                m_shader->finishdraw(gl);
            }

        } else
        {

        }
    }

    void HudElement::setName(const aex::string& name)
    {
        m_name = name;
    }

    aex::string HudElement::getName() const
    {
        return m_name;
    }

    void HudElement::setCanChangeBlendState(bool canChangeBlendState)
    {
        m_canChangeBlendState = canChangeBlendState;
    }

    bool HudElement::isCanChangeBlendState() const
    {
        return m_canChangeBlendState;
    }

    void HudElement::setClickable(bool Clickable)
    {
        m_isClickable = Clickable;
    }

    bool HudElement::isClickable() const
    {
        return m_isClickable;
    }

    void HudElement::setPos(float posX, float posY)
    {

        m_posX = posX;
        m_posY = posY;
    }

    void HudElement::addPos(float posX, float posY)
    {
        m_posX += posX;
        m_posY += posY;
    }

    void HudElement::scale(float sclX, float sclY)
    {
        m_posX *= sclX;
        m_posY *= sclY;

        m_sizeX *= sclX;
        m_sizeY *= sclY;
    }

    float HudElement::getAlpha() const
    {
        return m_alphaUniform->getValue();
    }
    
    void  HudElement::setAlpha(float alpha)
    {
        m_alphaUniform->setValue(alpha);
    }
    
    void HudElement::setColor(float r,float g,float b,float a)
    {
        m_colorModUniform->setValue(r, g, b);
        setAlpha(a);
    }
    
    void HudElement::setColor(aex::math::Vector3 rgb,float a)
    {
        setColor(rgb.x, rgb.y, rgb.z, a);
        setAlpha(a);
    }
    
    void HudElement::setColor(aex::math::Vector3 rgb)
    {
        m_colorModUniform->setValue(rgb);
    }
    
    void HudElement::setColor(float r,float g,float b)
    {
        m_colorModUniform->setValue(r, g, b);
    }
    
    aex::math::Vector3 HudElement::getColor() const
    {
        return m_colorModUniform->getValue();
    }
    
    void HudElement::cut(float cutX, float cutY)
    {
        m_cutX = cutX;
        m_cutY = cutY;

        if (aex::math::Math::closeEnough(cutX, 1.0f, aex::math::Math::EPSILON) && aex::math::Math::closeEnough(cutY, 1.0f, aex::math::Math::EPSILON))
        {
            m_isCut = false;
        } else
        {
            m_isCut = true;
        }
    }

    aex::math::Vector2 HudElement::GetPosition()
    {
        return aex::math::Vector2(m_posX, m_posY);
    }

    void HudElement::SetPosition(aex::math::Vector2 pos)
    {
        m_posX = pos.x;
        m_posY = pos.y;
    }

    void HudElement::SetPosition(float posX, float posY)
    {
        m_posX = posX;
        m_posY = posY;
    }

    void HudElement::setPosY(float posY)
    {
        m_posY = posY;
    }

    float HudElement::getPosY() const
    {
        return m_posY;
    }

    void HudElement::setPosX(float posX)
    {
        m_posX = posX;
    }

    float HudElement::getPosX() const
    {
        return m_posX;
    }

    void HudElement::setSize(aex::math::Vector2 size)
    {
        m_sizeX = size.x;
        m_sizeY = size.y;
    }

    void HudElement::setSize(float sizeX, float sizeY)
    {
        m_sizeX = sizeX;
        m_sizeY = sizeY;
    }

    void HudElement::addSize(float sizeX, float sizeY)
    {
        m_sizeX += sizeX;
        m_sizeY += sizeY;
    }

    void HudElement::setSizeY(float sizeY)
    {
        m_sizeY = sizeY;
    }

    float HudElement::getSizeY() const
    {
        return m_sizeY;
    }

    void HudElement::setSizeX(float sizeX)
    {
        m_sizeX = sizeX;
    }

    float HudElement::getSizeX() const
    {
        return m_sizeX;
    }

    void HudElement::setTextureAtlas(bool textureAtlas)
    {
        m_textureAtlas = textureAtlas;
    }

    bool HudElement::isTextureAtlas() const
    {
        return m_textureAtlas;
    }

    void HudElement::setTouched(bool Touched)
    {
        m_isTouched = Touched;
    }

    bool HudElement::isTouched() const
    {
        return m_isTouched;
    }

    void HudElement::setShader(const ShaderDrw& shader)
    {
        if (m_cleanup && m_shader)
        {
            m_cleanup(m_shader);
        }

        m_shader = new ShaderDrw(shader);
        m_cleanup = [](ShaderDrw * s) {
            delete s;
        };
        m_shaderLoaded = true;
    }

    ShaderDrw& HudElement::getShader()
    {
        if (!m_shader)
        {
            m_shader = ShaderDrw::BasicHudElementShaderOrthoOffCenter();
            m_cleanup = [](ShaderDrw * s) {
                UNUSED(s);
                ShaderDrw::FreeHudElementShaderOrthoOffCenter();
            };
            m_shaderLoaded = true;
        }

        return *m_shader;
    }

    aex::math::Vector2 HudElement::getUV()
    {
        return aex::math::Vector2(m_uvOffsizeX, m_uvOffsizeY);
    }

    aex::math::Vector2 HudElement::getUvOffset()
    {
        return aex::math::Vector2(m_uvOffsetX, m_uvOffsetY);
    }

    void
    HudElement::setQuatCamera(const aex::shared_ptr<aex::QuaternionCamera>& camera)
    {
        m_quatCamera = camera.get();
    }

    void HudElement::setNeedOwnShader(bool needOwnShader)
    {
        m_needOwnShader = needOwnShader;
    }

    bool HudElement::isNeedOwnShader() const
    {
        return m_needOwnShader;
    }

    void HudElement::setTargetBlendState(bool targetBlendState)
    {
        m_targetBlendState = targetBlendState;
    }

    bool HudElement::isTargetBlendState() const
    {
        return m_targetBlendState;
    }

    void HudElement::setTargetDepthTestState(bool targetDepthTestState)
    {
        m_targetDepthTestState = targetDepthTestState;
    }

    bool HudElement::isTargetDepthTestState() const
    {
        return m_targetDepthTestState;
    }

    void
    HudElement::setTexture(Texture* t)
    {
        if (t != m_hudElementTex)
        {
            if (m_hudElementTex)
            {
                TextureManager::GetInstance().releaseTexture(m_hudElementTex);
            }

            m_hudElementTex = t;
        }
    }

    void HudElement::logTexture()
    {
        LOG_DEBUG("HudElement", m_hudElementTex);
    }

    void HudElement::setTextureRaw(Texture* t)
    {
        m_hudElementTex = t;
    }

    void HudElement::setHudElemOnTouch(HudElement_ptr hudElem)
    {
        m_hudElemOnTouch = hudElem;
    }

    void HudElement::setHitbox(HudHitbox_ptr hitbox)
    {
        m_hitbox = hitbox;
    }

    HudHitbox_ptr HudElement::getHitbox() const
    {
        return m_hitbox;
    }

    void HudElement::onDown(float posX, float posY)
    {
        UNUSED(posX);
        UNUSED(posY);
        // Nothing here
    }

    void HudElement::onMotion(float posX, float posY)
    {
        UNUSED(posX);
        UNUSED(posY);
        // Nothing here
    }

    void HudElement::onUp(float posX, float posY)
    {
        UNUSED(posX);
        UNUSED(posY);
        // nothing here
    }

    void HudElement::onClick(float posX, float posY)
    {
        UNUSED(posX);
        UNUSED(posY);
        // Nothing here
    }

}
