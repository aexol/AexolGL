/** @file UniformMatrix2fv.hpp
 *  @brief Definition of UniformMatrix2fv.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __aexolGL__UniformMatrix2f__
#define __aexolGL__UniformMatrix2f__

#include <aex/UniformMatrixBase.hpp>
#include <string>
#include <vector>
#include <array>

namespace aex
{
    class AEX_PUBLIC_API UniformMatrix2fv : public UniformMatrixBase<float, 4>
    {
        typedef UniformMatrixBase<float, 4> Base;
    public:
        typedef Base::value_type value_type;
        /**
         *	@brief	Ctor
         *
         *	@param 	value 	Uniform value.
         *	@param 	name 	uniform name in program.
         */
        UniformMatrix2fv();
        UniformMatrix2fv(const aex::string& name);
        UniformMatrix2fv(const std::vector<float>& value);
        UniformMatrix2fv(const value_type& value);
        UniformMatrix2fv(const std::vector<float>& value, const aex::string& name);
        UniformMatrix2fv(const value_type& value, const aex::string& name);
        Uniform* copy();
    protected:
        void checkUniform(void* uInfo);
    };
}

#endif /* defined(__aexolGL__UniformMatrix2f__) */
