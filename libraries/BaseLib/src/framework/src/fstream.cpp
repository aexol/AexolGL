#include <aex/fstream.hpp>
#include "../include/internal_sdl.hpp"
#include <array>
namespace aex
{
namespace io
{
CONSTEXPR std::array<uint32_t, 3> seek_lookup
{
    {
        RW_SEEK_SET,
        RW_SEEK_CUR,
        RW_SEEK_END
    }
};
int64_t size(void* context)
{
    return SDL_RWsize(static_cast<SDL_RWops*>(context));
}
int64_t seek(void* context, int64_t offset, int whence)
{
    return SDL_RWseek(static_cast<SDL_RWops*>(context), offset, seek_lookup[whence]);
}
size_t read(void* context, void* ptr, size_t size, size_t maxnum)
{
    return SDL_RWread(static_cast<SDL_RWops*>(context), ptr, size, maxnum);
}
size_t write(void* context, const void* ptr, size_t size, size_t num)
{
    return SDL_RWwrite(static_cast<SDL_RWops*>(context), ptr, size, num);
}
int close(void* context)
{
    return SDL_RWclose(static_cast<SDL_RWops*>(context));
}
void* RWFromFile(const char* file, const char* mode)
{
    return SDL_RWFromFile(file, mode);
}
int64_t tell(void* context)
{
    return SDL_RWtell(static_cast<SDL_RWops*>(context));
}
}
}
