#ifndef __AEX_VERTEXDATAINTERNAL_HPP
#define __AEX_VERTEXDATAINTERNAL_HPP
#include <cstddef>
namespace aex
{
    void* makeInternalVertexData();
    void freeInternalVertexData(void* p);
    void* copyInternalVertexData(void* p);
    void initInternalVertexData(void* p, const void* data, const std::size_t bitSize);
    void enableInternalVertexData(void* p);
    void disableInternalVertexData(void* p);
    void* makeInternalIndexData(std::size_t mode, std::size_t element_count);
    void freeInternalIndexData(void* p);
    void* copyInternalIndexData(void* p);
    void initInternalIndexData(void* p, const void* data, const std::size_t bitSize);
    void enableInternalIndexData(void* p);
}

#endif
