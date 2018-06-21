//
//  Texture.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 25.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#include "../include/TextureInternal.hpp"
#include <aex/api_definitions.hpp>
#include <aex/Texture.hpp>
#include <aex/ax_Macros.hpp>
#include <aex/Logger.hpp>
#include <aex/GPUApi.hpp>
#include <aex/AexRenderer.hpp>
#include <aex/UniformSampler.hpp>
#include <aex/FileManager.hpp>
#include <SDL.h>
#include <SDL_image.h>
#include <boost/filesystem/path.hpp>
#include "../src/framework/platform_specific/include/CameraImpl.hpp"



namespace aex {
    //Texture( std::vector<std::pair<aex::string, AexEnum>> name, uint32_t = GL_TEXTURE_2D, uint32_t = GL_RGBA, uint32_t = GL_UNSIGNED_BYTE, uint32_t = GL_LINEAR_MIPMAP_LINEAR, uint32_t = GL_LINEAR, uint32_t = GL_REPEAT, uint32_t = GL_REPEAT );

    TextureConfig::TextureConfig(uint32_t textureType,
            uint32_t format,
            uint32_t dataType,
            uint32_t minFilter,
            uint32_t magFilter,
            uint32_t wrapS,
            uint32_t wrapT,
            bool generateMipMaps) :
    m_textureType(textureType),
    m_format(format),
    m_dataType(dataType),
    m_minFilter(minFilter),
    m_magFilter(magFilter),
    m_wrapS(wrapS),
    m_wrapT(wrapT),
    m_generateMipmaps(generateMipMaps) {
    }

    TextureConfig::TextureConfig() :
    TextureConfig(TEX_TYPE_2D, TEX_FORMAT_RGBA, TEX_DATATYPE_8BIT_PER_CHANNEL, TEX_FILTER_LINEAR_MIPMAP_LINEAR, TEX_FILTER_LINEAR, TEX_WRAP_REPEAT, TEX_WRAP_REPEAT, true) {
    }

    TextureConfig TextureConfig::makeDefaultTextureConfig() {
        return TextureConfig{TEX_TYPE_2D, TEX_FORMAT_RGBA, TEX_DATATYPE_8BIT_PER_CHANNEL, TEX_FILTER_LINEAR_MIPMAP_LINEAR, TEX_FILTER_LINEAR, TEX_WRAP_REPEAT, TEX_WRAP_REPEAT, true};
    }

    TextureConfig TextureConfig::makeDefaultTextureCubeConfig() {
        return TextureConfig{TEX_TYPE_CUBEMAP, TEX_FORMAT_RGB, TEX_DATATYPE_8BIT_PER_CHANNEL, TEX_FILTER_LINEAR_MIPMAP_LINEAR, TEX_FILTER_LINEAR, TEX_WRAP_REPEAT, TEX_WRAP_REPEAT, true};
    }

    TextureConfig TextureConfig::makeDefaultTextureCameraConfig() {
        return TextureConfig{TEX_TYPE_2D, TEX_FORMAT_RGBA, TEX_DATATYPE_8BIT_PER_CHANNEL, TEX_FILTER_LINEAR, TEX_FILTER_LINEAR, TEX_WRAP_CLAMP_TO_EDGE, TEX_WRAP_CLAMP_TO_EDGE, false};
    }

    void TextureConfig::setRGBFormat() {
        m_format = TEX_FORMAT_RGB;
    }

    void TextureConfig::setRGBAFormat() {
        m_format = TEX_FORMAT_RGBA;
    }

    void TextureConfig::setDepthComponentFormat()
    {
        m_format = TEX_FORMAT_DEPTH_COMPONENT;
    }

    void TextureConfig::setAlphaChannelFormat() {
        m_format = TEX_FORMAT_ALPHA;
    }

    void TextureConfig::setCubeTextureType() {
        m_textureType = TEX_TYPE_CUBEMAP;
    }

