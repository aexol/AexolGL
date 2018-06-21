#include "../include/GLFramebuffer.hpp"
#include "../include/GLRenderbuffer.hpp"
#include "../include/GLTexture.hpp"
#include "../../include/FramebufferObjectInternal.hpp"
#include "../../include/TextureInternal.hpp"
#include "../../include/ContextInternal.hpp"
#include <aex/Common.hpp>
#include <aex/ax_Macros.hpp>
#include <aex/api_definitions.hpp>
namespace aex
{
void setTexBuff(GLenum attachment, GLenum target, GLuint texId)
{
    context_guard ctxGuard;
    return ctxGuard([](aex_context & ctx, GLenum attachment, GLenum target, GLuint texId)
    {
        commitChanges(ctx);
        GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, target, texId, 0));
    }, attachment, target, texId);
}

void setTexBuff(GLenum attachment, const void* tx)
{

    const GLTexture* texture = static_cast<const GLTexture*>(static_cast<const HWTexture*>(tx)->impl());

    if(texture->mTarget == GL_TEXTURE_2D)
    {
        setTexBuff(attachment, GL_TEXTURE_2D, texture->mTexture);
    }
    else
    {
        setTexBuff(attachment, GL_TEXTURE_CUBE_MAP_POSITIVE_X, texture->mTexture);
    }
}
void setTexCubeBuff(GLenum attachment, const void* tx, const Texture::CubeSide& side)
{
    const GLTexture* texture = static_cast<const GLTexture*>(static_cast<const HWTexture*>(tx)->impl());

    if(texture->mTarget == GL_TEXTURE_CUBE_MAP)
    {
        setTexBuff(attachment, textureCubeSideToGLEnum(side), texture->mTexture);
    }
}
void setRenderbuff(GLenum attachment, const void* tx)
{
    context_guard ctxGuard;
    return ctxGuard([](aex_context & ctx, GLenum attachment, const void* tx)
    {
        commitChanges(ctx);
        const GLRenderbuffer* rBuff = static_cast<const GLRenderbuffer*>(tx);
        GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rBuff->mRenderbuffer));
    }, attachment, tx);
}
void* createFramebufferObject()
{
    //GLuint* fbo = new GLuint;
    GLFramebuffer* fbo = new GLFramebuffer();
    if(fbo)
    {
        GL_CHECK(glGenFramebuffers(1, &fbo->id));
        return fbo;
    }
    else{
        LOG_ERR("Could not genaerate Framebuffer!");
        return nullptr;
    }
}
void freeFramebufferObject(void* p)
{
    GL_CHECK(glDeleteFramebuffers(1, &static_cast<const GLFramebuffer*>(p)->id));
    delete static_cast<GLFramebuffer*>(p);
}
void setTexColorBuff(const void* fb, const void* tx)
{
    UNUSED(fb);
    setTexBuff(GL_COLOR_ATTACHMENT0, tx);
}

void setTexColorBuff(const void* fb, int buffer_number, const void* tx)
{
    UNUSED(fb);
    setTexBuff(GL_COLOR_ATTACHMENT0+buffer_number, tx);
}

void setTexColorBuff(const void* fb, const void* tx, const Texture::CubeSide& side)
{
    UNUSED(fb);
    setTexCubeBuff(GL_COLOR_ATTACHMENT0, tx, side);
}
void setRenderbuffColorBuff(const void* fb, const void* tx)
{
    UNUSED(fb);
    setRenderbuff(GL_COLOR_ATTACHMENT0, tx);
}
void setTexDepthBuff(const void* fb, const void* tx)
{
    UNUSED(fb);
    setTexBuff(GL_DEPTH_ATTACHMENT, tx);
}
void setTexDepthBuff(const void* fb, const void* tx, const Texture::CubeSide& side)
{
    UNUSED(fb);
    setTexCubeBuff(GL_DEPTH_ATTACHMENT, tx, side);
}
void setRenderbuffDepthBuff(const void* fb, const void* tx)
{
    UNUSED(fb);
    setRenderbuff(GL_DEPTH_ATTACHMENT, tx);
}
void setTexStencilBuff(const void* fb, const void* tx)
{
    UNUSED(fb);
    setTexBuff(GL_STENCIL_ATTACHMENT, tx);
}
void setTexStencilBuff(const void* fb, const void* tx, const Texture::CubeSide& side)
{
    UNUSED(fb);
    setTexCubeBuff(GL_STENCIL_ATTACHMENT, tx, side);
}
void setRenderbuffStencilBuff(const void* fb, const void* tx)
{
    UNUSED(fb);
    setRenderbuff(GL_STENCIL_ATTACHMENT, tx);
}
bool FramebufferStatus(aex::string& err)
{
    bool ret = true;
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if(status != GL_FRAMEBUFFER_COMPLETE)
    {
        ret = false;

        if(status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
        {
            err = "Incomplete framebuffer attachments.";
        }       
        else if(status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
        {
            err = "Incomplete missing framebuffer attachments.";
        }
        else if(status == GL_FRAMEBUFFER_UNSUPPORTED)
        {
            err = "Combination of internal formats used by attachments in the framebuffer results in a nonrednerable target.";
        }
    }

    return ret;
}
void
GLFramebuffer::onContextDirty()
{
    id = -1;
}



int getMaxColorAttachments()
{
#ifdef GL_MAX_COLOR_ATTACHMENTS
    GLint ret = 1; 
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &ret);
    if(ret==GL_INVALID_ENUM)
    {
        return 1;
    }
    
    return ret;
#else
    return 1;
#endif
}



}
