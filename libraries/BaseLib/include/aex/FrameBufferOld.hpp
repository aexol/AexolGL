/*
 * File:   FrameBuffer.hpp
 * Author: mritke
 *
 * Created on February 6, 2014, 9:31 AM
 */

#ifndef FRAMEBUFFEROLD_HPP
#define	FRAMEBUFFEROLD_HPP
#include <aex/Common.hpp>
#include <aex/AexDrawable.hpp>
#include <aex/TextureIncludes.hpp>
#include <aex/Point.hpp>
#include <aex/Box.hpp>
#include <atomic>

namespace aex
{
    class FramebufferObject;
    class FrameBufferOldOGL : public DrawObject
    {

    public:

        FrameBufferOldOGL();
        FrameBufferOldOGL(int x, int y, bool npotAllowed = false);
        void draw(AexRenderer* gl);

        void finishdraw(AexRenderer* gl);
        void init();
        void needsUpdate() {}
        void setScreenSizeY(int screenSizeY);
        int getScreenSizeY() const;
        void setScreenSizeX(int screenSizeX);
        int getScreenSizeX() const;
        void setScreenSize(int screenSizeX, int screenSizeY);
        void setTextureSize(int texSizeX, int texSizeY);
        int getTexSizeX() const;
        int getTexSizeY() const;

        ~FrameBufferOldOGL();
        void setColorTextureName(const std::string& ColorTextureName);
        std::string getColorTextureName() const;
        void setDepthTextureName(const std::string& DepthTextureName);
        std::string getDepthTextureName() const;
        const Texture* getTexture() const;
        Texture* getTexture();
        const Texture* getTextureDepth() const;
        Texture* getTextureDepth();
        FBox2D bufferArea();
        void setForceViewport(int32_t wPix, int32_t hPix, int32_t xPix = 0, int32_t yPix = 0);
        
    private:
        int m_screenSizeX;
        int m_screenSizeY;
        UIPoint2D m_texSize;
        UIPoint2D m_newTexSize;
        UIPoint2D m_fboDrawArea;
        bool m_npotAllowed;
        bool m_recreate;
        aex::unique_ptr<Texture> m_texture;
        aex::unique_ptr<Texture> m_textureDepth;
        std::string m_ColorTextureName;
        std::string m_DepthTextureName;
        std::unique_ptr<FramebufferObject> m_fb;
        UIPoint2D getValidTexSize(UIPoint2D texSize);
        bool m_forceViewport;
        float m_viewPortX;
        float m_viewPortY;
        float m_viewPortW;
        float m_viewPortH;
    };


    typedef shared_ptr<FrameBufferOldOGL> FrameBufferOldOGL_ptr;
}



#endif	/* FRAMEBUFFER_HPP */
