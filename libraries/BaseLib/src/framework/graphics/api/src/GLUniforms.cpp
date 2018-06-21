#include "../include/GLUniforms.hpp"
#include "../include/GLContext.hpp"
#include "../include/GLTexture.hpp"
#include "../../include/TextureInternal.hpp"
#include "../../include/UniformInternal.hpp"
#include "../../include/ShaderProgramInternal.hpp"
#include <aex/Common.hpp>
#include <aex/api_definitions.hpp>
#include <aex/ax_Macros.hpp>
#include <aex/ShaderProgram.hpp>
namespace aex
{
/* forward declarations */
template<class> class TUniformInfo;
template<class T>
bool operator==(const TUniformInfo<T>& lhs, const T& rhs);

UniformInfo::UniformInfo() :
    location(-1),
    program(0),
    type(UNSET),
    dirty(true)
{
}

UniformInfo::~UniformInfo()
{
}

UniformInfo::UniformInfo(uint32_t progId, const aex::string& name, Type _type)
{
    GL_CHECK(location = glGetUniformLocation(progId, name.c_str()));
    type = _type;
    program = progId;
}

UniformInfo::UniformInfo(uint32_t progId, uint32_t loc, Type _type)
{
    location = loc;
    type = _type;
    program = progId;
}

void
UniformInfo::set()
{
    if(dirty)
        setOnDirty();
}

UniformInfo::Type
UniformInfo::getType() const
{
    return type;
}

UnsetUniformInfo::UnsetUniformInfo(uint32_t progId, const aex::string& name) : UniformInfo(progId, name){}

void
UnsetUniformInfo::setOnDirty()
{}

static void setUniformInState(uint32_t location, bool value);
static void setUniformInState(uint32_t location, float value);
static void setUniformInState(uint32_t location, float2 value);
static void setUniformInState(uint32_t location, float3 value);
static void setUniformInState(uint32_t location, float4 value);
static void setUniformInState(uint32_t location, int value);
static void setUniformInState(uint32_t location, const int2& value);
static void setUniformInState(uint32_t location, const int3& value);
static void setUniformInState(uint32_t location, const int4& value);
static void setUniformInState(uint32_t location, const std::vector<vec1>& value);
static void setUniformInState(uint32_t location, const std::vector<vec2>& value);
static void setUniformInState(uint32_t location, const std::vector<vec3>& value);
static void setUniformInState(uint32_t location, const std::vector<vec4>& value);
static void setUniformInState(uint32_t location, const std::vector<veci1>& value);
static void setUniformInState(uint32_t location, const std::vector<veci2>& value);
static void setUniformInState(uint32_t location, const std::vector<veci3>& value);
static void setUniformInState(uint32_t location, const std::vector<veci4>& value);
static void setUniformInState(uint32_t location, const std::vector<mat2>& value);
static void setUniformInState(uint32_t location, const std::vector<mat3>& value);
static void setUniformInState(uint32_t location, const std::vector<mat4>& value);

template<class T>
struct valueToUniformType
{
    static CONSTEXPR UniformInfo::Type u_type = UniformInfo::UNSET;
};
    
template<>
struct valueToUniformType<bool>
{
    static CONSTEXPR UniformInfo::Type u_type = UniformInfo::BOOL;
};
    
template<>
struct valueToUniformType<float>
{
    static CONSTEXPR UniformInfo::Type u_type = UniformInfo::FLOAT;
};
    
template<>
struct valueToUniformType<float2>
{
    static CONSTEXPR UniformInfo::Type u_type = UniformInfo::FLOAT2;
};
    
template<>
struct valueToUniformType<float3>
{
    static CONSTEXPR UniformInfo::Type u_type = UniformInfo::FLOAT3;
};
    
template<>
struct valueToUniformType<float4>
{
    static CONSTEXPR UniformInfo::Type u_type = UniformInfo::FLOAT4;
};
    
template<>
struct valueToUniformType<int>
{
    static CONSTEXPR UniformInfo::Type u_type = UniformInfo::INT;
};
    
template<>
struct valueToUniformType<int2>
{
    static CONSTEXPR UniformInfo::Type u_type = UniformInfo::INT2;
};
    
template<>
struct valueToUniformType<int3>
{
    static CONSTEXPR UniformInfo::Type u_type = UniformInfo::INT3;
};
    
template<>
struct valueToUniformType<int4>
{
    static CONSTEXPR UniformInfo::Type u_type = UniformInfo::INT4;
};
    
template<>
struct valueToUniformType<std::vector<vec1>>
{
    static CONSTEXPR UniformInfo::Type u_type = UniformInfo::VEC1F;
};
template<>
struct valueToUniformType<std::vector<vec2>>
{
    static CONSTEXPR UniformInfo::Type u_type = UniformInfo::VEC2F;
};
    
template<>
struct valueToUniformType<std::vector<vec3>>
{
    static CONSTEXPR UniformInfo::Type u_type = UniformInfo::VEC3F;
};
    
template<>
struct valueToUniformType<std::vector<vec4>>
{
    static CONSTEXPR UniformInfo::Type u_type = UniformInfo::VEC4F;
};
    
template<>
struct valueToUniformType<std::vector<veci1>>
{
    static CONSTEXPR UniformInfo::Type u_type = UniformInfo::VEC1I;
};
    
template<>
struct valueToUniformType<std::vector<veci2>>
{
    static CONSTEXPR UniformInfo::Type u_type = UniformInfo::VEC2I;
};
    
template<>
struct valueToUniformType<std::vector<veci3>>
{
    static CONSTEXPR UniformInfo::Type u_type = UniformInfo::VEC3I;
};
    
template<>
struct valueToUniformType<std::vector<veci4>>
{
    static CONSTEXPR UniformInfo::Type u_type = UniformInfo::VEC4I;
};
    
template<>
struct valueToUniformType<std::vector<mat2>>
{
    static CONSTEXPR UniformInfo::Type u_type = UniformInfo::MAT2F;
};
    
template<>
struct valueToUniformType<std::vector<mat3>>
{
    static CONSTEXPR UniformInfo::Type u_type = UniformInfo::MAT3F;
};
    
template<>
struct valueToUniformType<std::vector<mat4>>
{
    static CONSTEXPR UniformInfo::Type u_type = UniformInfo::MAT4F;
};

template<class T>
class TUniformInfo : public UniformInfo
{
    public:
        typedef typename std::remove_reference<typename std::remove_cv<T>::type>::type value_type;
        explicit TUniformInfo(const UniformInfo& ui) : UniformInfo(ui)
        {
            type = valueToUniformType<value_type>::u_type;
        }
        TUniformInfo(const T& value, uint32_t progId, const aex::string& name) : UniformInfo(progId, name, valueToUniformType<value_type>::u_type), value(value){}
        TUniformInfo& operator=(const value_type& new_value)
        {
            if(value == new_value)
                return *this;

            value = new_value;

            try
            {
                aex_context& ctx = getCurrentContext();
                if(program != ctx.ShaderProgramState)
                    dirty = true;
                else
                {
                    _setImpl();
                }
            } catch (WindowHidden& e)
            {
                dirty = true;
            }
            return *this;
        }
    protected:
        void setOnDirty()
        {
            _setImpl();
        }
        void _setImpl()
        {
            setUniformInState(location, value);
            dirty = false;
        }
    private:
        value_type value;
    friend bool operator==<>(const TUniformInfo<value_type>& lhs, const value_type& rhs);
};
template<class T>
bool operator==(const TUniformInfo<T>& lhs, const T& rhs)
{
    return lhs.value == rhs;
}

using BoolUniformInfo = TUniformInfo<bool>;
using FloatUniformInfo = TUniformInfo<float>;
using Float2UniformInfo = TUniformInfo<float2>;
using Float3UniformInfo = TUniformInfo<float3>;
using Float4UniformInfo = TUniformInfo<float4>;
using IntUniformInfo = TUniformInfo<int>;
using Int2UniformInfo = TUniformInfo<int2>;
using Int3UniformInfo = TUniformInfo<int3>;
using Int4UniformInfo = TUniformInfo<int4>;
using VecFloat1UniformInfo = TUniformInfo<std::vector<vec1>>;
using VecFloat2UniformInfo = TUniformInfo<std::vector<vec2>>;
using VecFloat3UniformInfo = TUniformInfo<std::vector<vec3>>;
using VecFloat4UniformInfo = TUniformInfo<std::vector<vec4>>;
using VecInt1UniformInfo = TUniformInfo<std::vector<veci1>>;
using VecInt2UniformInfo = TUniformInfo<std::vector<veci2>>;
using VecInt3UniformInfo = TUniformInfo<std::vector<veci3>>;
using VecInt4UniformInfo = TUniformInfo<std::vector<veci4>>;
using MatFloat2UniformInfo = TUniformInfo<std::vector<mat2>>;
using MatFloat3UniformInfo = TUniformInfo<std::vector<mat3>>;
using MatFloat4UniformInfo = TUniformInfo<std::vector<mat4>>;

template<class ParamType, UniformInfo::Type __GLSL_TYPE>
static ParamType* castParam(void *p)
{
    std::unique_ptr<UniformInfo>* _pinfo = static_cast<std::unique_ptr<UniformInfo>*>(p);
    const UniformInfo::Type _ptype = (*_pinfo)->getType();
    if(_ptype != __GLSL_TYPE)
    {
        if(_ptype == UniformInfo::UNSET)
        {
            UnsetUniformInfo& _unset_ui = static_cast<UnsetUniformInfo&>(**_pinfo);
            ParamType* const info = new ParamType{_unset_ui};
            _pinfo->reset(info);
        } else {
            //TODO: Exception
        }
    }
    return static_cast<ParamType*>(_pinfo->get());
}

template<class ParamType, class ValueType>
void setShaderParam(ParamType* p, ValueType&& value)
{
    //ParamType* info = castParam<ParamType>(p);
    *p = aex::forward<ValueType>(value);
}

void freeUniformInfor(UniformInfo* p)
{
    delete p;
}

void
UniformLib::setBoolShaderParam(void* p, bool value)
{
    setShaderParam(castParam<BoolUniformInfo, UniformInfo::BOOL>(p), value);
}

void
UniformLib::setFloatShaderParam(void* p, float value)
{
    setShaderParam(castParam<FloatUniformInfo, UniformInfo::FLOAT>(p), value);
}

void
UniformLib::setFloat2ShaderParam(void* p, float2 value)
{
    setShaderParam(castParam<Float2UniformInfo, UniformInfo::FLOAT2>(p), value);
}

void
UniformLib::setFloat3ShaderParam(void* p, float3 value)
{
    setShaderParam(castParam<Float3UniformInfo, UniformInfo::FLOAT3>(p), value);
}

void
UniformLib::setFloat4ShaderParam(void* p, const float4& value)
{
    setShaderParam(castParam<Float4UniformInfo, UniformInfo::FLOAT4>(p), value);
}

void
UniformLib::setSamplerShaderParam(void* p, sampler value)
{
    if(value->enable())
    {
        const GLTexture* tt = static_cast<const GLTexture*>(value->impl());
        setShaderParam(castParam<IntUniformInfo, UniformInfo::INT>(p), tt->mTURef);
    }
}

void
UniformLib::setIntShaderParam(void* p, int value)
{
    setShaderParam(castParam<IntUniformInfo, UniformInfo::INT>(p), value);
}

void
UniformLib::setInt2ShaderParam(void* p, int2 value)
{
    setShaderParam(castParam<Int2UniformInfo, UniformInfo::INT2>(p), value);
}

void
UniformLib::setInt3ShaderParam(void* p, int3 value)
{
    setShaderParam(castParam<Int3UniformInfo, UniformInfo::INT3>(p), value);
}

void
UniformLib::setInt4ShaderParam(void* p, int4 value)
{
    setShaderParam(castParam<Int4UniformInfo, UniformInfo::INT4>(p), value);
}

/********** Arrays of vectors and matrices **************/
void
UniformLib::setVec1fShaderParam(void* p, const std::vector<vec1>& value)
{
    setShaderParam(castParam<VecFloat1UniformInfo, UniformInfo::VEC1F>(p), value);
}

void
UniformLib::setVec2fShaderParam(void* p, const std::vector<vec2>& value)
{
    setShaderParam(castParam<VecFloat2UniformInfo, UniformInfo::VEC2F>(p), value);
}

void
UniformLib::setVec3fShaderParam(void* p, const std::vector<vec3>& value)
{
    setShaderParam(castParam<VecFloat3UniformInfo, UniformInfo::VEC3F>(p), value);
}

void
UniformLib::setVec4fShaderParam(void* p, const std::vector<vec4>& value)
{
    setShaderParam(castParam<VecFloat4UniformInfo, UniformInfo::VEC4F>(p), value);
}

void
UniformLib::setMat2fShaderParam(void* p, const std::vector<mat2>& value)
{
    setShaderParam(castParam<MatFloat2UniformInfo, UniformInfo::MAT2F>(p), value);
}

void 
UniformLib::setMat3fShaderParam(void* p, const std::vector<mat3>& value)
{
    setShaderParam(castParam<MatFloat3UniformInfo, UniformInfo::MAT3F>(p), value);
}

void
UniformLib::setMat4fShaderParam(void* p, const std::vector<mat4>& value)
{
    setShaderParam(castParam<MatFloat4UniformInfo, UniformInfo::MAT4F>(p), value);
}

void
UniformLib::setVec1iShaderParam(void* p, const std::vector<veci1>& value)
{
    setShaderParam(castParam<VecInt1UniformInfo, UniformInfo::VEC1I>(p), value);
}

void
UniformLib::setVec2iShaderParam(void* p, const std::vector<veci2>& value)
{
    setShaderParam(castParam<VecInt2UniformInfo, UniformInfo::VEC2I>(p), value);
}

void
UniformLib::setVec3iShaderParam(void* p, const std::vector<veci3>& value)
{
    setShaderParam(castParam<VecInt3UniformInfo, UniformInfo::VEC3I>(p), value);
}

void
UniformLib::setVec4iShaderParam(void* p, const std::vector<veci4>& value)
{
    setShaderParam(castParam<VecInt4UniformInfo, UniformInfo::VEC4I>(p), value);
}

void freeParamInternal(void* p)
{
    delete static_cast<aex::weak_ptr<UniformInfo>*>(p);
}

bool testParameter(void* p)
{
    aex::weak_ptr<UniformInfo>& uInfo = *static_cast<aex::weak_ptr<UniformInfo>*>(p);

    if(auto spt = uInfo.lock())
    {
        return true;
    }

    return false;
}

void setUniformInState(uint32_t location, bool value)
{
    GL_CHECK(glUniform1i(location, int(value)));
}

void setUniformInState(uint32_t location, float value)
{
    GL_CHECK(glUniform1f(location, value));
}

void setUniformInState(uint32_t location, float2 value)
{
    GL_CHECK(glUniform2f(location, value[0], value[1]));
}

void setUniformInState(uint32_t location, float3 value)
{
    GL_CHECK(glUniform3f(location, value[0], value[1], value[2]));
}

void setUniformInState(uint32_t location, float4 value)
{
    GL_CHECK(glUniform4f(location, value[0], value[1], value[2], value[3]));
}

void setUniformInState(uint32_t location, int value)
{
    GL_CHECK(glUniform1i(location, value));
}

void setUniformInState(uint32_t location, const int2& value)
{
    GL_CHECK(glUniform2i(location, value[0], value[1]));
}

void setUniformInState(uint32_t location, const int3& value)
{
    GL_CHECK(glUniform3i(location, value[0], value[1], value[2]));
}

void setUniformInState(uint32_t location, const int4& value)
{
    GL_CHECK(glUniform4i(location, value[0], value[1], value[2], value[3]));
}

void setUniformInState(uint32_t location, const std::vector<vec1>& value)
{
    GL_CHECK(glUniform1fv(location, value.size(), &value[0][0]));
}

void setUniformInState(uint32_t location, const std::vector<vec2>& value)
{
    GL_CHECK(glUniform2fv(location, value.size(), &value[0][0]));
}

void setUniformInState(uint32_t location, const std::vector<vec3>& value)
{
    GL_CHECK(glUniform3fv(location, value.size(), &value[0][0]));
}

void setUniformInState(uint32_t location, const std::vector<vec4>& value)
{
    GL_CHECK(glUniform4fv(location, value.size(), &value[0][0]));
}

void setUniformInState(uint32_t location, const std::vector<veci1>& value)
{
    GL_CHECK(glUniform1iv(location, value.size(), &value[0][0]));
}

void setUniformInState(uint32_t location, const std::vector<veci2>& value)
{
    GL_CHECK(glUniform2iv(location, value.size(), &value[0][0]));
}

void setUniformInState(uint32_t location, const std::vector<veci3>& value)
{
    GL_CHECK(glUniform3iv(location, value.size(), &value[0][0]));
}

void setUniformInState(uint32_t location, const std::vector<veci4>& value)
{
    GL_CHECK(glUniform4iv(location, value.size(), &value[0][0]));
}

void setUniformInState(uint32_t location, const std::vector<mat2>& value)
{
    GL_CHECK(glUniformMatrix2fv(location, value.size(), false, &value[0][0]));
}

void setUniformInState(uint32_t location, const std::vector<mat3>& value)
{
    GL_CHECK(glUniformMatrix3fv(location, value.size(), false, &value[0][0]));
}

void setUniformInState(uint32_t location, const std::vector<mat4>& value)
{
    GL_CHECK(glUniformMatrix4fv(location, value.size(), false, &value[0][0]));
}

}
