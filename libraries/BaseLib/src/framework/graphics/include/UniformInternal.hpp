#ifndef __AEX_UNIFORMINTERNAL_HPP
#define __AEX_UNIFORMINTERNAL_HPP
#include <aex/Vector2.hpp>
#include <aex/Vector3.hpp>
#include <array>
namespace aex
{
    class HWTexture;
    /* primitives */
    typedef const HWTexture* sampler;
    typedef aex::math::Vector2 float2;
    typedef aex::math::Vector3 float3;
    typedef std::array<float, 4> float4;

    typedef std::array<int, 2> int2;
    typedef std::array<int, 3> int3;
    typedef std::array<int, 4> int4;

    /* vec float types */
    typedef std::array<float, 1> vec1;
    typedef std::array<float, 2> vec2;
    typedef std::array<float, 3> vec3;
    typedef std::array<float, 4> vec4;

    /* vec int types */
    typedef std::array<int, 1> veci1;
    typedef std::array<int, 2> veci2;
    typedef std::array<int, 3> veci3;
    typedef std::array<int, 4> veci4;

    /* matrix types */
    template<class T, size_t SIZE> struct umatrix : public std::array<T, SIZE>
    {
        umatrix() : std::array<T, SIZE>(){}
        explicit umatrix(const std::array<T, SIZE>& m) : std::array<T, SIZE>(m){}
    };
    typedef umatrix<float, 4> mat2;
    typedef umatrix<float, 9> mat3;
    typedef umatrix<float, 16> mat4;
    struct UniformLib
    {
        static void setBoolShaderParam(void* p, bool value);
        static void setFloatShaderParam(void* p, float value);
        static void setFloat2ShaderParam(void* p, float2 value);
        static void setFloat3ShaderParam(void* p, float3 value);
        static void setFloat4ShaderParam(void* p, const float4& value);
        static void setSamplerShaderParam(void* p, sampler value);
        static void setIntShaderParam(void* p, int value);
        static void setInt2ShaderParam(void* p, int2 value);
        static void setInt3ShaderParam(void* p, int3 value);
        static void setInt4ShaderParam(void* p, int4 value);
        static void setVec1fShaderParam(void* p, const std::vector<vec1>& value);
        static void setVec2fShaderParam(void* p, const std::vector<vec2>& value);
        static void setVec3fShaderParam(void* p, const std::vector<vec3>& value);
        static void setVec4fShaderParam(void* p, const std::vector<vec4>& value);
        static void setMat2fShaderParam(void* p, const std::vector<mat2>& value);
        static void setMat3fShaderParam(void* p, const std::vector<mat3>& value);
        static void setMat4fShaderParam(void* p, const std::vector<mat4>& value);
        static void setVec1iShaderParam(void* p, const std::vector<veci1>& value);
        static void setVec2iShaderParam(void* p, const std::vector<veci2>& value);
        static void setVec3iShaderParam(void* p, const std::vector<veci3>& value);
        static void setVec4iShaderParam(void* p, const std::vector<veci4>& value);
    };
}

#endif
