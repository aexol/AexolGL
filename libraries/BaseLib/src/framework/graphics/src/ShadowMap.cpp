#include <aex/ShadowMap.hpp>
#include <aex/AexDrawable.hpp>
#include <aex/Math.hpp>
#include <aex/Matrix.hpp>
#include <aex/PostEffect.hpp>
#include <aex/RenderingTreeBlock.hpp>

namespace aex {
    ///////////////////////////////////////////LightFrustum

    LightFrustum::LightFrustum(float resX, float resY, bool isCube, Light_Type type) :
    m_fbo(resX, resY, false, isCube),
    m_type(type),
    m_xResolution(resX),
    m_yResolution(resY)
    {
        m_camera.ortho(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 5000.0f);
    }

    LightFrustum::~LightFrustum() {

    }

    void LightFrustum::update() {

    }

    void LightFrustum::setPosition(aex::math::Vector3& position) {
        m_camera.set_position(position);
    }

    void LightFrustum::move(aex::math::Vector3& vector) {
        m_camera.move(vector);
    }

    void LightFrustum::setRotation(aex::math::Vector3& rotation) {
        m_camera.setRotation(rotation);
    }

    void LightFrustum::rotate(aex::math::Vector3& vector) {
        m_camera.rotate(vector);
    }

    void LightFrustum::setNeedsUpdate(bool needsUpdate) {
        m_needsUpdate = needsUpdate;
    }

    bool LightFrustum::isNeedsUpdate() const {
        return m_needsUpdate;
    }

    void LightFrustum::setType(Light_Type type) {
        m_type = type;
    }

    aex::math::Matrix LightFrustum::getViewPersepcitveMatrix() {
        return m_camera.get_view_projection_matrix();
    }

    Light_Type LightFrustum::getType() const {
        return m_type;
    }

    ///////////////////////////////////////////

    ///////////////////////////////////////////ConeLightFrustum

    ConeLightFrustum::ConeLightFrustum() {

    }

    ConeLightFrustum::ConeLightFrustum(float resX, float resY, Vector3 possition, Vector3 direction) :
    LightFrustum(resX, resY, false, Light_Type::CONE_LIGHT)
    {
        computeFrustum();
        direction.normalizeThis();
        m_camera.lookAt(possition, possition + direction);
    }

    void ConeLightFrustum::computeFrustum() {
        m_camera.perspective(90.0f, m_xResolution, m_yResolution, 1.0f, 5000.0f);
        m_needsUpdate = true;
    }

    bool ConeLightFrustum::updateCamera(AexRenderer* gl) {

        gl->setQuatCamera(&m_camera);
        gl->setViewProjectionMatrix(m_camera.transforms());
        gl->setViewMatrix(m_camera.get_view_matrix());
        m_fbo.draw(gl);

        return true;
    }

    void ConeLightFrustum::draw(AexRenderer* gl, RenderingTreeNode* parent) {

        updateCamera(gl);
        parent->drawChildren(gl);

    }

    void ConeLightFrustum::finishdraw(AexRenderer* gl) {
        m_fbo.finishdraw(gl);
    }

    Texture* ConeLightFrustum::getTexture() {
        return m_fbo.getTextureDepth();
    }
    ///////////////////////////////////////////

    ///////////////////////////////////////////DirectionalLightFrustum

    DirectionalLightFrustum::DirectionalLightFrustum() {

    }

    DirectionalLightFrustum::DirectionalLightFrustum(float resX, float resY, float ortoSizeX, float ortoSizeY, Vector3 possition, Vector3 direction) :
    LightFrustum(resX, resY, false, Light_Type::DIRECTIONAL_LIGHT),
    m_position(possition),
    m_direction(direction),
    m_orhoSize(ortoSizeX, ortoSizeY) {

        computeFrustum();
        direction.normalizeThis();

        m_camera.lookAt(possition, possition + direction);
    }

    void DirectionalLightFrustum::computeFrustum() {

        m_camera.set_behavior(aex::CAMERA_MODES::FP_MODE);
        m_camera.perspectiveOrtho(m_orhoSize.x, m_orhoSize.y, 1.0f, 5000.0f);
    }

    bool DirectionalLightFrustum::updateCamera(AexRenderer* gl) {

        m_camera.lookAt(m_position, m_position + m_direction, Vector(0.0f, 1.0f, 0.0f));

        gl->setQuatCamera(&m_camera);
        gl->setViewProjectionMatrix(m_camera.transforms());
        gl->setViewMatrix(m_camera.get_view_matrix());
        gl->getShader()->updateVPMatix(gl);
        
        m_fbo.draw(gl);

        return true;
    }

