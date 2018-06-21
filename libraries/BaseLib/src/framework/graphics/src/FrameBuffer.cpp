#include <aex/FrameBuffer.hpp>
#include <aex/Logger.hpp>
#include "aex/ShaderDrw.hpp"
#include "../include/TextureInternal.hpp"
#include "../include/FramebufferObjectInternal.hpp"

namespace aex {

    /// std::atomic<uint32_t> FrameBufferOGL::nFbos(0);

    FrameBufferOGL::FrameBufferOGL() :
    m_texSize{
        0, 0
    }

    ,
    m_npotAllowed(false),
    m_depthAttachment(true),
    m_recreate(false)
    {
        ViewPort vp = CurrentViewport();
        m_screenSizeX = vp.w;
        m_screenSizeY = vp.h;
        m_newTexSize.x() = m_screenSizeX;
        m_newTexSize.y() = m_screenSizeY;
        init();
    }

    FrameBufferOGL::FrameBufferOGL(int x, int y, bool npotAllowed, bool isCube) :
    m_texSize{
        0, 0
    },
    m_newTexSize{x, y}

    ,
    m_npotAllowed(npotAllowed),
    m_isCube(isCube),
    m_depthAttachment(true),
    m_recreate(false)
    {
        ViewPort vp = CurrentViewport();
        m_screenSizeX = vp.w;
        m_screenSizeY = vp.h;

        init();
    }

    UIPoint2D
    FrameBufferOGL::getValidTexSize(UIPoint2D texSize)
    {
        UIPoint2D newTexSize = texSize;

        if (!m_npotAllowed)
        {
            unsigned pot = texSize.x() > texSize.y() ? texSize.x() : texSize.y();
            pot--;

            for (size_t ii = 1; ii < sizeof (UIPoint2D::value_type); ii = ii * 2)
            {
                pot |= pot >> ii;
            }

            pot++;
            newTexSize = UIPoint2D{pot, pot};
        }

        return newTexSize;
    }

    void FrameBufferOGL::init()
    {
        m_fboDrawArea = m_newTexSize;
        m_newTexSize = getValidTexSize(m_newTexSize);

        m_depthAttachment = true;
        //Skip if same resize to same size
        if (m_recreate && (m_texSize == m_newTexSize))
        {
            m_recreate = false;
            return;
        }
        LOG_DEBUG("FBO_DRAW_AREA: ", m_fboDrawArea.x(), " - ", m_fboDrawArea.y());

        m_texSize = m_newTexSize;
        LOG_DEBUG("m_texSize.x(): ", m_texSize.x(), " ", "m_texSize.y(): ", m_texSize.y());

        TextureConfig cfg;
        if (!m_isCube)
        {
            cfg = TextureConfig::makeDefaultTextureConfig();
        } else
        {
            cfg = TextureConfig::makeDefaultTextureCubeConfig();
        }

        if (cfg.hasGeneratedMipmaps())
        {
            cfg.toggleGenerateMipmaps();
        }

        //cfg.set8bitPerChannel();
        cfg.setRGBAFormat();
        if (!m_npotAllowed)
        {
            cfg.setMinFilterNearest();
            cfg.setMagFilterNearest();
        } else
        {
            cfg.setMinFilterLinear();
            cfg.setMagFilterLinear();
        }
        cfg.disableMipMapping();

        if (m_colorAttachments.size() > 0)
        {
            m_colorAttachments[0].reset(new Texture(cfg));
        } else
        {
            m_colorAttachments.emplace_back(new Texture(cfg));
        }

        if (!m_isCube)
        {
            m_colorAttachments[0]->allocate2D(m_texSize.x(), m_texSize.y());
        } else
        {
            m_colorAttachments[0]->allocateCubeFramebuffer(m_texSize.x(), m_texSize.y());
        }

        //        m_texture->setTarget(GL_TEXTURE_2D);


        TextureConfig cfgDepth;

        if (m_depthAttachment)
        {
            if (!m_isCube)
            {
                cfgDepth = TextureConfig::makeDefaultTextureConfig();

            } else
            {
                cfgDepth = TextureConfig::makeDefaultTextureCubeConfig();
            }

            cfgDepth.setDepthComponentFormat();
            if (!m_npotAllowed)
            {
                cfgDepth.setMinFilterNearest();
                cfgDepth.setMagFilterNearest();
            } else
            {
                cfgDepth.setMinFilterLinear();
                cfgDepth.setMagFilterLinear();
            }
            cfgDepth.set24bitDepth();


            if (cfgDepth.hasGeneratedMipmaps())
            {
                cfgDepth.toggleGenerateMipmaps();
            }

            m_textureDepth.reset(new Texture(cfgDepth));

            if (!m_isCube)
            {
                m_textureDepth->allocate2D(m_texSize.x(), m_texSize.y());
                m_textureDepth->setData(0, m_texSize.x(), m_texSize.y());
            } else
            {
                m_textureDepth->allocateCubeFramebuffer(m_texSize.x(), m_texSize.y());
            }
        }

        m_fb.reset(new FramebufferObject);

        m_fb->enable();

        if (!m_isCube)
        {
            m_fb->addColorBuffer(m_colorAttachments[0].get());

            if (m_depthAttachment)
            {
                m_fb->setDepthBuffer(m_textureDepth.get());
            }
            
        } else
        {
            m_fb->setColorBuffer(m_colorAttachments[0].get(), Texture::CubeSide::POS_X);

            if (m_depthAttachment)
            {
                m_fb->setDepthBuffer(m_textureDepth.get(), Texture::CubeSide::POS_X);
            }
        }

        aex::string err;

        if (!m_fb->valid(err))
        {
            LOG_ERR(err, " size: ", m_texSize.x(), ", ", m_texSize.y(), ", is Cube: ", m_isCube);
        }

        m_fb->disable();
        m_recreate = false;
    }

