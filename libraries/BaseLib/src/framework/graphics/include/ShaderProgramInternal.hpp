#ifndef __AEX_SHADERPROGRAMINTERNAL_HPP
#define __AEX_SHADERPROGRAMINTERNAL_HPP
#include <aex/Common.hpp>
#include <aex/Vector2.hpp>
#include <aex/Vector3.hpp>
#include <array>
#include <functional>
#include <memory>
#include <list>
#include <utility>
namespace aex
{
    namespace api_detail
    {
        struct DataFormatCommon;
        typedef std::pair<aex::string, DataFormatCommon> AttribType;
        typedef std::list<AttribType> AttribList;
    }
    class AexShaderParameterLocation;
    class VertexData;
    class ShaderParameterMapping
    {
    private:
        typedef std::function<void(void*)> DeleterType;
    public:
        operator bool() const;
        typedef std::unique_ptr<void, DeleterType> Impl;
    private:
        ShaderParameterMapping();
        bool m_standAlone;
        Impl m_impl;
        friend class ShaderProgram;
        friend struct UniformLib;
    };

    void buildProgramObject(const AexShader& program, const std::unique_ptr<ShaderObject>& v, const std::unique_ptr<ShaderObject>& f);
    void freeParamInternal(void* p);
    bool testParameter(void* p);
    void* makeInternalShader();
    void freeInternalShader(void* s);
    void internalShaderDraw(void* shader);
    void* findParameter(const aex::string& name, void* shader);
    void enableProgram(void* s);
    void* makeVertexShader(const aex::string& vsource);
    void* makeFragmentShader(const aex::string& vsource);
    void makeShader(void* prog, void* vprog, void* fprog);
    void freeShaderObject(void* s);
    void setAttribImpl(void* prog, const api_detail::AttribList& attribs);
    void unsetAttribImpl(void* prog, const api_detail::AttribList& attribs);
    void unsetAttribImpl(void* prog, const aex::string& name); //DEPRACATED
    void setAttribImpl(void* prog, const aex::string& name, const api_detail::DataFormatCommon& attrType); //DEPRACATED
    void drawInternal(void* prog);
    void drawInternalPart(void* prog, size_t count, size_t offset);
}
#endif