    void DirectionalLightFrustum::draw(AexRenderer* gl, RenderingTreeNode* parent) {

        m_camera.lookAt(m_position, m_position + m_direction, Vector(0.0f, 1.0f, 0.0f));

        gl->setQuatCamera(&m_camera);
        gl->setViewProjectionMatrix(m_camera.transforms());
        gl->setViewMatrix(m_camera.get_view_matrix());

        m_fbo.draw(gl);
        //LOG_DEBUG("parent->drawChildren(gl); ", parent);

        if (parent) {
            parent->drawChildren(gl);
        }

        //LOG_DEBUG("ok")
    }

    void DirectionalLightFrustum::finishdraw(AexRenderer* gl) {
        m_fbo.finishdraw(gl);

    }

    Texture* DirectionalLightFrustum::getTexture() {
        return m_fbo.getTextureDepth();
    }
    
    ///////////////////////////////////////////


    ///////////////////////////////////////////PointLightFrustum

    const std::vector<Vector3> PointLightFrustum::m_directions =

        std::vector<aex::math::Vector3>({aex::math::Vector3(1.0f, 0.0f, 0.0f),
        aex::math::Vector3(-1.0f, 0.0f, 0.0f),

        aex::math::Vector3(0.0f, 1.0f, 0.0f),
        aex::math::Vector3(0.0f, -1.0f, 0.0f),

        aex::math::Vector3(0.0f, 0.0f, 1.0f),
        aex::math::Vector3(0.0f, 0.0f, -1.0f)});


    const std::vector<Vector3> PointLightFrustum::m_upVector =
            std::vector<aex::math::Vector3>({aex::math::Vector3(0.0f, 1.0f, 0.0f),
        aex::math::Vector3(0.0f, 1.0f, 0.0f),

        aex::math::Vector3(-1.0f, 0.0f, 0.0f),
        aex::math::Vector3(1.0f,  0.0f, 0.0f),

        aex::math::Vector3(0.0f, 1.0f, 0.0f),
        aex::math::Vector3(0.0f, 1.0f, 0.0f)});

    PointLightFrustum::PointLightFrustum() {
        computeFrustum();
    }
    
    #define single_fbo true

    PointLightFrustum::PointLightFrustum(float resX, float resY, Vector3 possition) :
    LightFrustum(resX, resY, single_fbo, Light_Type::POINT_LIGHT),
    m_position(possition)   
    {
        if(!single_fbo)
        {
        m_fbos.emplace_back(new FrameBufferOGL(resX, resY, false, false));
        m_fbos.emplace_back(new FrameBufferOGL(resX, resY, false, false));
        m_fbos.emplace_back(new FrameBufferOGL(resX, resY, false, false));
        m_fbos.emplace_back(new FrameBufferOGL(resX, resY, false, false));
        m_fbos.emplace_back(new FrameBufferOGL(resX, resY, false, false));
       
        m_fbosPick[0]=&m_fbo;
        m_fbosPick[1]=m_fbos[0].get();
        m_fbosPick[2]=m_fbos[1].get();
        m_fbosPick[3]=m_fbos[2].get();
        m_fbosPick[4]=m_fbos[3].get();
        m_fbosPick[5]=m_fbos[4].get();
        
        TextureConfig cfg = TextureConfig::makeDefaultTextureCubeConfig();
        cfg.disableMipMapping();
                
        cfg.setMinFilterNearest();
        cfg.setMagFilterNearest();
        cfg.set32bitDepth();
        m_texture = Texture(cfg);
        }

        m_oneBuffer = single_fbo;
        
        computeFrustum();
        
        /*
        0 	GL_TEXTURE_CUBE_MAP_POSITIVE_X
        1 	GL_TEXTURE_CUBE_MAP_NEGATIVE_X
        2 	GL_TEXTURE_CUBE_MAP_POSITIVE_Y
        3 	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
        4 	GL_TEXTURE_CUBE_MAP_POSITIVE_Z
        5 	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
         */



        /*
        TextureConfig cfgDepth = TextureConfig::makeDefaultTextureConfig();
        
        if(cfgDepth.hasGeneratedMipmaps())
        {
            cfgDepth.toggleGenerateMipmaps();
        }
        cfgDepth.setCubeTextureType();

        cfgDepth.setMinFilterNearest();
        cfgDepth.setMagFilterNearest();
        cfgDepth.set32bitDepth();
        
        m_texture=Texture(cfgDepth);
         */

    }

