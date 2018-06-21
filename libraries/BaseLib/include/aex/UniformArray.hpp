/** @file UniformArray.hpp
 *  @brief Definition of UniformArray.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __aexolGL__UniformArray__
#define __aexolGL__UniformArray__

#include <aex/Uniform.hpp>
#include <aex/Common.hpp>
#include <vector>
#include <memory>
#include <string>

namespace aex
{
    class AEX_PUBLIC_API UniformArray : public Uniform
    {
    public:
        /**
         *	@brief	Ctor
         *
         *	@param 	name 	Name of uniform in program.
         */
        UniformArray(const aex::string& name = "");
        UniformArray(const UniformArray& array);
        /**
         *	@brief	Specify uniform value in program.
         *
         *	@param 	programHandler 	Program handler.
         */
        void setUniform(const AexShader& programHandler);
        void setUniform(const ShaderDrw& shader);
        /**
         *	@brief	Add content to uniform array.
         *
         *	@param 	uniforms 	uniforms to be added.
         */
        void addContent(Uniform* uniforms);
        /**
         *	@brief	Set array name.
         *
         *	@param 	name 	New name.
         */
        void setName(const aex::string& name);
        /**
         *	@brief	Get array size.
         *
         *	@return	Size.
         */
        size_t size()
        {
            return m_size;
        }
        Uniform* copy();

        virtual ~UniformArray() { }
    private:
        std::vector<Uniform*> uniforms;
        size_t  m_size;

    };
}

#endif /* defined(__aexolGL__UniformArray__) */
