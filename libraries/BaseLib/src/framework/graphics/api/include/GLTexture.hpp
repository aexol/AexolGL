#ifndef __AEX_GLTEXTURE_HPP
#define __AEX_GLTEXTURE_HPP
#include "GLContext.hpp"
#include <aex/api_definitions.hpp>
#include <aex/Texture.hpp>
#include <map>
#include <array>
#include <vector>
#include <queue>
namespace aex
{
    struct aex_context;
    struct GLTexture : public ContextObject
    {
        GLTexture() = default;
        GLTexture(GLuint tex, GLuint target, size_t ref);
        GLuint mTexture;
        GLenum mTarget;
        size_t mTURef;
        void onContextDirty();
    };
    GLenum textureCubeSideToGLEnum(const Texture::CubeSide& side);
    bool isTextureActive(aex_context& ctx, GLTexture* t, std::vector<GLTextureUnit>& TUs);
    void setTextureInUnit(aex_context& ctx, GLTexture* t, size_t unit, bool newBinding);
    void setActiveTex(GLTexture* t, std::queue<size_t>& ring, std::vector<GLTextureUnit>& TUs);
    void pickTexBuffers(aex_context& ctx, GLTexture* t, std::vector<GLTextureUnit>** TU, std::queue<size_t>** AvailTUs);
}

#endif
