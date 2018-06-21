#include "../../include/TextureInternal.hpp"
#include "../../include/ContextInternal.hpp"
#include "../../include/UniformInternal.hpp"
#include "../../include/ShaderProgramInternal.hpp"
#include "../include/GLTexture.hpp"
#include "../include/GLContext.hpp"
#include <aex/api_definitions.hpp>
#include <aex/platform_definitions.hpp>
#include <aex/utils.hpp>
#include <aex/ax_Macros.hpp>
#include <array>
namespace aex
{
void setTexture(const void* bitmap, GLsizei width, GLsizei height, GLenum target, GLenum internalTexFormat, GLenum texFormat, const TextureConfig& cfg, uint32_t level = 0);
void setTexture(const void* bitmap, GLsizei width, GLsizei height, GLsizei depth, GLenum target, GLenum internalTexFormat, GLenum texFormat, const TextureConfig& cfg, uint32_t level = 0);
void setTexture(const SDL_Surface* bitmap, const TextureConfig& cfg, GLenum target, uint32_t level);
void genMipmaps(GLenum target, uint32_t width, uint32_t height, GLenum minFilter);
void setTexParams(GLenum target, uint32_t width, uint32_t height, const TextureConfig& cfg);
static CONSTEXPR std::array<GLenum, 4> TextureFormats
{
    {
        GL_RGB,
        GL_RGBA,
        GL_ALPHA,
#ifdef BUILD_FOR_GLES3
        GL_DEPTH_COMPONENT24
#else
        GL_DEPTH_COMPONENT
#endif
    }
};

static CONSTEXPR std::array<GLenum, 7> TextureDataTypes
{
    {
        GL_UNSIGNED_BYTE,
        GL_UNSIGNED_SHORT_5_6_5,
        GL_UNSIGNED_SHORT_4_4_4_4,
        GL_UNSIGNED_SHORT_5_5_5_1,
        GL_UNSIGNED_SHORT,
        GL_UNSIGNED_INT,
        GL_UNSIGNED_INT
    }
};
static CONSTEXPR std::array<GLenum, 6> TextureFilters
{
    {
        GL_NEAREST,
        GL_LINEAR,
        GL_NEAREST_MIPMAP_NEAREST,
        GL_LINEAR_MIPMAP_NEAREST,
        GL_NEAREST_MIPMAP_LINEAR,
        GL_LINEAR_MIPMAP_LINEAR
    }
};

static CONSTEXPR std::array<GLenum, 3> TextureWrap
{
    {
        GL_CLAMP_TO_EDGE,
        GL_REPEAT,
        GL_MIRRORED_REPEAT
    }
};

CONSTEXPR unsigned mask(unsigned bitCount, unsigned startingPos = 0, unsigned ii = 0)
{
    return ii < bitCount ? mask(bitCount, startingPos, ii + 1) | (1 << (startingPos + ii)) : 0;
}

GLTexture* genTexture()
{
    GLTexture* tex = new GLTexture {0, 0, MaxTextures()};
    GL_CHECK(glGenTextures(1, &(tex->mTexture)));
    return tex;
}

void* makeHWTexture2D()
{
    GLTexture* t = genTexture();
    t->mTarget = GL_TEXTURE_2D;
    return t;
}

void* makeHWTextureCube()
{
    GLTexture* t = genTexture();
    t->mTarget = GL_TEXTURE_CUBE_MAP;
    return t;
}

void* makeHWTexture3D()
{
#ifndef BUILD_FOR_GLES2
    GLTexture* t = genTexture();
    t->mTarget = GL_TEXTURE_3D;
    return t;
#else
    return nullptr;
#endif
}

void deleteTexture(void* t)
{
    GLTexture* tt = static_cast<GLTexture*>(t);
    destroyTextureCTX(tt);
    GL_CHECK(glDeleteTextures(1, &(tt->mTexture)));
    delete tt;
}

void freeHWTexture2D(void* t)
{
    deleteTexture(t);
}



void freeHWTextureCube(void* t)
{
    deleteTexture(t);
}

void
HWTexture::delete_hwtex::operator()(void* p) const
{
    deleteTexture(p);
}

HWTexture::HWTexture(const TextureConfig& cfg) :
    m_impl
{
    nullptr, delete_hwtex{}
}

,
mCfg(cfg)
{
}

void
HWTexture::setTexture2D(SDL_Surface* data, uint32_t level)
{
    m_impl.reset(makeHWTexture2D());
    enable();
    setTexture(data, mCfg, GL_TEXTURE_2D, level);
}


void
HWTexture::setTextureCube(const std::array<SDLCubeMapping, 6>& data, uint32_t level)
{
    m_impl.reset(makeHWTextureCube());
    enable();

    for(auto& mapping : data)
    {
        iSetTextureCubeSide(mapping, level);
    }
}

void
HWTexture::setTextureCubeSide(const SDLCubeMapping& data, uint32_t level)
{
    m_impl.reset(makeHWTextureCube());
    enable();
    iSetTextureCubeSide(data, level);
}

void
HWTexture::iSetTextureCubeSide(const SDLCubeMapping& data, uint32_t level)
{
    GLenum target = 0;

    switch(data.type)
    {
    case Texture::CubeSide::POS_X:
        target = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
        break;

    case Texture::CubeSide::POS_Y:
        target = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
        break;

    case Texture::CubeSide::POS_Z:
        target = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
        break;

    case Texture::CubeSide::NEG_X:
        target = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
        break;

    case Texture::CubeSide::NEG_Y:
        target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
        break;

    case Texture::CubeSide::NEG_Z:
        target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
        break;

    default:
        break;
    }

    setTexture(data.data, mCfg, target, level);
}
void
HWTexture::setImpl(impl_ptr&& ni)
{
    m_impl.swap(ni);
}

void
HWTexture::setTexture2D(void* data, uint32_t w, uint32_t h, uint32_t level)
{
    m_impl.reset(makeHWTexture2D());
    enable();
    GLenum format = TextureFormats[mCfg.getFormat()];
    
    aex::setTexture(data, w, h, GL_TEXTURE_2D, format, format, mCfg, level);
}

void 
HWTexture::setTexture3D(void* data, uint32_t w, uint32_t h, uint32_t d, uint32_t level)
{
#ifndef BUILD_FOR_GLES2
    m_impl.reset(makeHWTexture3D());
    enable();
    GLenum format = TextureFormats[mCfg.getFormat()];
    aex::setTexture(data, w, h, d, GL_TEXTURE_3D, format, format, mCfg, level);
#endif
}

void
HWTexture::setTextureCubeSide(const CubeMapping& data, uint32_t w, uint32_t h, uint32_t level)
{
    m_impl.reset(makeHWTexture2D());
    enable();
    iSetTextureCubeSide(data, w, h, level);
}

void
HWTexture::setTextureCube(const std::array<CubeMapping, 6>& data, uint32_t w, uint32_t h, uint32_t level)
{
    m_impl.reset(makeHWTextureCube());
    enable();
    for(auto& mapping : data)
    {
        iSetTextureCubeSide(mapping, w, h, level);
    }
}

void
HWTexture::iSetTextureCubeSide(const CubeMapping& data, uint32_t w, uint32_t h, uint32_t level)
{
    GLenum target = 0;

    switch(data.type)
    {
    case Texture::CubeSide::POS_X:
        target = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
        break;

    case Texture::CubeSide::POS_Y:
        target = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
        break;

    case Texture::CubeSide::POS_Z:
        target = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
        break;

    case Texture::CubeSide::NEG_X:
        target = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
        break;

    case Texture::CubeSide::NEG_Y:
        target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
        break;

    case Texture::CubeSide::NEG_Z:
        target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
        break;

    default:
        break;
    }

    GLenum format = TextureFormats[mCfg.getFormat()];

    aex::setTexture(data.data, w, h, target, format, format, mCfg, level);

}

void HWTexture::setCubeType()
{
    if(m_impl)
    {
        GLTexture* tt = static_cast<GLTexture*>(m_impl.get());
        tt->mTarget=GL_TEXTURE_CUBE_MAP;
    }
}

bool
HWTexture::enable() const
{
    bool enabled = false;

    if(m_impl)
    {
        GLTexture* tt = static_cast<GLTexture*>(m_impl.get());
        setTextureCTX(tt);
        enabled = true;
    }

    return enabled;
}


void
HWTexture::disable() const
{
    if(m_impl)
    {
        GLTexture* tt = static_cast<GLTexture*>(m_impl.get());
        releaseTextureCTX(tt);
    }
}

void
HWTexture::refreshTexParams(uint32_t w, uint32_t h)
{
    if(enable() )
    {
        GLTexture* tt = static_cast<GLTexture*>(m_impl.get());
        setTexParams( tt->mTarget, w, h, mCfg); // to be not pow of 2
        disable();
    }
}

int  HWTexture::getBinder()
{
    GLTexture* tt = static_cast<GLTexture*>(m_impl.get());
    return tt->mTURef;
}

const void*
HWTexture::impl() const
{
    return m_impl.get();
}

bool isTextureActive(aex_context& ctx, GLTexture* t, std::vector<GLTextureUnit>& TUs)
{
    GLTexture* tex = nullptr;

    if(t->mTURef < ctx.MaxTexUnits)
    {
        tex = TUs[t->mTURef].mTexture;
    }

    return t->mTURef < ctx.MaxTexUnits &&
            tex != nullptr &&
            tex == t &&
            (TUs[t->mTURef].mSkip == true);
}

void setTextureInUnit(aex_context& ctx, GLTexture* t, size_t unit, bool newBinding)
{
    if(ctx.CurrentTextureUnit != unit)
    {
        ctx.CurrentTextureUnit = unit;
        GL_CHECK(glActiveTexture(GL_TEXTURE0 + unit));
    }
   
    if(newBinding)
    {
        GL_CHECK(glBindTexture(t->mTarget, t->mTexture));
    }
}

void setActiveTex(GLTexture* t, std::queue<size_t>& ring, std::vector<GLTextureUnit>& TUs)
{
    if(ring.empty())
    {
        LOG_ERR("Not enough system resources.(To many textures bound)");
        throw std::runtime_error
        {
            "Not enough system resources.(To many textures bound)"
        };
    }

    context_guard ctxGuard {};
    return ctxGuard([](aex_context & ctx, GLTexture * t, std::queue<size_t>* ring_p, std::vector<GLTextureUnit>* TUs_p)
    {
        std::queue<size_t>& ring = *ring_p;
        std::vector<GLTextureUnit>& TUs = *TUs_p;
        size_t indexFound = ctx.MaxTexUnits;
        bool newBinding= true;
        
        if(TUs[t->mTURef].mTexture != t)
        {
            while(indexFound == ctx.MaxTexUnits && !ring.empty())
            {
                size_t nextFree = ring.front();
                ring.pop();

                if(!TUs[nextFree].mSkip)
                {
                    indexFound = nextFree;
                }
            }
            

            if(indexFound == ctx.MaxTexUnits)
            {
                LOG_ERR("Not enough system resources.(Max texture units.)");
                throw std::runtime_error
                {
                    "Not enough system resources.(Max textur units.)"
                };
            }
        }
        else
        {
            indexFound =  t->mTURef;
            newBinding= false;
        }

        TUs[indexFound].mTexture = t;
        t->mTURef = indexFound;
        TUs[indexFound].mSkip = true;
   
        setTextureInUnit(ctx, t, indexFound, newBinding);
    }, t, &ring, &TUs);
}

static void initCustomTU(aex_context& ctx, TextureUnits& TUs)
{
    if(TUs.mTUs.size() != ctx.MaxTexUnits)
    {
        TUs.mTUs.resize(ctx.MaxTexUnits);
        std::fill(TUs.mTUs.begin(), TUs.mTUs.end(), GLTextureUnit
        {
            nullptr, false
        });
        GLint n = ctx.MaxTexUnits > 0 ? ctx.MaxTexUnits : 0;
        GLint m = n;

        while(n)
        {
            GLint k = m - n;
            TUs.mAvailable.push(k);
            n--;
        }
    }
}

void pickTexBuffers(aex_context& ctx, GLTexture* t, std::vector<GLTextureUnit>** TU, std::queue<size_t>** AvailTUs)
{
    GLenum target = t->mTarget;

    if(target == GL_TEXTURE_2D)
    {
        *TU = &ctx.TextureUnits.m2D.mTUs;
        *AvailTUs = &ctx.TextureUnits.m2D.mAvailable;
    }
    else if(target == GL_TEXTURE_CUBE_MAP)
    {
        *TU = &ctx.TextureUnits.mCubes.mTUs;
        *AvailTUs = &ctx.TextureUnits.mCubes.mAvailable;
    }
    else
    {
        initCustomTU(ctx, ctx.TextureUnits.mCustom[target]);
        *TU = &ctx.TextureUnits.mCustom[target].mTUs;
        *AvailTUs = &ctx.TextureUnits.mCustom[target].mAvailable;
    }
}

void setTexture(const SDL_Surface* bitmap, const TextureConfig& cfg, GLenum target, uint32_t level)
{
    void* bits;
    uint32_t width;
    uint32_t height;
    int32_t nOfColors;
    nOfColors = bitmap->format->BytesPerPixel;
    GLenum texFormat = 0;
    GLenum internalTexFormat = 0;

    if(nOfColors == 4)
    {
        if(bitmap->format->Rmask == 0x000000ff)
        {
            internalTexFormat = texFormat = GL_RGBA;
        }
        else if(bitmap->format->Bmask == 0x000000ff)
        {
#if __TARGET_PLATFORM__ != __ANDROID_OS__ 
            texFormat = GL_BGRA;
#endif
            internalTexFormat = GL_RGBA;
        }
    }
    else if(nOfColors == 3)    // no alpha channel
    {
        if(bitmap->format->Rmask == 0x000000ff)
        {
            internalTexFormat = texFormat = GL_RGB;
        }
    }
    else
    {
        LOG_WARN("Texture is not truecolor.");
    }

    bits = bitmap->pixels;
    width = bitmap->w;
    height = bitmap->h;

    if((bits == 0) || (width == 0) || (height == 0))
    {
        LOG_ERR("Failed to load texture.");
        return;
    }
    
    
    setTexture(bits, width, height, target, internalTexFormat, texFormat, cfg, level);
}

void setTexture(const void* bits, GLsizei width, GLsizei height, GLenum target, GLenum internalTexFormat, GLenum texFormat, const TextureConfig& cfg, uint32_t level)
{

    setTexParams(target, width, height, cfg);
    
#ifdef BUILD_FOR_GLES3
        LOG_DEBUG(" ==========================    BUILD FOR GLES 3.0");
        if( (internalTexFormat == GL_DEPTH_COMPONENT24) || (internalTexFormat == GL_DEPTH_COMPONENT16) || (internalTexFormat == GL_DEPTH_COMPONENT32) )
            texFormat = GL_DEPTH_COMPONENT;  
#endif
    
    GL_CHECK(glTexImage2D(target, level, internalTexFormat, width, height, 0, texFormat, TextureDataTypes[cfg.getDataType()], bits));

    if(cfg.hasGeneratedMipmaps() && level == 0)
    {
        genMipmaps(target, width, height, TextureDataTypes[cfg.getMinFilter()]);
    }
}

void setTexture(const void* bits, GLsizei width, GLsizei height, GLsizei depth, GLenum target, GLenum internalTexFormat, GLenum texFormat, const TextureConfig& cfg, uint32_t level)
{
#if !defined(HAVE_GLES2_GL2_H) && !defined(HAVE_OPENGLES_ES2_GL_H)
    
    setTexParams(target, width, height, cfg);
    GL_CHECK(glTexImage3D(target, level, internalTexFormat, width, height, depth, 0, texFormat, TextureDataTypes[cfg.getDataType()], bits));
    
    LOG_DEBUG("glTexImage3D: ",level,", ",width,", ",height,", ",depth,", ",texFormat, ", " ,(((const uint32_t*)bits)[0]), " ",(((const uint32_t*)bits)[1]), " ",(((const uint32_t*)bits)[2]), " ",(((const uint32_t*)bits)[3]));
    if(cfg.hasGeneratedMipmaps() && level == 0)
    {
        genMipmaps(target, width, height, TextureDataTypes[cfg.getMinFilter()]);
    }
    
#endif
}

void genMipmaps(GLenum target, uint32_t width, uint32_t height, GLenum minFilter)
{
    if(is_pow_2(height) && is_pow_2(width) && minFilter != GL_LINEAR && minFilter != GL_NEAREST)
    {
        generateMipmapCTX(target);
    }
}

void setTexParams(GLenum target, uint32_t width, uint32_t height, const TextureConfig& cfg)
{
#if !defined(HAVE_GLES2_GL2_H) && !defined(HAVE_OPENGLES_ES2_GL_H)
    if(target!=GL_TEXTURE_2D && target!=GL_TEXTURE_3D)
#else
    if(target!=GL_TEXTURE_2D)
#endif
    {
        target=GL_TEXTURE_CUBE_MAP;
    }
    
    
    if(is_pow_2(height) && is_pow_2(width))
    {
        GL_CHECK(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, TextureFilters[cfg.getMinFilter()]));
        GL_CHECK(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, TextureFilters[cfg.getMagFilter()]));
        GL_CHECK(glTexParameteri(target, GL_TEXTURE_WRAP_S, TextureWrap[cfg.getWrapS()]));
        GL_CHECK(glTexParameteri(target, GL_TEXTURE_WRAP_T, TextureWrap[cfg.getWrapT()]));
    }
    else
    {
        auto minFilter = cfg.getMinFilter();
        auto magFilter = cfg.getMagFilter();
        GLenum glMinFilter = minFilter != TEX_FILTER_LINEAR && minFilter != TEX_FILTER_NEAREST ? GL_LINEAR : TextureFilters[cfg.getMinFilter()];
        GLenum glMagFilter = magFilter != TEX_FILTER_LINEAR && magFilter != TEX_FILTER_NEAREST ? GL_LINEAR : TextureFilters[cfg.getMagFilter()];
        GL_CHECK(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, glMinFilter));
        GL_CHECK(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, glMagFilter));
        GL_CHECK(glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CHECK(glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    }
}


GLenum textureCubeSideToGLEnum(const Texture::CubeSide& side)
{
    GLenum target = GL_TEXTURE_CUBE_MAP_POSITIVE_X;

    switch(side)
    {
    case Texture::CubeSide::POS_X:
        target = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
        break;

    case Texture::CubeSide::POS_Y:
        target = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
        break;

    case Texture::CubeSide::POS_Z:
        target = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
        break;

    case Texture::CubeSide::NEG_X:
        target = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
        break;

    case Texture::CubeSide::NEG_Y:
        target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
        break;

    case Texture::CubeSide::NEG_Z:
        target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
        break;

    default:
        break;
    }

    return target;
}

GLTexture::GLTexture(GLuint tex, GLuint target, size_t ref) :
    mTexture(tex),
    mTarget(target),
    mTURef(ref)
{
}
void
GLTexture::onContextDirty()
{
    mTexture = 0;
}
}
