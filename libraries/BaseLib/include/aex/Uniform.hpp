/** @file Uniform.hpp
 *  @brief Definition of Uniform.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __aexolGL__Uniform__
#define __aexolGL__Uniform__


#include <aex/Common.hpp>
#include <aex/Exceptions.hpp>
#include <memory>
#include <string>

namespace aex
{
    class AEX_PUBLIC_API UnnamedUniform : public std::exception
    {
    public:
        const char* what() const NOEXCEPT;
    };
    DECLARE_AEX_EXCEPTION(UniformError);
    /**
     *	@brief	Representation of uniform var in shader.
     */
    class AEX_PUBLIC_API ShaderDrw;
    class AEX_PUBLIC_API AexShaderParameterLocation;
    class AEX_PUBLIC_API Uniform
    {
        
    protected:

        
    public:
        /**
         *	@brief	Ctor
         *
         *	@param 	name 	uniform name in program.
         */
        explicit Uniform(const aex::string& name);
        explicit Uniform(aex::string&& name);
        /**
         *	@brief	Default ctor
         */
        Uniform();

        /**
         *	@brief	Get full name of uniform.
         *	@return	Full name.
         */
        aex::string getName();
        /**
         *	@brief	Get base name of uniform. For ex. in shader struct_name.base_name
         *	@return	Base name.
         */
        aex::string getBaseName();
        //Abstract
        /**
         *	@brief	Specify uniform value in shader program.
         *	@param 	programHandler 	Shader program.
         */
        virtual void setUniform(const AexShader& programHandler) = 0;
        virtual void setUniform(const ShaderDrw& shader) = 0;

                 /**
         *	@brief	Set uniform name.
         *	@param 	name 	New name.
         */
        virtual void setName(const aex::string& name) = 0;
        
        virtual Uniform* copy() = 0;
        virtual ~Uniform() = 0;

    protected:
        aex::string     m_name;
        aex::string     m_base_name;

    };
    inline Uniform::~Uniform() {};
}

#endif /* defined(__aexolGL__Uniform__) */