    void PointLightFrustum::computeFrustum() {
        m_camera.perspective(90.0f, m_xResolution, m_yResolution, 1.0f, 500.0f);
    }

    bool PointLightFrustum::updateCamera(AexRenderer* gl) {
        
        if(!m_oneBuffer && m_currentPlane > 0)
        {
            m_fbosPick[m_currentPlane-1]->finishdraw(gl);
        }
        
        m_camera.lookAt(m_position, m_position + m_directions[m_currentPlane], m_upVector[m_currentPlane]);
        //m_camera.rotate(aex::math::Vector3(1.0,0.0,0.0));
       
        LOG_DEBUG(m_position, m_position + m_directions[m_currentPlane]);

        gl->setQuatCamera(&m_camera);
        gl->setViewProjectionMatrix(m_camera.transforms());
        gl->setViewMatrix(m_camera.get_view_matrix());
        gl->getShader()->updateVPMatix(gl);
        
        if(m_oneBuffer)
        {
            m_fbo.setCubeSide(m_currentPlane);
            m_fbo.draw(gl);
        }else
        {
            m_fbosPick[m_currentPlane]->draw(gl);
        }
        
        ++m_currentPlane;
        if (m_currentPlane < 6) {
            return false;
        } else {
            return true;
        }

    }

    void PointLightFrustum::draw(AexRenderer* gl, RenderingTreeNode* parent) {
        //LOG_DEBUG("PointLightFrustum::draw");
        for (int xyz = 0; xyz < 6; ++xyz) {
            m_camera.lookAt(m_position, m_position + m_directions[xyz]);

            gl->setQuatCamera(&m_camera);
            gl->setViewProjectionMatrix(m_camera.transforms());
            gl->setViewMatrix(m_camera.get_view_matrix());
            m_fbo.setCubeSide(xyz);
            m_fbo.draw(gl);
            //LOG_DEBUG("parent->drawChildren(gl); ", parent);

            if (parent) {
                parent->drawChildren(gl);
            }

            LOG_DEBUG("ok");

        }

    }

    void PointLightFrustum::finishdraw(AexRenderer* gl) {
        
        if(!m_oneBuffer)
        {
            m_fbosPick[5]->finishdraw(gl);
        }
        else
        {
            m_fbo.finishdraw(gl);
        }
        m_currentPlane=0;
                
    }

    Texture* PointLightFrustum::getTexture() {
        return m_fbo.getTexture();
    }

    ///////////////////////////////////////////


    ///////////////////////////////////////////ShadowMap

    const aex::math::Matrix ShadowMap::m_biasMatrix = aex::math::Matrix(std::array<float, 16>{{0.5, 0.0, 0.0, 0.5,
        0.0, 0.5, 0.0, 0.5,
        0.0, 0.0, 0.5, 0.5,
        0.0, 0.0, 0.0, 1.0}});

    const aex::string ShadowMap::m_fragmentShader =
            "uniform sampler2D diffuse;"
            "uniform vec2 vTex;"
            "void main(void)\n"
            "{\n"
            "vec4 color= texture2D(diffuse, vTex);"
            "if(color.a < 0.02) {discard;}"
            "gl_FragColor =vec4(color.rgb,1.0);\n"
            "}\n";

    ShadowMap::ShadowMap() :
    m_depthSamplerUniform("shadowMap"),
    m_maskUniform("shadowMapMask"),
    m_lightMatrix("aex_lightMatrix") {
        aex::TextureConfig cfg = aex::TextureConfig::makeDefaultTextureConfig();
        m_blackTexture.reset(new Texture(cfg));
        /*int alignmentOffset = (4 - 16 % 4) % 4;
        uint8_t* bitmap = new uint8_t[(16 + alignmentOffset)*16];
        m_blackTexture->setData(bitmap, 16, 16);
        delete [] bitmap;*/
        LOG_DEBUG("ShadowMap::ShadowMap()");
        m_blurrFilter = TextureFilter::simpleBlurrFilter(nullptr);
    }

