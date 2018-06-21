#ifndef __AEX_GLFRAMEBUFFER_HPP
#define __AEX_GLFRAMEBUFFER_HPP
#include "GLContext.hpp"

namespace aex
{
    struct GLFramebuffer : public ContextObject
    {
        GLuint id = -1;
        GLuint cubeSide = 0;
        bool isCube=false;
        void onContextDirty();
    };
}

#endif
