//
//  Material.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 26.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#include <aex/Material.hpp>
#include <aex/TextureIncludes.hpp>
#include <aex/TextureManager.hpp>

namespace aex {
    /*
     *struct Material
     *{
     *   vec3 color;
     *   float shininess;
     *   bool useDiffuse;
     *   bool useSpecular;
     *   bool useLights;
     *   float specularWeight;
     *   bool useBump;
     *   loat bumpWeight;
     *   bool useDof;
     *   float mappingType;
     *   float dofWeight;
     *   bool useRefraction;
     *   float refraction;
     *};
     */

     namespace __detail_tmaterial
    {
        std::atomic<uint32_t> _nextId(1);
        uint32_t nextMaterialId()
        {
            return _nextId.fetch_add(1);
        }
    }
    
    TextureUniform::TextureUniform(Texture* tex , std::string name):
    uniformSampler(tex, name),
    texture(tex)
    {
        
    }
    
    Material::Material() :
    m_material_id(__detail_tmaterial::nextMaterialId()),
    mDiffuseSampler(nullptr, "diffuse"),
    mBumpSampler(nullptr, "bump"),
    mSpecularSampler(nullptr, "specular"),
    mCubeSampler(nullptr, "cube"),
    mMaterialColor(1.0, 1.0, 1.0, "color"),
    mMaterialTiling(1.0, 1.0, "tiling"),
    mMaterialUseDiffuse(false, "useDiffuse"),
    mMaterialUseSpecular(false, "useSpecular"),
    mMaterialUseBump(false, "useBump"),
    mMaterialUseRefraction(false, "useRefraction"),
    mMaterialUseCube(false, "useCube"),
    mMaterialSpecularWeight(1.0f, "specularWeight"),
    mMaterialReflectionWeight(1.0f, "reflectionWeight"),
    mMaterialMappingType(1.0f, "mappingType"),
    mMaterialRefraction(1.0f, "refraction"),
    mMaterialBumpWeight(0.5f, "bumpWeight"),
    mMaterialUseDof(false, "useDof"),
    mMaterialUseLights(true, "useLights"),
    mMaterialDofWeight(0.1f, "dofWeight"),
    mMaterialShininess(0.0f, "shininess"),
    mMaterialStruct("material") {
        mMaterialStruct.addContent(&mMaterialColor);
        mMaterialStruct.addContent(&mMaterialTiling);
        mMaterialStruct.addContent(&mMaterialUseDiffuse);
        mMaterialStruct.addContent(&mMaterialUseSpecular);
        mMaterialStruct.addContent(&mMaterialUseBump);
        mMaterialStruct.addContent(&mMaterialUseCube);
        mMaterialStruct.addContent(&mMaterialSpecularWeight);
        mMaterialStruct.addContent(&mMaterialReflectionWeight);
        
        mMaterialStruct.addContent(&mMaterialMappingType);
        mMaterialStruct.addContent(&mMaterialBumpWeight);
        mMaterialStruct.addContent(&mMaterialRefraction);
        mMaterialStruct.addContent(&mMaterialUseDof);
        mMaterialStruct.addContent(&mMaterialUseLights);
        mMaterialStruct.addContent(&mMaterialUseLights);
        mMaterialStruct.addContent(&mMaterialDofWeight);
        mMaterialStruct.addContent(&mMaterialShininess);
    }

    Material::Material(bool cube) :
    Material() {
    mMaterialUseCube.setValue(cube);
    }

    Material& Material::operator=(const Material& mat) {
        m_material_id = mat.m_material_id;
        m_isTransparent = mat.m_isTransparent;

        m_alpha = mat.m_alpha;
        m_diffuse_texture = mat.m_diffuse_texture;
        m_bump_texture = mat.m_bump_texture;
        m_specular_texture = mat.m_specular_texture;
        m_cube_texture = mat.m_cube_texture;
        mDiffuseSampler=mat.mDiffuseSampler;
        mBumpSampler=mat.mBumpSampler;
        mSpecularSampler=mat.mSpecularSampler;
        mCubeSampler=mat.mCubeSampler;
        mMaterialColor=mat.mMaterialColor;
        mMaterialTiling=mat.mMaterialTiling;
        mMaterialUseDiffuse=mat.mMaterialUseDiffuse;
        mMaterialUseSpecular=mat.mMaterialUseSpecular;
        mMaterialUseBump=mat.mMaterialUseBump;
        mMaterialUseCube=mat.mMaterialUseCube;
        mMaterialSpecularWeight=mat.mMaterialSpecularWeight;
        mMaterialReflectionWeight=mat.mMaterialReflectionWeight;
        mMaterialMappingType=mat.mMaterialMappingType;
        mMaterialRefraction=mat.mMaterialRefraction;
        mMaterialBumpWeight=mat.mMaterialBumpWeight;
        mMaterialUseDof=mat.mMaterialUseDof;
        mMaterialUseLights=mat.mMaterialUseLights;
        mMaterialDofWeight=mat.mMaterialDofWeight;
        mMaterialShininess=mat.mMaterialShininess;
        m_customTextures = mat.m_customTextures;

        return *this;
    }

