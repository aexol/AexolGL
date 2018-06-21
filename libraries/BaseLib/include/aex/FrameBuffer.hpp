/*
 * File:   FrameBuffer.hpp
 * Author: mritke
 *
 * Created on February 6, 2014, 9:31 AM
 */

#ifndef FRAMEBUFFER_HPP
#define	FRAMEBUFFER_HPP
#include <aex/Common.hpp>
#include <aex/AexDrawable.hpp>
#include <aex/TextureIncludes.hpp>
#include <aex/Point.hpp>
#include <aex/Box.hpp>
#include <atomic>
namespace aex
{
    class FramebufferObject;
    class AEX_PUBLIC_API FrameBufferOGL : public DrawObject
    {
    public:

        FrameBufferOGL();
        FrameBufferOGL(int x, int y, bool npotAllowed = false, bool isCube =false);
        ~FrameBufferOGL();
        
        virtual void draw(AexRenderer* gl);
        virtual void finishdraw(AexRenderer* gl);
        
        Texture* addColorAttachment();
        Texture* getColorAttachment(uint i);
        
        void needsUpdate() {}
        
        void setScreenSizeY(int screenSizeY);
        int getScreenSizeY() const;
        void setScreenSizeX(int screenSizeX);
        int getScreenSizeX() const;
        void setScreenSize(int screenSizeX, int screenSizeY);
        void setTextureSize(int texSizeX, int texSizeY);
        int getTexSizeX() const;
        int getTexSizeY() const;
        int getCollorAttachmentsSize() const;

        void setColorTextureName(const aex::string& ColorTextureName);
        aex::string getColorTextureName() const;
        void setDepthTextureName(const aex::string& DepthTextureName);
        aex::string getDepthTextureName() const;
        const Texture* getTexture() const;
        Texture* getTexture();
        const Texture* getTextureDepth() const;
        Texture* getTextureDepth();
        FBox2D bufferArea();
        void setCubeSide(int cubeSide);
        int getCubeSide() const;
        operator bool();
        
    private:
        bool m_isInit = false;
        int m_screenSizeX;
        int m_screenSizeY;
        UIPoint2D m_texSize;
        UIPoint2D m_newTexSize;
        UIPoint2D m_fboDrawArea;
        bool m_npotAllowed;
        bool m_isCube = false;
        bool m_depthAttachment;
        bool m_recreate;
        std::vector<aex::unique_ptr<Texture>> m_colorAttachments;
        aex::unique_ptr<Texture> m_textureDepth;
        aex::string m_ColorTextureName;
        aex::string m_DepthTextureName;
        std::unique_ptr<FramebufferObject> m_fb;
        UIPoint2D getValidTexSize(UIPoint2D texSize);
        Texture::CubeSide  m_cubeSide=Texture::CubeSide::POS_X; 
        virtual void init();
    };
    
    typedef FrameBufferOGL FboGL;
}



#endif	/* FRAMEBUFFER_HPP */

