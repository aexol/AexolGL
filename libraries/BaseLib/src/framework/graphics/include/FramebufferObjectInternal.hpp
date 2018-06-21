#ifndef __AEX_FRAMEBUFFEROBJECTINTERNAL_HPP
#define __AEX_FRAMEBUFFEROBJECTINTERNAL_HPP
#include <aex/Texture.hpp>
#include <memory>

namespace aex
{
class RenderbufferObject;

class FramebufferObject
{
public:
    FramebufferObject();
    void setColorBuffer(const RenderbufferObject* rBuff);
    void setColorBuffer(const Texture* tBuff);
    void addColorBuffer(const Texture* tBuff);
    void setColorBuffer(const Texture* tBuff, const Texture::CubeSide& side);
    void setDepthBuffer(const RenderbufferObject* rBuff);
    void setDepthBuffer(const Texture* tBuff);
    void setDepthBuffer(const Texture* tBuff, const Texture::CubeSide& side);
    void setStencilBuffer(const RenderbufferObject* rBuff);
    void setStencilBuffer(const Texture* tBuff);
    void setStencilBuffer(const Texture* tBuff, const Texture::CubeSide& side);
    void setCubeSide(Texture::CubeSide mCubeSide);
    Texture::CubeSide getCubeSide() const;
    
    bool isAttachmentAvailable();
    bool valid(aex::string& err);
    void enable();
    void disable();
    int getCollorAttachmentsSize() const;

private:
    struct delete_frambufferobject
    {
        void operator()(void* p);
    };
    struct BufferBinding
    {
        enum class BINDING_TYPE
        {
            TEXTURE,
            RENDERBUFFER,
            NONE
        };
        BufferBinding(const BINDING_TYPE& bType, const void* binding):
            mType {bType},
            mBinding {binding}
        {}
        BufferBinding():
            BufferBinding {BINDING_TYPE::NONE, nullptr}
        {}
        BINDING_TYPE mType;
        const void* mBinding;
        
        operator bool() const 
        { 
            return mBinding != nullptr; 
        }
        
    };
    void setColorBuffer(const BufferBinding::BINDING_TYPE& bType, const void* buff, int att= 0 );
    void setDepthBuffer(const BufferBinding::BINDING_TYPE& bType, const void* buff);
    void setStencilBuffer(const BufferBinding::BINDING_TYPE& bType, const void* buff);
    
    int mCollorAttachments=0;
    const std::unique_ptr<void, delete_frambufferobject> m_impl;
    std::vector<BufferBinding> mColorBufferAttachments;
    BufferBinding mColorBuffer;
    BufferBinding mDepthBuffer;
    BufferBinding mStencilBuffer;
    bool mIsCube;
    Texture::CubeSide mLastCubeSide =Texture::CubeSide::POS_X;
    Texture::CubeSide mCubeSide =Texture::CubeSide::POS_X;
    const void* mPrevFbo;
};

int getMaxColorAttachments();
void* createFramebufferObject();
void freeFramebufferObject(void* p);
void setTexColorBuff(const void* fb, const void* tx);
void setTexColorBuff(const void* fb, const void* tx, const Texture::CubeSide& side);
void setRenderbuffColorBuff(const void* fb, const void* tx);
void setTexDepthBuff(const void* fb, const void* tx);
void setTexDepthBuff(const void* fb, const void* tx, const Texture::CubeSide& side);
void setRenderbuffDepthBuff(const void* fb, const void* tx);
void setTexStencilBuff(const void* fb, const void* tx);
void setTexStencilBuff(const void* fb, const void* tx, const Texture::CubeSide& side);
void setRenderbuffStencilBuff(const void* fb, const void* tx);
bool FramebufferStatus(aex::string& err);

void setTexColorBuff(const void* fb, int buffer_number, const void* tx);
}

#endif