    Texture* FrameBufferOGL::addColorAttachment()
    {

        if (m_fb->isAttachmentAvailable())
        {

            m_fb->enable();

            TextureConfig cfg;
            if (!m_isCube)
            {
                cfg = TextureConfig::makeDefaultTextureConfig();
            } else
            {
                cfg = TextureConfig::makeDefaultTextureCubeConfig();
            }

            if (cfg.hasGeneratedMipmaps())
            {
                cfg.toggleGenerateMipmaps();
            }

            //cfg.set8bitPerChannel();
            cfg.setRGBAFormat();
            cfg.setMinFilterNearest();
            cfg.setMagFilterNearest();
            cfg.disableMipMapping();

            m_colorAttachments.emplace_back(new Texture(cfg));

            if (!m_isCube)
            {
                m_colorAttachments.back()->allocate2D(m_texSize.x(), m_texSize.y());
            } else
            {
                m_colorAttachments.back()->allocateCube(m_texSize.x(), m_texSize.y());
            }

            m_fb->addColorBuffer(m_colorAttachments.back().get());


            m_fb->disable();

            return m_colorAttachments.back().get();

        } else
        {
            return nullptr;
        }
    }

    Texture* FrameBufferOGL::getColorAttachment(uint i)
    {
        if (i < m_colorAttachments.size())
        {
            return m_colorAttachments[i].get();
        } else
        {
            return nullptr;
        }
    }

    void FrameBufferOGL::draw(AexRenderer* gl)
    {
        UNUSED(gl);

        if (!m_isInit)
        {
            ViewPort vp = CurrentViewport();
            //LOG_DEBUG("FrameBufferOGL::draw CurrentViewport ::  (m_screenSizeX, m_screenSizeY); ", vp.w," ", vp.h);
            m_screenSizeX = vp.w;
            m_screenSizeY = vp.h;
            m_isInit = true;
        }

        if (m_recreate)
        {
            init();
            m_isInit = false;
        }

        if (m_isCube)
        {
            m_fb->setCubeSide(m_cubeSide);
        }

        m_fb->enable();
        //LOG_DEBUG("FrameBufferOGL::draw setViewPort  (m_screenSizeX, m_screenSizeY); ", m_texSize.x()," ", m_texSize.y());
        setViewPort(m_texSize.x(), m_texSize.y());
        clearColorDepthStencilBuffers();
    }

    void FrameBufferOGL::finishdraw(AexRenderer* gl)
    {
        UNUSED(gl);
        m_fb->disable( );

        //LOG_DEBUG("FrameBufferOGL::finishdraw setViewPort  (m_screenSizeX, m_screenSizeY); ", m_screenSizeX," ", m_screenSizeY, " from: ", m_texSize.x()," ,",m_texSize.y());
        setViewPort(m_screenSizeX, m_screenSizeY);
    }

    FrameBufferOGL::~FrameBufferOGL()
    {
    }

    void FrameBufferOGL::setScreenSizeY(int screenSizeY)
    {
        m_screenSizeY = screenSizeY;
    }

    int FrameBufferOGL::getScreenSizeY() const
    {
        return m_screenSizeY;
    }

    void FrameBufferOGL::setScreenSizeX(int screenSizeX)
    {
        m_screenSizeX = screenSizeX;
    }

    void FrameBufferOGL::setScreenSize(int screenSizeX, int screenSizeY)
    {
        m_screenSizeX = screenSizeX;
        m_screenSizeY = screenSizeY;
    }

    void FrameBufferOGL::setTextureSize(int texSizeX, int texSizeY)
    {
        m_newTexSize = UIPoint2D{texSizeX, texSizeY};
        m_recreate = true;
    }

    int FrameBufferOGL::getScreenSizeX() const
    {
        return m_screenSizeX;
    }

    int FrameBufferOGL::getTexSizeX() const
    {
        return m_texSize.x();
    }

    int FrameBufferOGL::getTexSizeY() const
    {
        return m_texSize.y();
    }

    int FrameBufferOGL::getCollorAttachmentsSize() const
    {
        return m_fb->getCollorAttachmentsSize();
    }

    void FrameBufferOGL::setColorTextureName(const aex::string& ColorTextureName)
    {
        m_ColorTextureName = ColorTextureName;
    }

    aex::string FrameBufferOGL::getColorTextureName() const
    {
        return m_ColorTextureName;
    }

    void FrameBufferOGL::setDepthTextureName(const aex::string& DepthTextureName)
    {
        m_DepthTextureName = DepthTextureName;
    }

    aex::string FrameBufferOGL::getDepthTextureName() const
    {
        return m_DepthTextureName;
    }

    const Texture* FrameBufferOGL::getTexture() const
    {
        return m_colorAttachments[0].get();
    }

    Texture* FrameBufferOGL::getTexture()
    {
        return m_colorAttachments[0].get();
    }

    const Texture* FrameBufferOGL::getTextureDepth() const
    {
        return m_textureDepth.get();
    }

    Texture* FrameBufferOGL::getTextureDepth()
    {
        return m_textureDepth.get();
    }

    FBox2D
    FrameBufferOGL::bufferArea()
    {
        return FBox2D{
            FPoint2D
            {0.0f, 0.0f},
            FPoint2D
            {1.0f, 1.0f}};
    }

    void FrameBufferOGL::setCubeSide(int cubeSide)
    {
        m_cubeSide = static_cast<Texture::CubeSide> (cubeSide);
    }

    int FrameBufferOGL::getCubeSide() const
    {
        return (int) m_cubeSide;
    }

    FrameBufferOGL::operator bool()
    {
        return bool(m_fb);
    }
}
