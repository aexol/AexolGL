#include <aex/FrameBufferOld.hpp>
#include <aex/Logger.hpp>
#include "aex/ShaderDrw.hpp"
#include "../include/TextureInternal.hpp"
#include "../include/FramebufferObjectInternal.hpp"

namespace aex
{
    

/// std::atomic<uint32_t> FrameBufferOldOGL::nFbos(0);

    FrameBufferOldOGL::FrameBufferOldOGL() :
        m_texSize{0, 0},
        m_npotAllowed(false),
        m_recreate(false),
        m_forceViewport(false)
    {
        ViewPort vp = CurrentViewport();
        m_screenSizeX = vp.w;
        m_screenSizeY = vp.h;
        m_newTexSize.x() = m_screenSizeX;
        m_newTexSize.y() = m_screenSizeY;
        init();
    }

    FrameBufferOldOGL::FrameBufferOldOGL(int x, int y, bool npotAllowed) :
        m_texSize{0, 0},
        m_newTexSize{x, y},
        m_npotAllowed(npotAllowed),
        m_recreate(false),
        m_forceViewport(false)
    {
        ViewPort vp = CurrentViewport();
        m_screenSizeX = vp.w;
        m_screenSizeY = vp.h;

        init();
    }

    UIPoint2D
    FrameBufferOldOGL::getValidTexSize(UIPoint2D texSize)
    {
        UIPoint2D newTexSize = texSize;

        if(!m_npotAllowed)
        {
            unsigned pot = 1;
            pot = texSize.x() > texSize.y() ? texSize.x() : texSize.y();
            pot--;

            for(size_t ii = 1; ii < sizeof(UIPoint2D::value_type); ii = ii * 2)
            {
                pot |= pot >> ii;
            }

            pot++;
            newTexSize = UIPoint2D{pot, pot};
        }

        return newTexSize;
    }

    void FrameBufferOldOGL::init()
    {
        m_fboDrawArea = m_newTexSize;
        m_newTexSize = getValidTexSize(m_newTexSize);

        //Skip if same resize to same size
        if(m_recreate && m_texSize == m_newTexSize)
        {
            m_recreate = false;
            return;
        }

        m_texSize = m_newTexSize;
        LOG_DEBUG("m_texSize.x(): ", m_texSize.x(), " ", "m_texSize.y(): ", m_texSize.y());
        LOG_DEBUG("HUJ");
        
        TextureConfig cfg = TextureConfig::makeDefaultTextureConfig();

        if(cfg.hasGeneratedMipmaps())
        {
            cfg.toggleGenerateMipmaps();
        }

        cfg.setRGBAFormat();
        cfg.setMinFilterNearest();
        cfg.setMagFilterNearest();
        m_texture.reset(new Texture(cfg));
        m_texture->allocate2D(m_texSize.x(), m_texSize.y());
        // m_texture->setTarget(GL_TEXTURE_2D);
        cfg.setRGBAFormat();
        cfg.setMinFilterNearest();
        cfg.setMagFilterNearest();
        m_textureDepth.reset(new Texture(cfg));
        m_textureDepth->setData(0, m_texSize.x(), m_texSize.y());
        
        m_fb.reset(new FramebufferObject);

        m_fb->enable();
        m_fb->setColorBuffer(m_texture.get());
        m_fb->setDepthBuffer(m_textureDepth.get());

        std::string err;

        if(!m_fb->valid(err))
        {
            LOG_ERR(err);
        }

        m_fb->disable();
        m_recreate = false;
    }

    void FrameBufferOldOGL::draw(AexRenderer* gl)
    {
        if(m_recreate)
        {
            init();
        }

        m_fb->enable();
        
        if(!m_forceViewport)
            setViewPort(m_texSize.x(), m_texSize.y());
        else
            setViewPort(m_viewPortW, m_viewPortH, m_viewPortX, m_viewPortY);
        clearColorDepthStencilBuffers();
    }

    void FrameBufferOldOGL::finishdraw(AexRenderer* gl)
    {
        m_fb->disable();
        setViewPort(m_screenSizeX, m_screenSizeY);
    }

    FrameBufferOldOGL::~FrameBufferOldOGL()
    {
    }

    void FrameBufferOldOGL::setScreenSizeY(int screenSizeY)
    {
        m_screenSizeY = screenSizeY;
    }

    int FrameBufferOldOGL::getScreenSizeY() const
    {
        return m_screenSizeY;
    }

    void FrameBufferOldOGL::setScreenSizeX(int screenSizeX)
    {
        m_screenSizeX = screenSizeX;
    }

    void FrameBufferOldOGL::setScreenSize(int screenSizeX, int screenSizeY)
    {
        m_screenSizeX = screenSizeX;
        m_screenSizeY = screenSizeY;
    }
    void FrameBufferOldOGL::setTextureSize(int texSizeX, int texSizeY)
    {
        m_newTexSize = UIPoint2D{texSizeX, texSizeY};
        m_recreate = true;
    }

    int FrameBufferOldOGL::getScreenSizeX() const
    {
        return m_screenSizeX;
    }

    int FrameBufferOldOGL::getTexSizeX() const
    {
        return m_texSize.x();
    }

    int FrameBufferOldOGL::getTexSizeY() const
    {
        return m_texSize.y();
    }

    void FrameBufferOldOGL::setColorTextureName(const std::string& ColorTextureName)
    {
        m_ColorTextureName = ColorTextureName;
    }

    std::string FrameBufferOldOGL::getColorTextureName() const
    {
        return m_ColorTextureName;
    }

    void FrameBufferOldOGL::setDepthTextureName(const std::string& DepthTextureName)
    {
        m_DepthTextureName = DepthTextureName;
    }

    std::string FrameBufferOldOGL::getDepthTextureName() const
    {
        return m_DepthTextureName;
    }

    const Texture* FrameBufferOldOGL::getTexture() const
    {
        return m_texture.get();
    }

    Texture* FrameBufferOldOGL::getTexture()
    {
        return m_texture.get();
    }

    const Texture* FrameBufferOldOGL::getTextureDepth() const
    {
        return m_textureDepth.get();
    }

    Texture* FrameBufferOldOGL::getTextureDepth()
    {
        return m_textureDepth.get();
    }
    FBox2D
    FrameBufferOldOGL::bufferArea()
    {
        return FBox2D
        {
            FPoint2D{0.0f, 0.0f},
            FPoint2D{1.0f, 1.0f}
        };
    }
    
    void FrameBufferOldOGL::setForceViewport(int32_t wPix, int32_t hPix, int32_t xPix, int32_t yPix)
    {
        m_viewPortX = xPix;
        m_viewPortY = yPix;
        m_viewPortW = wPix;
        m_viewPortH = hPix;
        m_forceViewport = true;
        setViewPort(m_viewPortW, m_viewPortH, m_viewPortX, m_viewPortY);
    }
}