    ShadowMap::ShadowMap(int x, int y, Light_Type lightType) :
    ShadowMap() {
        LOG_DEBUG(" ShadowMap::ShadowMap(int x, int y, Light_Type lightType)");
        if (Light_Type::DIRECTIONAL_LIGHT == lightType) {
            //  m_lightFrustrum = new DirectionalLightFrustum(float(x), float(y), Vector3(0.0f, 0.0f, 0.0f), Vector3(-1.0f, -1.0f, 0.0f));
        } else if (Light_Type::CONE_LIGHT == lightType) {
            m_lightFrustrum = new ConeLightFrustum(float(x), float(y), Vector3(20.0f, 20.0f, 20.0f), Vector3(0.0f, -1.0f, 0.0f));
        } else if (Light_Type::POINT_LIGHT == lightType) {
            m_lightFrustrum = new PointLightFrustum(float(x), float(y), Vector3::Zero()+(Vector3::Up()*40.0f));
        }

    }

    ShadowMap::ShadowMap(int x, int y, float ortoSizeX, float ortoSizeY, aex::math::Vector3 position, aex::math::Vector3 direction) :
    ShadowMap() {

        LOG_DEBUG("ShadowMap::ShadowMap(int x, int y, float ortoSizeX, float ortoSizeY, aex::math::Vector3 position, aex::math::Vector3 direction)");
        m_lightFrustrum = new DirectionalLightFrustum(float(x), float(y), ortoSizeX, ortoSizeY, position, direction);
        m_blurrFilter = TextureFilter::simpleBlurrFilter(nullptr, x, y);
    }

    ShadowMap::ShadowMap(int x, int y, int type) :
    ShadowMap(x, y, static_cast<Light_Type> (type)) {

    }

    ShadowMap&
    ShadowMap::operator=(const ShadowMap& shadowmap) {
        m_firstPass = shadowmap.m_firstPass;
        m_selfDraw = shadowmap.m_selfDraw;
        m_initialized = shadowmap.m_initialized;
        m_mapInProgress = shadowmap.m_mapInProgress;
        m_isStatic = shadowmap.m_isStatic;
        m_updateStatic = shadowmap.m_updateStatic;
        m_textureDepth = shadowmap.m_textureDepth;
        m_maskUniform = shadowmap.m_maskUniform;
        //
        m_lightFrustrum = shadowmap.m_lightFrustrum;
        m_depthSamplerUniform = shadowmap.m_depthSamplerUniform;
        m_lightMatrix = shadowmap.m_lightMatrix;
        m_blurrFilter = shadowmap.m_blurrFilter;
        m_customFragmentShader = shadowmap.m_customFragmentShader;
        
        //m_blackTexture = std::move(shadowmap.m_blackTexture);
        //m_shadowMapShader = std::move(shadowmap.m_shadowMapShader);
        
        return *this;

    }

    ShadowMap::~ShadowMap() {

        if (m_blackTexture) {
            //delete m_blackTexture;
        }
        if (m_blackTexture) {
            //delete m_blackTexture;
        }
    }

    /*ShadowMap::operator=(ShadowMap& map)
    {
        
    }*/
    
    ShaderDrw* ShadowMap::makeShadowMapShader(const ShaderDrw* shader)
    {
        
        ShaderDrw* ret;
        if(m_customFragmentShader.empty())
        {
           ret = new ShaderDrw(shader->getVertex_lines(), m_fragmentShader, shader->getVertex_attr());
        }
        else
        {
           ret = new ShaderDrw(shader->getVertex_lines(), m_customFragmentShader, shader->getVertex_attr());
        }
        
        //ret->addUniform(&m_depthSamplerUniform);
        //ret->addUniform(&m_lightMatrix);
        return ret;
    }

    void ShadowMap::onContextDirty() {

    }

    void ShadowMap::setUseBlur(bool isblur)
    {
        m_useBlur = isblur;
    }
    
    void ShadowMap::setSelfDraw(bool selfDraw) {
        m_selfDraw = selfDraw;
    }

    bool ShadowMap::isSelfDraw() const {
        return m_selfDraw;
    }

    void ShadowMap::setLightFrustrum(LightFrustum* lightFrustrum) {
        m_lightFrustrum = lightFrustrum;
    }

    LightFrustum* ShadowMap::getLightFrustrum() const {
        return m_lightFrustrum;
    }

    void ShadowMap::init() {
        //m_textureDepth = m_fbo.getTextureDepth();]

        m_initialized = true;
    }

    void ShadowMap::setSamplerName(const std::string& mapName) {
        m_depthSamplerUniform.setName(mapName);
    }

    void ShadowMap::setDiffuseInMaterial(Material& material) {
        material.set_diffuse(m_lightFrustrum->getTexture());
    }

    UniformSampler* ShadowMap::getSampler() {

        return &m_depthSamplerUniform;
    }

    void ShadowMap::setUpdateStatic(bool updateStatic) {
        m_updateStatic = updateStatic;
    }
    

