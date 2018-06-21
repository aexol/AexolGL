#ifndef __AEX_GLVERTEXATTRIBARRAYS_HPP
#define __AEX_GLVERTEXATTRIBARRAYS_HPP
#include <aex/VertexData.hpp>
namespace aex
{
    struct aex_context;
    class GLESBuffer;
    struct AttribArrayState
    {
        std::size_t size;
        api_detail::PrimitiveType type;
        std::size_t stride;
        std::size_t offset;
        const GLESBuffer* buffer;
        bool normalized;
        bool enabled;
        bool markedForDisable;
    };
    void setVertexAttribArray(uint32_t index, const AttribArrayState& state);
    void enableVertexAttribArray(uint32_t index);
    void disableVertexAttribArray(uint32_t index);

}

#endif
