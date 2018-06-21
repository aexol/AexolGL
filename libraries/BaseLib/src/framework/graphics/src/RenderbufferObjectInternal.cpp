#include "../include/RenderbufferObjectInternal.hpp"
#include <stdexcept>

namespace aex
{
void
RenderbufferObject::delete_renderbufferobject::operator()(void* p)
{
    freeRenderbufferObject(p);
}
RenderbufferObject::RenderbufferObject(size_t type, size_t width, size_t height):
    m_impl {nullptr, delete_renderbufferobject{}},
    m_type {type},
    m_width {width},
    m_height {height}
{
    if(type >= RENDERBUFFEROBJECT_TYPES_SIZE)
        throw std::runtime_error{"Invalid renderbuffer object type."};
}
const void*
RenderbufferObject::getRenderbuffer() const
{
    return m_impl.get();
}
void
RenderbufferObject::init()
{
    if(!m_impl)
    {
        m_impl.reset(createRenderbufferObject(m_type, m_width, m_height));
    }
}
}