    Material::~Material() {

        if (m_diffuse_texture) {
            //LOG_DEBUG("Material::~Material(): TextureManager::GetInstance().ReleaseTexture(m_diffuse_texture);", m_diffuse_texture->getName() );
            TextureManager::GetInstance().releaseTexture(m_diffuse_texture);
           
        }

        if (m_bump_texture) {
            //LOG_DEBUG("Material::~Material(): TextureManager::GetInstance().ReleaseTexture(m_bump_texture);", m_bump_texture->getName() );
            TextureManager::GetInstance().releaseTexture(m_bump_texture);
            
        }

        if (m_specular_texture) {
            //LOG_DEBUG("Material::~Material(): TextureManager::GetInstance().ReleaseTexture(m_specular_texture);",  m_specular_texture->getName());
            TextureManager::GetInstance().releaseTexture( m_specular_texture);
            
        }

        /*if( m_cube_texture)
        {
            TextureManager::GetInstance().ReleaseTexture(m_cube_texture);
        }*/
    }

    void Material::set_tiling(aex::math::Vector2 tiling) {
        mMaterialTiling.setValue(tiling);
    }

    void Material::set_tiling(float u, float v) {
        mMaterialTiling.setValue(u, v);
    }

    void Material::set_tiling(float uv) {
        set_tiling(uv, uv);
    }

    void
    Material::use_diffuse(bool diff) {
        if (m_diffuse_texture) {
            mMaterialUseDiffuse.setValue(diff);
        }
    }

    void
    Material::use_bump(bool bump) {
        if (m_bump_texture) {
            mMaterialUseBump.setValue(bump);
        }
    }

    void
    Material::set_bump_weight(float bump_weight) {
        mMaterialBumpWeight.setValue(bump_weight);
    }

    void
    Material::set_mapping_type(float mapping_type) {
        mMaterialMappingType.setValue(mapping_type);
    }

    void
    Material::use_specular(bool specular) {
        if (m_specular_texture) {
            mMaterialUseSpecular.setValue(specular);
        }
    }

    void
    Material::set_specular_weight(float specular_weight) {
        mMaterialSpecularWeight.setValue(specular_weight);
    }

    void
    Material::set_reflection_weight(float weight) {
        mMaterialReflectionWeight.setValue(weight);
    }

    void
    Material::use_cube(bool cube) {
        if (m_cube_texture) {
            mMaterialUseCube.setValue(cube);
        }
    }

    void
    Material::set_color(float r, float g, float b) {
        mMaterialColor.setValue(aex::math::Vector3(r, g, b));
    }

    void
    Material::set_colorRGB(float r, float g, float b) {
        mMaterialColor.setValue(aex::math::Vector3(r / 255.0f, g / 255.0f, b / 255.0f));
    }

    void
    Material::set_color(aex::math::Vector3 color) {
        mMaterialColor.setValue(color);
    }

    void
    Material::set_colorRGB(aex::math::Vector3 color) {
        mMaterialColor.setValue(color / 255.0f);
    }

    void
    Material::set_color(float color[3]) {
        mMaterialColor.setValue(color);
    }

    void
    Material::set_shininess(float shininess) {
        mMaterialShininess.setValue(shininess);
    }

    int32_t
    Material::get_material_id() {
        return m_material_id;
    }

    void Material::draw(AexRenderer* gl) {

        bindAll();
        uniforms(gl);

        const ShaderDrw* program = gl->getShader();
        for(auto& textureuniform : m_customTextures)
        {
            textureuniform.texture->bind();
            textureuniform.uniformSampler.setUniform(*program);
        }
        //_uniformsForDrawNode(gl);

        m_changedBlendState = gl->setBlendState(m_isTransparent);
        //LOG_DEBUG("m_changedBlendState: ",m_changedBlendState," ", m_isTransparent);

    }

    void Material::finishdraw(AexRenderer* gl) {

        unbindAll();
        for(auto& textureuniform : m_customTextures)
        {
            textureuniform.texture->unbind();
        }
        
        if (m_changedBlendState) {
            gl->setBlendState(!m_isTransparent);
            m_changedBlendState = false;
        }

    }

