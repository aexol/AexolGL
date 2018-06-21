#include "../include/FramebufferObjectInternal.hpp"
#include "../include/RenderbufferObjectInternal.hpp"
#include "../include/ContextInternal.hpp"
#include "../include/TextureInternal.hpp"
#include <aex/Texture.hpp>
namespace aex
{
void
FramebufferObject::delete_frambufferobject::operator()(void* p)
{
    freeFramebufferObject(p);
}
FramebufferObject::FramebufferObject():
    m_impl {createFramebufferObject(), delete_frambufferobject{}},
    mColorBuffer {},
    mDepthBuffer {},
    mStencilBuffer {},
    mPrevFbo{nullptr}
{
}

void
FramebufferObject::setColorBuffer(const BufferBinding::BINDING_TYPE& bType, const void* buff, int att)
{
    //enable();
    mColorBuffer = BufferBinding {bType, buff};

}

void
FramebufferObject::setDepthBuffer(const BufferBinding::BINDING_TYPE& bType, const void* buff)
{
    //enable();
    mDepthBuffer = BufferBinding {bType, buff};
   
}

void
FramebufferObject::setStencilBuffer(const BufferBinding::BINDING_TYPE& bType, const void* buff)
{
    //enable();
    mStencilBuffer = BufferBinding {bType, buff};
    
}

void
FramebufferObject::setColorBuffer(const aex::RenderbufferObject* rBuff)
{
    const void* rBuffImpl;

    if(rBuff && (rBuffImpl = rBuff->getRenderbuffer()))
    {
        setColorBuffer(BufferBinding::BINDING_TYPE::RENDERBUFFER, rBuffImpl);
        setRenderbuffColorBuff(m_impl.get(), rBuffImpl);
    }
}

void
FramebufferObject::setColorBuffer(const aex::Texture* tBuff)
{
    const void* tBuffImpl;

    if(tBuff && (tBuffImpl = tBuff->getTexture()))
    {
        setColorBuffer(BufferBinding::BINDING_TYPE::TEXTURE, tBuffImpl);
        setTexColorBuff(m_impl.get(), tBuffImpl);
        
        if(tBuff->getTextureConfig().getTextureType()==TEX_TYPE_CUBEMAP)
        {
            mIsCube=true;
        }
    }
}

bool FramebufferObject::isAttachmentAvailable()
{
   return mCollorAttachments+1 <= getMaxColorAttachments();
}

void FramebufferObject::addColorBuffer(const Texture* tBuff)
{
    if(mCollorAttachments+1 <= getMaxColorAttachments())  
    { 
        const void* tBuffImpl;

        if(tBuff && (tBuffImpl = tBuff->getTexture()))
        {
            setColorBuffer(BufferBinding::BINDING_TYPE::TEXTURE, tBuffImpl);
            setTexColorBuff(m_impl.get(), mCollorAttachments ,tBuffImpl);

            if(tBuff->getTextureConfig().getTextureType()==TEX_TYPE_CUBEMAP)
            {
                mIsCube=true;
            }
        }
        mCollorAttachments++;
    }
}

/*s
Texture* FramebufferObject::getColorAttachment(int i)
{
    
}*/

void
FramebufferObject::setColorBuffer(const aex::Texture* tBuff, const Texture::CubeSide& side)
{
    const void* tBuffImpl;

    if(tBuff && (tBuffImpl = tBuff->getTexture()))
    {
        setColorBuffer(BufferBinding::BINDING_TYPE::TEXTURE, tBuffImpl);
        setTexColorBuff(m_impl.get(), tBuffImpl, side);
        
        if(tBuff->getTextureConfig().getTextureType()==TEX_TYPE_CUBEMAP)
        {
            mIsCube=true;
        }
    }
}

void
FramebufferObject::setDepthBuffer(const aex::RenderbufferObject* rBuff)
{
    const void* rBuffImpl;

    if(rBuff && (rBuffImpl = rBuff->getRenderbuffer()))
    {
        setDepthBuffer(BufferBinding::BINDING_TYPE::RENDERBUFFER, rBuffImpl);
        setRenderbuffDepthBuff(m_impl.get(), rBuffImpl);
    }
}

void
FramebufferObject::setDepthBuffer(const aex::Texture* tBuff)
{
    const void* tBuffImpl;

    if(tBuff && (tBuffImpl = tBuff->getTexture()))
    {
        setDepthBuffer(BufferBinding::BINDING_TYPE::TEXTURE, tBuffImpl);
        setTexDepthBuff(m_impl.get(), tBuffImpl);
        
        if(tBuff->getTextureConfig().getTextureType()==TEX_TYPE_CUBEMAP)
        {
            mIsCube=true;
        }
    }
}

void
FramebufferObject::setDepthBuffer(const aex::Texture* tBuff, const Texture::CubeSide& side)
{
    const void* tBuffImpl;

    if(tBuff && (tBuffImpl = tBuff->getTexture()))
    {
        setDepthBuffer(BufferBinding::BINDING_TYPE::TEXTURE, tBuffImpl);
        setTexDepthBuff(m_impl.get(), tBuffImpl, side);
        
        if(tBuff->getTextureConfig().getTextureType()==TEX_TYPE_CUBEMAP)
        {
            mIsCube=true;
        }
    }
}

void
FramebufferObject::setStencilBuffer(const aex::RenderbufferObject* rBuff)
{
    const void* rBuffImpl;

    if(rBuff && (rBuffImpl = rBuff->getRenderbuffer()))
    {
        setStencilBuffer(BufferBinding::BINDING_TYPE::RENDERBUFFER, rBuffImpl);
        setRenderbuffStencilBuff(m_impl.get(), rBuffImpl);
    }
}

void
FramebufferObject::setStencilBuffer(const aex::Texture* tBuff)
{
    const void* tBuffImpl;

    if(tBuff && (tBuffImpl = tBuff->getTexture()))
    {
        setStencilBuffer(BufferBinding::BINDING_TYPE::TEXTURE, tBuffImpl);
        setTexStencilBuff(m_impl.get(), tBuffImpl);
        
        if(tBuff->getTextureConfig().getTextureType()==TEX_TYPE_CUBEMAP)
        {
            mIsCube=true;
        }
    }
} 

void
FramebufferObject::setStencilBuffer(const aex::Texture* tBuff, const Texture::CubeSide& side)
{
    const void* tBuffImpl;

    if(tBuff && (tBuffImpl = tBuff->getTexture()))
    {
        setStencilBuffer(BufferBinding::BINDING_TYPE::TEXTURE, tBuffImpl);
        setTexStencilBuff(m_impl.get(), tBuffImpl, side);
        
        if(tBuff->getTextureConfig().getTextureType()==TEX_TYPE_CUBEMAP)
        {
            mIsCube=true;
        }
    }
}

void
FramebufferObject::enable()
{
    const void* t = getFramebuffer();
    const void* c = m_impl.get();

    if(t != c)
    {
        mPrevFbo = t;
        setFramebuffer(c);
    }
    
    if(mCollorAttachments>1)
    {
        setDrawBuffers(mCollorAttachments);
    }
    
    if(mIsCube&&(mLastCubeSide!=mCubeSide))
    {
            if(mColorBuffer)
            {
                setTexColorBuff(m_impl.get(), mColorBuffer.mBinding, mCubeSide);
            }
           
            if(mDepthBuffer)
            {
                setTexDepthBuff(m_impl.get(), mDepthBuffer.mBinding, mCubeSide);
            }
            
            if(mStencilBuffer)
            {
                setTexStencilBuff(m_impl.get(), mStencilBuffer.mBinding, mCubeSide);
            }
    }
}

void
FramebufferObject::disable()
{
    setFramebuffer(mPrevFbo);
}

int FramebufferObject::getCollorAttachmentsSize() const {
    return mCollorAttachments;
}

void FramebufferObject::setCubeSide(Texture::CubeSide cubeSide) {
    if(mLastCubeSide!=cubeSide)
    {
        mLastCubeSide=mCubeSide;
        this->mCubeSide = cubeSide;
    }
}

Texture::CubeSide FramebufferObject::getCubeSide() const {
    return mCubeSide;
}

bool
FramebufferObject::valid(aex::string& err)
{
    enable();
    bool ret = FramebufferStatus(err);
    disable();
    return ret;
}

}
