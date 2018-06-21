#include "../include/GLBuffers.hpp"
#include "../include/GLContext.hpp"
#include "../../include/VertexDataInternal.hpp"
#include <aex/utils.hpp>
#include <aex/ax_Macros.hpp>
#include <aex/api_definitions.hpp>
#include <aex/VertexData.hpp>

namespace aex
{
CONSTEXPR std::size_t IndexData::TRIANGLES;
CONSTEXPR std::size_t IndexData::TRIANGLE_STIRP;
CONSTEXPR std::size_t IndexData::LINES;
CONSTEXPR std::size_t IndexData::LINE_STRIP;

CONSTEXPR GLenum evalMode(const std::size_t mode)
{
    return
        enableIfEqual(GL_TRIANGLES, IndexData::TRIANGLES, mode) |
        enableIfEqual(GL_TRIANGLE_STRIP, IndexData::TRIANGLE_STIRP, mode) |
        enableIfEqual(GL_LINES, IndexData::LINES, mode) |
        enableIfEqual(GL_LINE_STRIP, IndexData::LINE_STRIP, mode);
}
GLESBuffer::Buffer::Buffer() : buffName(0) {}
GLESBuffer::Buffer::~Buffer()
{
    deleteBuffer();
}
GLESBuffer::GLESBuffer() :
    m_backingBuffer {new Buffer} {}
    
void
GLESBuffer::Buffer::deleteBuffer()
{
    if(buffName != 0)
    {
        GL_CHECK(glDeleteBuffers(1, &buffName));
        buffName = 0;
    }
}

void
GLESBuffer::Buffer::release()
{
    deleteBuffer();
}

uint32_t
GLESBuffer::getBuffer() const
{
    if(m_backingBuffer->buffName == 0)
    {
        GL_CHECK(glGenBuffers(1, &m_backingBuffer->buffName));
    }

    return m_backingBuffer->buffName;
}

void
GLESBuffer::onContextDirty()
{
    if(m_backingBuffer)
        m_backingBuffer->buffName = 0;
}

/******************VERTEX_DATA*******************/
GLESVertexData::GLESVertexData() : GLESBuffer {} {}
GLESVertexData::~GLESVertexData()
{
    if(m_backingBuffer && m_backingBuffer->buffName)
    {
        bufferDeletedCTX(this);
    }
}

void
GLESVertexData::enable() const
{
    uint32_t buff = getBuffer();

    if(setArrayBuffer(this))
    {
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, buff));
    }
}

void
GLESVertexData::disable() const
{
    unsetArrayBuffer(this);
}

void
GLESVertexData::init(const void* data, std::size_t bitSize) const
{
    enable();
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, bitSize, data, GL_STATIC_DRAW));
    unsetArrayBuffer(this);
}

/******************INDEX_DATA********************/
GLESIndexData::GLESIndexData(GLenum mode, std::size_t count):
    GLESBuffer {},
    m_indexBuffer {count, mode}
{}
    
GLESIndexData::~GLESIndexData()
{
    if(m_backingBuffer && m_backingBuffer->buffName)
    {
        bufferDeletedCTX(this);
    }
}

void
GLESIndexData::init(const void* data, std::size_t bitSize) const
{
    enable();
    GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, bitSize, data, GL_STATIC_DRAW));
    unsetIndexBuffer();
}

void
GLESIndexData::enable() const
{
    uint32_t buff = getBuffer();

    if(setIndexBuffer(this, m_indexBuffer))
    {
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buff));
    }
}
/****************INTERFACE*********************/

void* makeInternalVertexData()
{
    return new GLESVertexData;
}

void freeInternalVertexData(void* p)
{
    delete static_cast<GLESVertexData*>(p);
}

void* copyInternalVertexData(void* p)
{
    return p ? new GLESVertexData {*static_cast<const GLESVertexData*>(p)} : nullptr;
}

void initInternalVertexData(void* p, const void* data, const std::size_t bitSize)
{
    static_cast<GLESVertexData*>(p)->init(data, bitSize);
}

void enableInternalVertexData(void* p)
{
    static_cast<GLESVertexData*>(p)->enable();
}

void disableInternalVertexData(void* p)
{
    static_cast<GLESVertexData*>(p)->disable();
}

void* makeInternalIndexData(std::size_t mode, std::size_t element_count)
{
    return new GLESIndexData {evalMode(mode), element_count};
}

void freeInternalIndexData(void* p)
{
    delete static_cast<GLESIndexData*>(p);
}

void* copyInternalIndexData(void* p)
{
    return p ? new GLESIndexData {*static_cast<const GLESIndexData*>(p)} : nullptr;
}

void initInternalIndexData(void* p, const void* data, const std::size_t bitSize)
{
    static_cast<GLESIndexData*>(p)->init(data, bitSize);
}

void enableInternalIndexData(void* p)
{
    static_cast<GLESIndexData*>(p)->enable();
}

}