    void Material::bindAll() {
        if (mMaterialUseCube.isValue()) {
            m_cube_texture->bind();
        }

        if (mMaterialUseSpecular.isValue()) {
            m_specular_texture->bind();
        }

        if (mMaterialUseBump.isValue()) {
            m_bump_texture->bind();
        }

        if (mMaterialUseDiffuse.isValue()) {
            m_diffuse_texture->bind();
        }
    }

    void
    Material::unbindAll() {
        if (mMaterialUseCube.isValue()) {
            m_cube_texture->unbind();
        }

        if (mMaterialUseSpecular.isValue()) {
            m_specular_texture->unbind();
        }

        if (mMaterialUseBump.isValue()) {
            m_bump_texture->unbind();
        }

        if (mMaterialUseDiffuse.isValue()) {
            m_diffuse_texture->unbind();
        }
    }

    void
    Material::uniforms(const AexRenderer* gl) {
        const ShaderDrw* program = gl->getShader();

        if (program) {
            mMaterialStruct.setUniform(*program);

            if (mMaterialUseDiffuse.isValue()) {
                mDiffuseSampler.setUniform(*program);
            }

            if (mMaterialUseBump.isValue()) {
                mBumpSampler.setUniform(*program);
            }

            if (mMaterialUseSpecular.isValue()) {
                mSpecularSampler.setUniform(*program);
            }

            if (mMaterialUseCube.isValue()) {
                mCubeSampler.setUniform(*program);
            }
        }
    }

    void Material::uniformsFrame(const AexRenderer* gl) {
        const ShaderDrw* program = gl->getShader();

        if (program) {
            if (mMaterialUseDiffuse.isValue()) {
                mDiffuseSampler.setUniform(*program);
            }

            if (mMaterialUseBump.isValue()) {
                mBumpSampler.setUniform(*program);
            }

            if (mMaterialUseSpecular.isValue()) {
                mSpecularSampler.setUniform(*program);
            }

            if (mMaterialUseCube.isValue()) {
                mCubeSampler.setUniform(*program);
            }
        }
    }

    void Material::setAlpha(float alpha) {
        m_alpha = alpha;
    }

    float Material::getAlpha() const {
        return m_alpha;
    }

    void Material::setTransparent(bool Transparent) {
        m_isTransparent = Transparent;
    }

    bool Material::isTransparent() const {
        return m_isTransparent;
    }

    void Material::addCustomTexture(Texture& tex, const std::string& name)
    {
        addCustomTexture(&tex, name);
    }
    
    void Material::addCustomTexture(Texture* tex, const std::string& name)
    {
        m_customTextures.emplace_back(tex, name);
    }
    
    void Material::changeCustomTexture(Texture* tex, const std::string& name)
    {
        for (auto& textureuniform : m_customTextures)
        {
            if(textureuniform.uniformSampler.getName() == name)
            {
                textureuniform.texture = tex;
                break;
            }
        }
    }
    
     void Material::changeCustomTexture(Texture& tex, const std::string& name)
    {
        changeCustomTexture(&tex, name);
    }
    
    void
    Material::set_diffuse(Texture& tex) {
        set_diffuse(&tex);
    }

    void Material::set_diffuse(Texture* tex) {
        m_diffuse_texture = tex;
        mDiffuseSampler.setValue(tex);
        mMaterialUseDiffuse.setValue(true);
      
    }

    //	/**
    //     * Set bump texture
    //     * @method setBump
    //     * @param {Texture} tex Texture to set on bump channel
    //     */

    void
    Material::set_bump(Texture& tex) {
        set_bump(&tex);
    }
    
    void Material::set_bump(Texture* tex)
    {
        m_bump_texture = tex;
        mBumpSampler.setValue(tex);
        mMaterialUseBump.setValue(true);
    }
    //	/**
    //     * Set specular texture
    //     * @method setSpecular
    //     * @param {Texture} tex Texture to set on specular channel
    //     */

    void
    Material::set_specular(Texture& tex) {
        set_specular(&tex);
    }
    
    void
    Material::set_specular(Texture* tex) {
        m_specular_texture = tex;
        mSpecularSampler.setValue(tex);
        mMaterialUseSpecular.setValue(true);
    }
    //	/**
    //     * Set cube environment texture
    //     * @method setCube
    //     * @param {Texture} tex Texture to set on environment channel
    //     */

    void
    Material::set_cube(Texture& tex) {
        set_cube(&tex);
    }
    
    void
    Material::set_cube(Texture* tex) {
        m_cube_texture = tex;
        mCubeSampler.setValue(tex);
        mMaterialUseCube.setValue(true);
    }
}