    void TextureConfig::set2DTextureType() {
        m_textureType = TEX_TYPE_2D;
    }

    void TextureConfig::set3DTextureType() {
        m_textureType = TEX_TYPE_3D;
    }

    void TextureConfig::set8bitPerChannel() {
        m_dataType = TEX_DATATYPE_8BIT_PER_CHANNEL;
    }

    void TextureConfig::set16bit565() {
        m_dataType = TEX_DATATYPE_565;
    }

    void TextureConfig::set16bit4444() {
        m_format = m_dataType = TEX_DATATYPE_4444;
    }

    void TextureConfig::set16bit5551() {
        m_dataType = TEX_DATATYPE_5551;
    }

    void TextureConfig::set16bitDepth() {
        m_format = TEX_FORMAT_DEPTH_COMPONENT;
        m_dataType = TEX_DATATYPE_16BIT_DEPTH;
    }

    void TextureConfig::set24bitDepth() {
        m_format = TEX_FORMAT_DEPTH_COMPONENT;
        m_dataType = TEX_DATATYPE_24BIT_DEPTH;
    }

    void TextureConfig::set32bitDepth() {
        m_format = TEX_FORMAT_DEPTH_COMPONENT;
        m_dataType = TEX_DATATYPE_32BIT_DEPTH;
    }

    void TextureConfig::setMagFilterNearest() {
        m_magFilter = TEX_FILTER_NEAREST;
    }

    void TextureConfig::setMagFilterLinear() {
        m_magFilter = TEX_FILTER_LINEAR;
    }

    void TextureConfig::setMinFilterNearest() {
        m_minFilter = TEX_FILTER_NEAREST;
    }

    void TextureConfig::setMinFilterLinear() {
        m_minFilter = TEX_FILTER_LINEAR;
    }

    void TextureConfig::setMinFilterNearestMipmapNearest() {
        m_minFilter = TEX_FILTER_NEAREST_MIPMAP_NEAREST;
    }

    void TextureConfig::setMinFilterLinearMipmapNearest() {
        m_minFilter = TEX_FILTER_LINEAR_MIPMAP_NEAREST;
    }

    void TextureConfig::setMinFilterNearestMipmapLinear() {
        m_minFilter = TEX_FILTER_NEAREST_MIPMAP_LINEAR;
    }

    void TextureConfig::setMinFilterLinearMipmapLinear() {
        m_minFilter = TEX_FILTER_LINEAR_MIPMAP_LINEAR;
    }

    void TextureConfig::setWrapSClamToEdge() {
        m_wrapS = TEX_WRAP_CLAMP_TO_EDGE;
    }

    void TextureConfig::setWrapTClamToEdge() {
        m_wrapT = TEX_WRAP_CLAMP_TO_EDGE;
    }

    void TextureConfig::setWrapSRepeat() {
        m_wrapS = TEX_WRAP_REPEAT;
    }

    void TextureConfig::setWrapTRepeat() {
        m_wrapT = TEX_WRAP_REPEAT;
    }

    void TextureConfig::setWrapSMirroredRepeat() {
        m_wrapS = TEX_WRAP_MIRRORED_REPEAT;
    }

    void TextureConfig::setWrapTMirroredRepeat() {
        m_wrapT = TEX_WRAP_MIRRORED_REPEAT;
    }

    void TextureConfig::enableMipMapping() {
        m_generateMipmaps = true;
    }

    void TextureConfig::disableMipMapping() {
        m_generateMipmaps = false;
    }

    void TextureConfig::toggleGenerateMipmaps() {
        m_generateMipmaps = !m_generateMipmaps;
    }

    bool TextureConfig::operator==(const TextureConfig& cfg) const {
        return
        m_textureType == cfg.m_textureType &&
                m_format == cfg.m_format &&
                m_dataType == cfg.m_dataType &&
                m_minFilter == cfg.m_minFilter &&
                m_magFilter == cfg.m_magFilter &&
                m_wrapS == cfg.m_wrapS &&
                m_wrapT == cfg.m_wrapT &&
                m_generateMipmaps == cfg.m_generateMipmaps;
    }

