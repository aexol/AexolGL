#ifndef __AEX_RENDERBUFFEROBJECTINTERNAL_HPP
#define __AEX_RENDERBUFFEROBJECTINTERNAL_HPP
#include <cstddef>
#include <memory>
#include <aex/api_definitions.hpp>
namespace aex
{

#define RENDERBUFFEROBJECT_TYPES_SIZE ::aex::RenderbufferObject::RGB4+5


    class RenderbufferObject
    {
    public:
        static CONSTEXPR const size_t RGB4 = 0;
        static CONSTEXPR const size_t RGB565 = RGB4 + 1;
        static CONSTEXPR const size_t RGB5A1 = RGB4 + 2;
        static CONSTEXPR const size_t DEPTH16 = RGB4 + 3;
        static CONSTEXPR const size_t DEPTH24 = RGB4 + 4;
        static CONSTEXPR const size_t DEPTH32 = RGB4 + 5;
        static CONSTEXPR const size_t STENCIL = RGB4 + 6;
        RenderbufferObject(size_t type, size_t width, size_t height);
        const void* getRenderbuffer() const;
        void init();
    private:

        struct delete_renderbufferobject
        {
            void operator()(void* p);
        };
        std::unique_ptr<void, delete_renderbufferobject> m_impl;
        const size_t m_type;
        const size_t m_width;
        const size_t m_height;
    };
    void* createRenderbufferObject(size_t type, size_t width, size_t height);
    void freeRenderbufferObject(void* p);
}

#endif
