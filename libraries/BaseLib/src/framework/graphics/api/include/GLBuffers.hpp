#ifndef __AEX_GLBUFFERS_HPP
#define __AEX_GLBUFFERS_HPP
#include <aex/api_definitions.hpp>
#include <aex/Common.hpp>
#include "GLContext.hpp"

namespace aex
{
    class GLESBuffer : public ContextObject
    {
    protected:
        struct Buffer
        {
            Buffer();
            uint32_t buffName;
            void release();
            ~Buffer();
        private:
            void deleteBuffer();
        };
        typedef std::shared_ptr<Buffer> RefCountedBuffer;
        const RefCountedBuffer m_backingBuffer;
    public:
        GLESBuffer();
        GLESBuffer(const GLESBuffer& gvd) = default;
        GLESBuffer(GLESBuffer&& gvd) = delete;
        GLESBuffer& operator=(const GLESBuffer& gvd) = delete;
        GLESBuffer& operator=(GLESBuffer&& gvd) = delete;
        void onContextDirty();
        uint32_t getBuffer() const;
    };
    
    class GLESVertexData : GLESBuffer
    {
    public:
        GLESVertexData();
        GLESVertexData(const GLESVertexData& gvd) = default;
        GLESVertexData(GLESVertexData&& gvd) = delete;
        GLESVertexData& operator=(const GLESVertexData& gvd) = delete;
        GLESVertexData& operator=(GLESVertexData&& gvd) = delete;
        ~GLESVertexData();
        void init(const void* data, std::size_t bitSize) const;
        void enable() const;
        void disable() const;
    };

    class GLESIndexData :   GLESBuffer
    {
    public:
        GLESIndexData(GLenum mode, std::size_t count);
        GLESIndexData(const GLESIndexData& ib) = default;
        GLESIndexData& operator=(const GLESIndexData& ib) = delete;
        ~GLESIndexData();
        void init(const void* data, std::size_t bitSize) const;
        void enable() const;
    private:
        const IndexBuffer m_indexBuffer;
    };
}
#endif