    struct bindCounter {
        static std::atomic<uint32_t> nTextures;
    };
    std::atomic<uint32_t> bindCounter::nTextures{0};

    Texture::Texture() :
    m_sizeW(0),
    m_sizeH(0),
    m_cfg(TextureConfig::makeDefaultTextureConfig()),
    m_tex(nullptr, delete_texture_impl{}),
    m_created{
        false
    }

    ,
    m_filename({std::make_pair("", CubeSide::POS_X)}) {
    }

    Texture::Texture(const TextureConfig& cfg) :
    m_sizeW(0),
    m_sizeH(0),
    m_cfg(cfg),
    m_tex(nullptr, delete_texture_impl{}),
    m_created{
        false
    }
    {
    }

    void
    Texture::delete_texture_impl::operator()(void* p) {
        delete static_cast<HWTexture*> (p);
    }

    Texture::Texture(const aex::string& name) :
    Texture(name, TextureConfig::makeDefaultTextureConfig()) {
    }

    Texture::Texture(const aex::string& name, const TextureConfig& cfg) :
    m_sizeW(0),
    m_sizeH(0),
    m_cfg(cfg),
    m_tex(nullptr, delete_texture_impl{}),
    m_created{
        false
    }
    {
        m_filename.push_back(FileKey{name, CubeSide::POS_X});
    }

    Texture::Texture(const std::array<FileKey, 6>& name) :
    Texture(name, TextureConfig::makeDefaultTextureConfig()) {
    }

    Texture::Texture(const std::array<FileKey, 6>& name, const TextureConfig& cfg) :
    m_cfg(cfg),
    m_tex(nullptr, delete_texture_impl{}),
    m_created{
        false
    }
    {
        m_filename.resize(6);
        std::copy(name.begin(), name.end(), m_filename.begin());
    }

    Texture::Texture(void* camImplPtr) :
    m_sizeW(0),
    m_sizeH(0),
    m_cfg(TextureConfig::makeDefaultTextureCameraConfig()),
    m_tex(nullptr, delete_texture_impl{}),
    m_created{
        true
    }
    {
        cam::CameraImpl* camImpl = (cam::CameraImpl*) camImplPtr;
        HWTexture* hwTex = new HWTexture(m_cfg);
        camImpl->setHWTexture(*hwTex);
        m_tex.reset(hwTex);
    }

    Texture::Texture(Texture&& tex) {
        *this = std::move(tex);
    }

    Texture&
            Texture::operator=(Texture&& tex) {
        if (this != &tex) {
            IOnContextDirty::operator=(std::move(tex));
            m_cfg = std::move(tex.m_cfg);
            m_tex = std::move(tex.m_tex);
            m_created = tex.m_created;
            m_filename = std::move(tex.m_filename);
            m_name = std::move(tex.m_name);
        }
        return *this;
    }

    void
    Texture::load(const aex::string& name) {
        load(name, TextureConfig::makeDefaultTextureConfig());
    }

    void
    Texture::load(const aex::string& name, const TextureConfig& cfg) {
        unload();
        setFilename(name);
        m_cfg = cfg;
    }

    void
    Texture::load(const std::array<FileKey, 6>& name) {
        load(name, TextureConfig::makeDefaultTextureConfig());
    }

    void
    Texture::load(const std::array<FileKey, 6>& name, const TextureConfig& cfg) {
        unload();
        setFilename(name);
        m_cfg = cfg;
    }

    void
    Texture::setData(void* data, uint32_t w, uint32_t h) {
        if (m_cfg.getTextureType() != TEX_TYPE_2D)
            throw std::runtime_error {
            "Invalid parameters for this texture type.(TEX_TYPE_2D)"
        };

        HWTexture* tex = new HWTexture{m_cfg};

        tex->setTexture2D(data, w, h);

        m_sizeW = w;

        m_sizeH = h;

        m_tex.reset(tex);
        m_isBound = true;
        m_created = true;
        unbind();
    }

