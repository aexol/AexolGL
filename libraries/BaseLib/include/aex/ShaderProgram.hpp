#ifndef __AEX_SHADERPROGRAM_HPP
#define __AEX_SHADERPROGRAM_HPP
#include <aex/Common.hpp>
#include <aex/Vector3.hpp>
#include <aex/Vector2.hpp>
#include <aex/VertexData.hpp>
#include <aex/Exceptions.hpp>
#include <functional>

namespace aex
{
    class IndexData;
    class VertexData;
    DECLARE_AEX_EXCEPTION(ShaderProgramError);
    namespace api_detail
    {
        struct DataFormatCommon;
    }
    class AEX_PUBLIC_API ShaderObject
    {
        typedef std::function<void(void*)> DeleterType;
        typedef std::unique_ptr<void, DeleterType> Impl;
    public:
        static std::unique_ptr<ShaderObject> makeVertexShader(const aex::string& vsource);
        static std::unique_ptr<ShaderObject> makeFragmentShader(const aex::string& fsource);

        ShaderObject(ShaderObject&& so);
        ~ShaderObject();
        explicit operator bool() const;
        ShaderObject();
    private:
        ShaderObject(const ShaderObject& s) = delete;
        Impl m_impl;
        friend void buildProgramObject(const AexShader& program, const std::unique_ptr<ShaderObject>& v, const std::unique_ptr<ShaderObject>& f);
    };

    class AEX_PUBLIC_API ShaderProgram
    {
        typedef std::function<void(void*)> DeleterForImpl;
        typedef std::unique_ptr<void, DeleterForImpl> Impl;
        using AttribList = VertexData::AttribList;
    public:
        static AexShader makeProgram();
        ShaderProgram();
        ShaderProgram(ShaderProgram&& prog);
        ~ShaderProgram();
        void swap(ShaderProgram& prog);
        explicit operator bool() const;
        void* getParameterLocation(const aex::string& name) const;
        void enable() const;
        void setVertexData(const VertexData& vd) const;
        void setVertexData(const VertexData& vd, const aex::string& name) const;
        void setVertexData(const VertexData& vd, const AttribList& attribs) const;
        void releaseVertexData(const VertexData& vd) const;
        void releaseVertexData(const VertexData& vd, const aex::string& name)const;
        void releaseVertexData(const VertexData& vd, const AttribList& attribs) const;
        void setIndices(const IndexData& id) const;
        void draw() const;
        void drawPart(size_t count, size_t offset) const;
    private:
        ShaderProgram(const ShaderProgram& s) = delete;
        void setAttrib(const aex::string& name, const api_detail::DataFormatCommon& attrType) const;
        void unsetAttrib(const aex::string& name) const;
        Impl m_impl;
        friend void buildProgramObject(const AexShader& program, const std::unique_ptr<ShaderObject>& v, const std::unique_ptr<ShaderObject>& f);
    };
}

#endif
