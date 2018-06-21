#ifndef __ISHADER_HPP
#define __ISHADER_HPP

namespace aex
{
    class AEX_PUBLIC_API IShader
    {
    public:
        virtual void compileSource() = 0
        virtual void draw() = 0;
        virtual ~IShader() = 0;
    };
    inline IShader::~IShader() {}
}

#endif