    void* Texture::getData(void* data, int lvl) {
        bool disable =false;
        if (!m_isBound) {
            bind();
            disable=true;
        }

        //getTextureData(int target, int lvl, void* data, uint32_t data_type)
        HWTexture* impl = static_cast<HWTexture*> (m_tex.get());

        if (data == nullptr)
        {
            size_t buffersize = m_sizeW * m_sizeH;
            data = new uint[buffersize];
        }


        getTextureData( impl->getBinder(), lvl, data, m_cfg.getFormat(), m_cfg.getTextureType());
        if(disable)
        {
            unbind();
        }

        return data;
    }

    void Texture::setData(void* data, uint32_t w, uint32_t h, uint32_t d) {
        if (m_cfg.getTextureType() != TEX_TYPE_3D)
            throw std::runtime_error {
            "Invalid parameters for this texture type.(!= TEX_TYPE_3D)"
        };

        LOG_DEBUG("TEX_TYPE_3D: ", w, ", ", h, ", ", d, ", ");
        HWTexture* tex = new HWTexture{m_cfg};

        tex->setTexture3D(data, w, h, d);

        m_sizeW = w;
        m_sizeH = h;
        m_sizeD = d;

        m_tex.reset(tex);
        m_isBound = true;
        m_created = true;
        unbind();
    }

    void
    Texture::setData(const std::array<DataKey, 6>& data, uint32_t w, uint32_t h) {
        if (m_cfg.getTextureType() != TEX_TYPE_CUBEMAP)
            throw std::runtime_error {
            "Invalid parameters for this texture type.(TEX_TYPE_CUBEMAP)"
        };

        HWTexture* tex = new HWTexture{m_cfg};

        for (auto& dataKey : data) {
            tex->setTextureCubeSide(CubeMapping{dataKey.first, dataKey.second}, w, h);
        }

        m_sizeW = w;
        m_sizeH = h;
        m_tex.reset(tex);
        m_created = true;
        unbind();
    }

    void
    Texture::allocate2D(uint32_t w, uint32_t h) {
        setData(0, w, h);
    }

    void
    Texture::allocateCube(uint32_t w, uint32_t h) {
        std::array<DataKey, 6> data{
            {
                DataKey
                {0, CubeSide::POS_X},
                DataKey
                {0, CubeSide::POS_Y},
                DataKey
                {0, CubeSide::POS_Z},
                DataKey
                {0, CubeSide::NEG_X},
                DataKey
                {0, CubeSide::NEG_Y},
                DataKey
                {0, CubeSide::NEG_Z},
            }
        };
        setData(data, w, h);
    }

    void
    Texture::allocateCubeFramebuffer(uint32_t w, uint32_t h) {
        std::array<CubeMapping, 6> data{
            {
                CubeMapping
                {0, CubeSide::POS_X},
                CubeMapping
                {0, CubeSide::POS_Y},
                CubeMapping
                {0, CubeSide::POS_Z},
                CubeMapping
                {0, CubeSide::NEG_X},
                CubeMapping
                {0, CubeSide::NEG_Y},
                CubeMapping
                {0, CubeSide::NEG_Z},
            }

        };

        if (m_cfg.getTextureType() != TEX_TYPE_CUBEMAP)
            throw std::runtime_error {
            "Invalid parameters for this texture type.(TEX_TYPE_CUBEMAP)"
        };

        HWTexture* tex = new HWTexture{m_cfg};

        tex->setTextureCube(data, w, h);
        tex->setCubeType();

        m_sizeW = w;
        m_sizeH = h;
        m_tex.reset(tex);
        m_created = true;
        unbind();
    }

