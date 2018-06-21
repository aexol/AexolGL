#include "../include/GLVertexAttribArrays.hpp"
#include "../include/GLContext.hpp"
#include <aex/ax_Macros.hpp>

namespace aex
{
    void setVertexAttribArray(uint32_t index, const AttribArrayState& state)
    {
        GLenum type = enableIfEqual(GL_FLOAT, api_detail::PrimitiveType::FLOAT, state.type);
        type |= enableIfEqual(GL_SHORT, api_detail::PrimitiveType::SHORT, state.type);
        type |= enableIfEqual(GL_UNSIGNED_SHORT, api_detail::PrimitiveType::UNSIGNED_SHORT, state.type);
        GL_CHECK(glVertexAttribPointer(index, state.size, type, false, state.stride, reinterpret_cast<void*>(state.offset)));
    }
    void enableVertexAttribArray(uint32_t index)
    {
        GL_CHECK(glEnableVertexAttribArray(index));
    }
    void disableVertexAttribArray(uint32_t index)
    {
        GL_CHECK(glDisableVertexAttribArray(index));
    }
}