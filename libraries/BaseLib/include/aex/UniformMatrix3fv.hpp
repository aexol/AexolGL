/** @file UniformMatrix3fv.hpp
 *  @brief Definition of UniformMatrix3fv.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __aexolGL__UniformMatrix3f__
#define __aexolGL__UniformMatrix3f__

#include <aex/UniformMatrixBase.hpp>
#include <string>
#include <vector>
#include <array>

namespace aex
{
    class AEX_PUBLIC_API UniformMatrix3fv : public UniformMatrixBase<float, 9>
    {
        typedef UniformMatrixBase<float, 9> Base;
    public:
        typedef Base::value_type value_type;
        /**
         *	@brief	Ctor
         *
         *	@param 	value 	Uniform value.
         *	@param 	name 	uniform name in program.
         */
        UniformMatrix3fv();
        UniformMatrix3fv(const aex::string& name);
        UniformMatrix3fv(const std::vector<float>& value);
        UniformMatrix3fv(const value_type& value);
        UniformMatrix3fv(const std::vector<float>& value, const aex::string& name);
        UniformMatrix3fv(const std::array<float, 9>& value, const aex::string& name);
        /**
         *	@brief	Specify uniform value.
         */
        Uniform* copy();
    protected:
        void checkUniform(void* uInfo);
    };
}

#endif /* defined(__aexolGL__UniformMatrix3f__) */