    void
    Texture::setMipmap(uint32_t level) {
        if (m_tex) {

            SDL_Surface* bitmap = IMG_Load(m_filename[0].first.c_str());
            SDL_SetSurfaceBlendMode(bitmap, SDL_BLENDMODE_NONE);

            if (bitmap) {
                HWTexture* tex = static_cast<HWTexture*> (m_tex.get());
                tex->setTexture2D(bitmap, level);
                SDL_FreeSurface(bitmap);
            }
        }
    }

    void
    Texture::setMipmap(const std::array<FileKey, 6>& name, uint32_t level) {
        if (m_tex) {
            for (auto& fileKey : name) {
                SDL_Surface* bitmap = IMG_Load(fileKey.first.c_str());
                SDL_SetSurfaceBlendMode(bitmap, SDL_BLENDMODE_NONE);

                if (bitmap) {
                    HWTexture* tex = static_cast<HWTexture*> (m_tex.get());
                    tex->setTextureCubeSide(SDLCubeMapping{bitmap, fileKey.second}, level);
                    SDL_FreeSurface(bitmap);
                }
            }
        }
    }

    void
    Texture::setRawMipmapData(void* data, uint32_t w, uint32_t h, uint32_t level) {
        if (m_tex) {
            HWTexture* tex = static_cast<HWTexture*> (m_tex.get());
            tex->setTexture2D(data, w, h, level);
        }
    }

    void
    Texture::setRawMipmapData(const std::array<DataKey, 6>& data, uint32_t w, uint32_t h, uint32_t level) {
        if (m_tex) {
            for (auto& dataKey : data) {
                HWTexture* tex = static_cast<HWTexture*> (m_tex.get());
                tex->setTextureCubeSide(CubeMapping{dataKey.first, dataKey.second}, w, h, level);
            }
        }
    }

    void Texture::bind() {
        finalize();

        if (!m_isBound) {
            HWTexture* impl = static_cast<HWTexture*> (m_tex.get());

            if (impl) {
                impl->enable();
            }
            m_isBound = true;
        }

    }

    void Texture::handle() {
        finalize();
    }

    void Texture::unbind() {
        if (m_isBound) {
            HWTexture* impl = static_cast<HWTexture*> (m_tex.get());

            if (impl) {
                impl->disable();
            }
            m_isBound=false;
        }

    }

    void
    Texture::unload() {
        m_filename.clear();
        m_tex.reset();
        m_created = false;
    }

    void
    Texture::finalize() {

        if (!m_created) {
            bool isReady = true;
            aex::string error_msg;
            if (!m_filename.empty()) {
                if (m_filename.size() != 1 && m_filename.size() != 6) {
                    isReady = false;
                    error_msg = "Invalid file/files. Could not load texture.(wrong texture count: " + aex::to_string(m_filename.size()) + ")";
                }

                if (m_filename.size() == 1 && m_cfg.getTextureType() != TEX_TYPE_2D) {
                    isReady = false;
                    error_msg = "Invalid file/files. Could not load texture.(type != TEX_TYPE_2D)";
                }

                if (m_filename.size() == 6 && m_cfg.getTextureType() != TEX_TYPE_CUBEMAP) {
                    isReady = false;
                    error_msg = "Invalid file/files. Could not load texture.(type != TEX_TYPE_CUBEMAP)";
                }

                if (m_cfg.getTextureType() == TEX_TYPE_2D) {
                    handle2D();
                } else if (m_cfg.getTextureType() == TEX_TYPE_CUBEMAP) {
                    handleCube();
                }

                m_created = true;
            }


            if (!isReady && !m_filename.empty()) {

                if (m_cfg.getTextureType() == TEX_TYPE_CUBEMAP) {
                    for (FileKey& key : m_filename) {
                        error_msg = error_msg + " " + key.first;
                    }
                } else {
                    error_msg = error_msg + " " + m_filename[0].first;
                }
                LOG_DEBUG(m_filename[0].first);
            }

            if (!isReady) throw std::runtime_error {
                error_msg
            };
        }
    }