    void ShadowMap::updateStatic() {
        m_updateStatic = true;
    }
    
    void ShadowMap::setCullFrontFace(bool cullface)
    {
        m_cullFrontFace = cullface;
    }

    bool ShadowMap::isUpdateStatic() const {
        return m_updateStatic;
    }

    void ShadowMap::setStatic(bool Static) {
        m_isStatic = Static;
    }

    bool ShadowMap::isStatic() const {
        return m_isStatic;
    }

    void ShadowMap::setCustomFragmentShader(const aex::string& customFragmentShader) {
        m_customFragmentShader = customFragmentShader;
    }

    aex::string ShadowMap::getCustomFragmentShader() const {
        return m_customFragmentShader;
    }

    void ShadowMap::setPassID(int passID)
    {
        m_passID = passID;
    }

    int ShadowMap::getPassID() const
    {
        return m_passID;
    }

    std::string ShadowMap::getPassName() const
    {
        return m_passname;
    }

    void ShadowMap::setFirstPass(bool firstPass)
    {
        m_firstPass = firstPass;
    }

    bool ShadowMap::isFirstPass() const
    {
        return m_firstPass;
    }

    void ShadowMap::setShadowMask(Texture* shadowMask)
    {
        m_shadowMask = shadowMask;
    }

    Texture* ShadowMap::getShadowMask() const
    {
        return m_shadowMask;
    }

    /*bool ShadowMap::updateCamera(AexRenderer* gl) {

        if (m_firstPass  || gl->isColorPick()) {
            return true;
        }
        return m_lightFrustrum->updateCamera(gl);
    }*/

    void ShadowMap::draw(AexRenderer* gl) {

        if (m_firstPass) {
            return;
        }

        if (m_initialized) {
            init();
        }
        m_mapInProgress = true;
        m_oldQuatCamera = gl->getQuatCamera();

        const ShaderDrw* shader = gl->getShader();

        if (m_selfDraw) {
            if (false) {
                m_blackTexture->bind();
            }
            m_lightFrustrum->getTexture()->bind();
            if (shader) {
                const auto& program = shader->GetProgram();


                m_depthSamplerUniform.setValue(m_lightFrustrum->getTexture());
                m_depthSamplerUniform.setUniform(program);
            }
        }

        //colorMask(false, false, false, false);

        //LOG_DEBUG("m_trackedNodes.size(): ", m_trackedNodes.size());
        m_lightFrustrum->draw(gl, m_trackedNodes.front());
        m_lightFrustrum->finishdraw(gl);

        //colorMask(true, true, true, true);

        if (gl->getQuatCamera() != m_oldQuatCamera) {
            gl->setQuatCamera(m_oldQuatCamera);
            gl->setViewProjectionMatrix(m_oldQuatCamera->transforms());
            gl->setViewMatrix(m_oldQuatCamera->get_view_matrix());
            shader->updateVPMatix(gl);
        }

        m_mapInProgress = false;

        if (m_textureDepth != m_lightFrustrum->getTexture()) {
            m_textureDepth = m_lightFrustrum->getTexture();
        }

        if (m_selfDraw) {

            if (m_blackTexture) {
                m_blackTexture->unbind();
            }

            m_textureDepth->bind();

            if (shader) {
                const auto& program = shader->GetProgram();

                m_depthSamplerUniform.setValue(m_textureDepth);
                m_depthSamplerUniform.setUniform(program);
            }
        }
    }

    void ShadowMap::finishdraw(AexRenderer* gl) {
        if (m_firstPass) {
            m_firstPass = false;
            return;
        }

        if (m_selfDraw && !gl->isColorPick()) {

            m_textureDepth->unbind();
            
            if (m_shadowMask)
            {
                m_shadowMask->unbind();
            }
        }
    }
    
    TextureFilter* ShadowMap::getBlurrFilter() const
    {
        return m_blurrFilter.get();
    }

    Texture* ShadowMap::getTexture() {
        if (m_mapInProgress) {
            return m_blackTexture.get();
        } else {

            if (m_textureDepth == nullptr) {
                m_textureDepth = m_lightFrustrum->getTexture();
                
                if (m_useBlur && m_blurrFilter) {
                    LOG_DEBUG("Texture* ShadowMap::getTexture(), m_blurrFilter->getOutputTexture(); ", m_blurrFilter->getOutputTexture());
                    m_textureDepth = m_blurrFilter->getOutputTexture();
                }
            }
            return m_textureDepth;
        }

    }

}
