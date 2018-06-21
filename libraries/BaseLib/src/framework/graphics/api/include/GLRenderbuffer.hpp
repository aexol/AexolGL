#ifndef __AEX_GLRENDERBUFFER_HPP
#define __AEX_GLRENDERBUFFER_HPP
#include <cstddef>
#include <aex/api_definitions.hpp>


namespace aex
{
    class GLRenderbuffer
    {
    public:
        GLRenderbuffer(size_t type, size_t width, size_t height);
        GLRenderbuffer(const GLRenderbuffer&) = delete;
        GLRenderbuffer& operator=(const GLRenderbuffer&) = delete;
        GLRenderbuffer(GLRenderbuffer&&);
        GLRenderbuffer& operator=(GLRenderbuffer&&);
        ~GLRenderbuffer();
    private:
        GLuint mRenderbuffer;
        friend void setRenderbuff(GLenum attachment, const void* tx);
    };
}

#endif
