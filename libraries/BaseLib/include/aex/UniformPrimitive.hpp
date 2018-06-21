/** @file UniformPrimitive.hpp
 *  @brief Definition of UniformPrimitive.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __aexolGL__UniformPrimitive__
#define __aexolGL__UniformPrimitive__
#include <aex/Common.hpp>
#include <aex/Uniform.hpp>
#include <string>
namespace aex
{
    class AEX_PUBLIC_API UniformPrimitive : public Uniform
    {
    public:
        /**
         *	@brief	Ctor
         *
         *	@param 	name 	uniform name in program.
         */
        explicit UniformPrimitive(const aex::string& name);
        explicit UniformPrimitive(aex::string&& name);
        /**
         *	@brief	Default ctor
         */
        UniformPrimitive();
        virtual ~UniformPrimitive();
        /**
         *	@brief	Specify uniform value in shader program.
         *
         *	@param 	programHandler 	Shader program.
         */
        void setUniform(const AexShader& programHandler);
        void setUniform(const ShaderDrw& shader);
        /*
         *	@brief	Set uniform name.
         *
         *	@param 	name 	New name.
         */
        void setName(const aex::string& name);
        //virtual             ~UniformPrimitive();
    protected:
        //void valueChanged();
        /**
         *	@brief	Specify uniform value.
         */
        virtual void        checkUniform(void* uInfo) = 0;
    private:
        void addContent(std::unique_ptr<Uniform>&&);
    };
}

#endif /* defined(__aexolGL__UniformPrimitive__) */
