/** @file Material.hpp
 *  @brief Definition of Material.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __aexolGL__Material__
#define __aexolGL__Material__

#include <aex/Uniforms.hpp>
#include <aex/AexDrawable.hpp>
#include <string>
#include <memory>
#include <array>

namespace aex
{
class AexRenderer;
class Texture;

    namespace __detail_tmaterial {
        uint32_t nextMaterialId();
    }
    
typedef aex::AABB AABB;

struct TextureUniform
{
    TextureUniform(Texture* texture , std::string name);
    UniformSampler uniformSampler;
    Texture* texture = nullptr; 
};

class AEX_PUBLIC_API Material : public aex::DrawObject, public aex::DrawVisibility
{
public:
    typedef std::vector<Uniform*> Uniforms;

    Material();
    /**
     *	@brief	Ctor
     *	@param 	cube 	Use shader with cube mapping.
     */
    Material(bool cube );

    virtual ~Material();
    
    void addCustomTexture(Texture& tex, const std::string& name);
    void addCustomTexture(Texture* tex, const std::string& name);
    
    void changeCustomTexture(Texture& tex, const std::string& name);
    void changeCustomTexture(Texture* tex, const std::string& name);
    
    /**
     *	@brief	Set diffuse texture data.
     *	@param 	tex 	Reference to Texture.
     */
    void set_diffuse(Texture& tex);
    void set_diffuse(Texture* tex);
    /**
     *	@brief	Use diffuse texture.
     *	@param 	diff 	True if yes, flase if no.
     */
    void use_diffuse(bool diff);
    /**
     *	@brief	Set bump texture data.
     *	@param 	tex 	Reference to bump texture.
     */
    void set_bump(Texture& tex);
    void set_bump(Texture* tex);
    /**
     *	@brief	Use bump texture.
     *	@param 	bump 	True if yes, false if no.
     */
    void use_bump(bool bump);
    /**
     *	@brief	Set bump texture weight.
     *	@param 	bump_weight 	Bump weight.
     */
    void set_bump_weight(float bump_weight);
    /**
     *	@brief	Set mapping type.
     *	@param 	mapping_type 	Mapping type.
     */
    void set_mapping_type(float mapping_type);
    /**
     *	@brief	Set specular texture data.
     *	@param 	tex 	Reference to specular texture.
     */
    void set_specular(Texture& tex);
    void set_specular(Texture* tex);
    /**
     *	@brief	Use specular texture.
     *	@param 	specular 	True if yes, false if no.
     */
    void use_specular(bool specular);
    /**
     *	@brief	Set specular weight.
     *	@param 	specular_weight 	Specular weight.
     */
    void set_specular_weight(float specular_weight);
    /**
     *	@brief	Set reflection weight.
     *	@param 	weight 	Reflection weight.
     */
    void set_reflection_weight(float weight);
    /**
     *	@brief	Set cube texture data.
     *	@param 	tex 	Reference to cube texture.
     */
    void set_cube(Texture& tex);
    void set_cube(Texture* tex);
    /**
     *	@brief	Use cube texture.
     *	@param 	cube 	True if yes, false if no.
     */
    void use_cube(bool cube);
    /**
     *	@brief	Default material color.
     *	@param 	color 	RGB color.
     */
    void set_color(float r, float g, float b);
    void set_colorRGB(float r, float g, float b);
    void set_color(aex::math::Vector3 color);
    void set_colorRGB(aex::math::Vector3 color);
    void set_color(float color[3]);
    
    
    void set_tiling(aex::math::Vector2 tiling);
    void set_tiling(float u, float v);
    void set_tiling(float uv);
    
    /**
     *	@brief	Set shininess.
     *	@param 	shininess 	Shininess.
     */
    void set_shininess(float shininess);
    /**
     *	@brief	Bind all textures to TUs.
     */
    void bindAll();
    /**
     *	@brief	Unbind all textures.
     */
    void unbindAll();
    /**
     *	@brief	Specify current program uniforms.
     */
    void uniforms(const AexRenderer*);
    /**
     *	@brief	Set uniforms first time in frame.
     */
    void uniformsFrame(const AexRenderer* gl);
    /**
     *	@brief	Id of shader.
     *	@return	Id
     */
    int32_t get_material_id();
    void draw(AexRenderer* gl);
    void finishdraw(AexRenderer* gl);
    void update() {};
    void needsUpdate()
    {
        m_needsUpdate = true;
    }
    bool isVisible()
    {
        return true;
    }
    AABB getBounds()
    {
        return AABB {};
    }
    void setAlpha(float alpha);
    float getAlpha() const;
    void setTransparent(bool Transparent);
    bool isTransparent() const;
    
    Material& operator=(const Material& mat);
    //        AABB getBounds(AABBBuilder children) {}
    //        AABB getChildBounds(){}
    
    protected:
    int32_t m_material_id;
    bool m_isTransparent        = false;
    bool m_changedBlendState    = false;
    float m_alpha;
    Texture* m_diffuse_texture  =nullptr;
    Texture* m_bump_texture     =nullptr;
    Texture* m_specular_texture =nullptr;
    Texture* m_cube_texture     =nullptr;
    
    UniformSampler mDiffuseSampler;
    UniformSampler mBumpSampler;
    UniformSampler mSpecularSampler;
    UniformSampler mCubeSampler;
    UniformFloat3  mMaterialColor;
    UniformFloat2  mMaterialTiling;
    UniformBool mMaterialUseDiffuse;
    UniformBool mMaterialUseSpecular;
    UniformBool mMaterialUseBump;
    UniformBool mMaterialUseRefraction;
    UniformBool mMaterialUseCube;
    UniformFloat mMaterialSpecularWeight;
    UniformFloat mMaterialReflectionWeight;
    UniformFloat mMaterialMappingType;
    UniformFloat mMaterialRefraction;
    UniformFloat mMaterialBumpWeight;
    UniformBool mMaterialUseDof;
    UniformBool mMaterialUseLights;
    UniformFloat mMaterialDofWeight;
    UniformFloat mMaterialShininess;
    UniformStruct mMaterialStruct;
    
    std::vector<TextureUniform> m_customTextures;
    
};

typedef shared_ptr<Material> MaterialShrd_Ptr;
typedef shared_ptr<Material> Material_ptr;
}

#endif /* defined(__aexolGL__Material__) */
