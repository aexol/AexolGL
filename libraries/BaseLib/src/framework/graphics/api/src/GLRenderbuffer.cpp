#include "../include/GLRenderbuffer.hpp"
#include "../../include/RenderbufferObjectInternal.hpp"
#include <aex/Common.hpp>
#include <aex/api_definitions.hpp>
#include <aex/utils.hpp>
#include <aex/ax_Macros.hpp>
#include <array>

namespace aex
{

    static GLenum getRenderbufferType(int type)
    {

        GLenum ret = GL_NONE;

        switch(type)
        {
        case RenderbufferObject::RGB4:
        {
            ret = GL_RGBA4;
        }
        break;

        case RenderbufferObject::RGB565:
        {
#ifdef GL_RGB565
            ret = GL_RGB565;
#else
            ret = GL_RGB5_A1;
#endif
        }
        break;

        case RenderbufferObject::RGB5A1:
        {
            ret = GL_RGB5_A1;
        }
        break;

        case RenderbufferObject::DEPTH16:
        {
            ret = GL_DEPTH_COMPONENT16;
        }
        break;

        case RenderbufferObject::DEPTH24:
        {
#ifdef GL_DEPTH_COMPONENT24
            ret = GL_DEPTH_COMPONENT24;
#elif GL_DEPTH_COMPONENT16
            ret = GL_DEPTH_COMPONENT16;
#endif
        }
        break;

        case RenderbufferObject::DEPTH32:
        {
#ifdef GL_DEPTH_COMPONENT32
            ret = GL_DEPTH_COMPONENT32;
#elif GL_DEPTH_COMPONENT24
            ret = GL_DEPTH_COMPONENT24;
#elif GL_DEPTH_COMPONENT16
            ret = GL_DEPTH_COMPONENT16;
#endif
        }
        break;

        case RenderbufferObject::STENCIL:
        {
            ret = GL_STENCIL_INDEX8;
        }
        break;

        default:
            break;
        }

        return ret;
    }

    void* createRenderbufferObject(size_t type, size_t width, size_t height)
    {
        return new GLRenderbuffer(type, width, height);
    }

    void freeRenderbufferObject(void* p)
    {
        delete static_cast<GLRenderbuffer*>(p);
    }

    GLRenderbuffer::GLRenderbuffer(size_t type, size_t width, size_t height)
    {
        GL_CHECK(glGenRenderbuffers(1, &mRenderbuffer));
        GL_CHECK(glRenderbufferStorage(mRenderbuffer, getRenderbufferType(type), width, height));
    }

    GLRenderbuffer::GLRenderbuffer(GLRenderbuffer&& glRb) :
        mRenderbuffer(glRb.mRenderbuffer)
    {
        glRb.mRenderbuffer = 0;
    }

    GLRenderbuffer&
    GLRenderbuffer::operator=(GLRenderbuffer&& glRb)
    {
        if(this != &glRb)
        {
            std::swap(mRenderbuffer, glRb.mRenderbuffer);
        }

        return *this;
    }

    GLRenderbuffer::~GLRenderbuffer()
    {
        if(mRenderbuffer)
        {
            GL_CHECK(glDeleteRenderbuffers(1, &mRenderbuffer));
        }
    }
}