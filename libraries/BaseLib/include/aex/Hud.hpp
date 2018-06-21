/*
 * File:   Hud.hpp
 * Author: mritke
 *
 * Created on December 10, 2013, 1:39 PM
 */

#ifndef HUD_HPP
#define	HUD_HPP

#include <aex/Common.hpp>
#include <aex/AexDrawable.hpp>
#include <aex/ShaderDrw.hpp>
#include <boost/signals2.hpp>
#include <array>
#include <vector>
#include <string>
#include <exception>

namespace aex
{

class TextureConfig;
class TextDrw;
class VertexData;
class IndexData;
class HudElement;
class Texture;
typedef shared_ptr<TextDrw> TextDrw_ptr;
typedef shared_ptr<HudElement> HudElementPtr;
typedef shared_ptr<HudElement> HudElement_ptr;
typedef std::map<std::string, HudElementPtr> ElementsMap;
class Hud;
typedef shared_ptr<Hud> Hud_ptr;

class AEX_PUBLIC_API HudParseError : public std::exception
{
    public:
        explicit HudParseError(const std::string& msg);
        explicit HudParseError(const char* msg);
        const char* what() const noexcept;
    private:
        static const std::string he_tag;
        const std::string m_msg;
};

class AEX_PUBLIC_API Hud : public DrawObject
{
    typedef VertexData* VertexVBOType;
    typedef IndexData* IndexVBOType;

public:
    Hud(Texture* tex);
    Hud(const std::string& texturePath, const aex::TextureConfig& cfg);
    Hud();
    ~Hud();
    static Hud_ptr makeHud(Texture* tex);
    void setQuatCamera(const aex::shared_ptr<aex::QuaternionCamera>& camera);
    void setQuatCamera(aex::QuaternionCamera* camera);

    void readHudFromTpacker(const std::string& fName);
    void readHudFromJson(const std::string& fName, bool forceLandscape = false, bool forcePortrait = false, bool rightOrBottomSizeTo00 = true);
    ElementsMap readTmapFromJson(const std::string& fName);
    HudElementPtr checkPointIsInHud(float x, float y, bool buttonDown = true, bool changeTouchValue = true);
    HudElementPtr getElementByName(const std::string& name);
    std::vector<HudElementPtr>& getElements();
    virtual void onOffByName(const std::string& name, bool on);
    virtual void draw(AexRenderer* gl);
    virtual void finishdraw(AexRenderer* gl);

    void needsUpdate() {}
    void update() {}
    void setShader(const ShaderDrw& shader);

    const ShaderDrw& getShader() const;
    void setName(std::string name);
    std::string getName();
    Uniforms getUniforms() const;
    float getHudOffsetY() const;
    float getHudOffsetX() const;
    virtual void addHudOffset(aex::math::Vector2 offset);
    virtual void setHudOffset(aex::math::Vector2 offset);
    virtual void addHudOffsetXY(float hudOffsetX, float hudOffsetY);
    virtual void setHudOffsetXY(float hudOffsetX, float hudOffsetY);
    virtual void scale(float hudscaleX, float hudscaleY);
    virtual void move(float hudscaleX, float hudscaleY);
    void setCanChangeDepthTestState(bool canChangeDepthTestState);
    bool isCanChangeDepthTestState() const;
    void setCanChangeBlendState(bool canChangeBlendState);
    bool isCanChangeBlendState() const;
    void addNextLayer(Hud_ptr nextLayer);
    void removeLayer(Hud_ptr nextLayer);
    void removeAllLayers();
    void setTextureAtlas(Texture* textureAtlas);
    void addHudElement(const HudElementPtr& hudelement);
    void removeHudElement(const HudElementPtr& hudelement);
    void replaceHudElement(const HudElementPtr& oldElem, const HudElementPtr& newElem);
    Texture* getTextureAtlas() const;
    Hud_ptr getNextLayer() const;
    virtual aex::ShaderDrw* createShader();
    virtual void releaseShader(aex::ShaderDrw* s);
    
    protected:
    std::string m_name;
    std::vector<HudElementPtr> m_hudElements;
    std::vector<HudElementPtr>* m_hudElementsDrawPtr;

    ShaderDrw* m_shader;
    bool m_shaderLoaded;
    IndexVBOType m_indexVbo;
    VertexVBOType m_spriteVbo;
    float m_hudOffsetX;
    float m_hudOffsetY;
    bool m_centerPosition;
    bool m_isDepthTestEnabledChange;
    bool m_blendStatechanged;
    bool m_canChangeDepthTestState;
    bool m_canChangeBlendState;
    UniformMatrix4fv m_viewMatrixU;
    UniformMatrix4fv m_ProjectionMatrixU;
    UniformFloat2 m_hudOffsetU;
    UniformFloat2 m_hudTxtSizeU;
    Texture* m_textureAtlas;
    UniformSampler m_HudAtlasUniform;
    //std::vector<Texture*> m_textureAtlasColection;
    QuaternionCamera* m_quatCamera=nullptr;
    Uniforms m_uniforms;
    bool m_initialized = false;

    float m_scaleX = 1.0f;
    float m_scaleY = 1.0f;
    Hud_ptr m_NextLayer =nullptr;
    std::vector<Hud_ptr> m_HudLayers;
    std::function<void(ShaderDrw*)> m_cleanup;
    void onContextDirty();
    
};

class AEX_PUBLIC_API MultiHudDisplay : public Hud
{
public:
    MultiHudDisplay();
    virtual void draw(AexRenderer* gl);
    virtual void finishdraw(AexRenderer* gl);
    void needsUpdate() {}
    void update() {}
    void addLayer(Hud_ptr hud);
    void scale(float hudscaleX, float hudscaleY);
    void move(float hudscaleX, float hudscaleY);
    
protected:
    std::vector<Hud_ptr> m_layers;

};



}

#endif	/* HUD_HPP */