    void
    Texture::handle2D() {
        FileManager fManager;
        typedef boost::filesystem::path path;
        path p = fManager.work_dir();
        p /= path{m_filename[0].first};

        //LOG_DEBUG("========================= OPENING TEXTURE 2D AT PATH: ", p.string());

        SDL_Surface* bitmap = IMG_Load(p.string().c_str());
        if (!bitmap) {
            LOG_ERR("Can't load texture(", IMG_GetError(), "): ", m_filename[0].first, " . Trying raw path.");
            bitmap = IMG_Load(m_filename[0].first.c_str());
            if (!bitmap) {
                LOG_ERR("Can't load texture(", IMG_GetError(), "): ", m_filename[0].first);
            }
            else{
                LOG_DEBUG("Texture from raw path loaded!");
            }
        }

        SDL_SetSurfaceBlendMode(bitmap, SDL_BLENDMODE_NONE);

        if (bitmap) {
            m_tex.reset(new HWTexture{m_cfg});
            HWTexture* tex = static_cast<HWTexture*> (m_tex.get());
            tex->setTexture2D(bitmap);
            m_sizeW = bitmap->w;
            m_sizeH = bitmap->h;
            SDL_FreeSurface(bitmap);
        }
        m_isBound = true;
        unbind();
    }

    void
    Texture::handleCube() {
        HWTexture* tex = nullptr;

        for (auto& fileKey : m_filename) {
            SDL_Surface* bitmap = IMG_Load(fileKey.first.c_str());


            if (bitmap) {
                tex = new HWTexture{m_cfg};
                tex->setTextureCubeSide(SDLCubeMapping{bitmap, fileKey.second});
                m_sizeW = bitmap->w;
                m_sizeH = bitmap->h;
                SDL_FreeSurface(bitmap);
            } else {
                LOG_DEBUG("Can't load texture: ", m_filename[0].first);
            }
        }

        m_tex.reset(tex);
    }

    void
    Texture::onContextDirty() {
        m_created = false;
    }

    Texture::~Texture() {
    }

    void Texture::setName(const aex::string& name) {
        m_name = name;
    }

    void Texture::setFilename(const aex::string& name) {
        m_filename.clear();
        m_filename.push_back(FileKey{name, CubeSide::POS_X});
    }

    void Texture::setFilename(const std::array<FileKey, 6>& name) {
        std::vector<FileKey> filenames(6);
        std::copy(name.begin(), name.end(), filenames.begin());
        m_filename.swap(filenames);
    }

    const aex::string& Texture::getName() const {
        if (m_name.empty() && m_filename.size() > 0)
            return m_filename[0].first;
        return m_name;
    }

    void Texture::setSizeH(uint32_t sizeH) {
        m_sizeH = sizeH;
    }

    uint32_t Texture::getSizeH() const {
        return m_sizeH;
    }

    void Texture::setSizeW(uint32_t sizeW) {
        m_sizeW = sizeW;
    }

    uint32_t Texture::getSizeW() const {
        return m_sizeW;
    }

    TextureConfig Texture::getTextureConfig() const {
        return m_cfg;
    }

    const void*
    Texture::getTexture() const {
        return m_tex.get();
    }

    //////////////////////////////////////////////TextureDrw

    #ifdef TEXDRW

    TextureDrw::TextureDrw (aex::Texture* tex, const std::string& name):
    m_texture(tex),
    m_samplerUniform(tex, name)
    {

    }

    void TextureDrw::draw(AexRenderer* gl)
    {
        m_texture->bind();

        auto program = gl->getShader()->GetProgram();
        if(program)
        {
            m_samplerUniform.setUniform(program);
        }

    }

    void TextureDrw::finishdraw(AexRenderer* gl)
    {
         m_texture->unbind();
    }
    #endif

}
