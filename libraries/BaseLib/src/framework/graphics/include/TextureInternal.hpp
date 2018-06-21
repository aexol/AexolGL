#ifndef __AEX_TEXTUREINTERNAL_HPP
#define __AEX_TEXTUREINTERNAL_HPP
#include <aex/Texture.hpp>
#include "../../include/internal_sdl.hpp"
namespace aex
{
    class AexShaderParameterLocation;
    static CONSTEXPR uint32_t TEX_TYPE_2D = 0x0;
    static CONSTEXPR uint32_t TEX_TYPE_CUBEMAP = TEX_TYPE_2D + 1;
    static CONSTEXPR uint32_t TEX_TYPE_3D = TEX_TYPE_2D + 2;
    static CONSTEXPR uint32_t TEX_FORMAT_RGB = 0x0;
    static CONSTEXPR uint32_t TEX_FORMAT_RGBA = TEX_FORMAT_RGB + 1;
    static CONSTEXPR uint32_t TEX_FORMAT_ALPHA = TEX_FORMAT_RGB + 2;
    static CONSTEXPR uint32_t TEX_FORMAT_DEPTH_COMPONENT = TEX_FORMAT_RGB + 3;
    static CONSTEXPR uint32_t TEX_DATATYPE_8BIT_PER_CHANNEL = 0x0;
    static CONSTEXPR uint32_t TEX_DATATYPE_565 = TEX_DATATYPE_8BIT_PER_CHANNEL + 1;
    static CONSTEXPR uint32_t TEX_DATATYPE_4444 = TEX_DATATYPE_8BIT_PER_CHANNEL + 2;
    static CONSTEXPR uint32_t TEX_DATATYPE_5551 = TEX_DATATYPE_8BIT_PER_CHANNEL + 3;
    static CONSTEXPR uint32_t TEX_DATATYPE_16BIT_DEPTH = TEX_DATATYPE_8BIT_PER_CHANNEL + 4;
    static CONSTEXPR uint32_t TEX_DATATYPE_24BIT_DEPTH = TEX_DATATYPE_8BIT_PER_CHANNEL + 5;
    static CONSTEXPR uint32_t TEX_DATATYPE_32BIT_DEPTH = TEX_DATATYPE_8BIT_PER_CHANNEL + 6;
    static CONSTEXPR uint32_t TEX_FILTER_NEAREST = 0x0;
    static CONSTEXPR uint32_t TEX_FILTER_LINEAR = TEX_FILTER_NEAREST + 1;
    static CONSTEXPR uint32_t TEX_FILTER_NEAREST_MIPMAP_NEAREST = TEX_FILTER_NEAREST + 2;
    static CONSTEXPR uint32_t TEX_FILTER_LINEAR_MIPMAP_NEAREST = TEX_FILTER_NEAREST + 3;
    static CONSTEXPR uint32_t TEX_FILTER_NEAREST_MIPMAP_LINEAR = TEX_FILTER_NEAREST + 4;
    static CONSTEXPR uint32_t TEX_FILTER_LINEAR_MIPMAP_LINEAR = TEX_FILTER_NEAREST + 5;
    static CONSTEXPR uint32_t TEX_WRAP_CLAMP_TO_EDGE = 0x0;
    static CONSTEXPR uint32_t TEX_WRAP_REPEAT = TEX_WRAP_CLAMP_TO_EDGE + 1;
    static CONSTEXPR uint32_t TEX_WRAP_MIRRORED_REPEAT = TEX_WRAP_CLAMP_TO_EDGE + 2;
    
    struct CubeMapping
    {
        const void* data;
        Texture::CubeSide type;
    };
    
    struct SDLCubeMapping
    {
        const SDL_Surface* data;
        Texture::CubeSide type;
    };
    class UniformSampler;
    
    class HWTexture
    {
    private:
        struct delete_hwtex
        {
            void operator()(void* p) const;
        };
    public:
        typedef std::unique_ptr<void, delete_hwtex> impl_ptr;
        explicit HWTexture(const TextureConfig& cfg);
        int  getBinder();
        void setImpl(impl_ptr&& ni);
        void setTexture2D(void* data, uint32_t w, uint32_t h, uint32_t level = 0);
        void setTexture3D(void* data, uint32_t w, uint32_t h, uint32_t d, uint32_t level = 0);
        void setTextureCubeSide(const CubeMapping& data, uint32_t w, uint32_t h, uint32_t level = 0);
        void setTextureCube(const std::array<CubeMapping, 6>& data, uint32_t w, uint32_t h, uint32_t level = 0);
        void setTexture2D(SDL_Surface* data, uint32_t level = 0);
        void setTextureCubeSide(const SDLCubeMapping& data, uint32_t level = 0);
        void setTextureCube(const std::array<SDLCubeMapping, 6>& data, uint32_t level = 0);
        void setCubeType();
        bool enable() const;
        void disable() const;
        void refreshTexParams(uint32_t w, uint32_t h);
        const void* impl() const;
    private:
        void iSetTextureCubeSide(const SDLCubeMapping& data, uint32_t level = 0);
        void iSetTextureCubeSide(const CubeMapping& data, uint32_t w, uint32_t h, uint32_t level = 0);
        impl_ptr m_impl;
        TextureConfig mCfg;
    };
}

#endif
