#ifndef __AEX_GL_API_INTERNAL_HPP
#define __AEX_GL_API_INTERNAL_HPP
#include <aex/Point.hpp>
#include <aex/GraphicsDevice.hpp>
#if defined(_MSC_BUILD) && defined(__clang__)
#   include <boost/type_traits/is_default_constructible.hpp>
#   include <boost/type_traits/is_assignable.hpp>
#endif
#include <boost/signals2.hpp>
#include <functional>
#include "../../include/internal_sdl.hpp"
namespace aex
{
    int32_t getVertexDataLimit();
}
#endif
