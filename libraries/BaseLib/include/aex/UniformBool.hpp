/** @file UniformBool.hpp
 *  @brief Definition of UniformBool.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __aexolGL__UniformBool__
#define __aexolGL__UniformBool__

#include <aex/UniformPrimitive.hpp>
#include <string>

namespace aex
{

    class AEX_PUBLIC_API UniformBool : public UniformPrimitive
    {
    public:

        /**
         *	@brief	Ctor
         *
         *	@param 	value 	Uniform value.
         *	@param 	name 	uniform name in program.
         */
        UniformBool();
        UniformBool(bool value);
        UniformBool(const aex::string& name);
        UniformBool(bool value, const aex::string& name);
        Uniform* copy();
        //virtual ~UniformBool();

        /*UniformBool& operator=(const UniformBool& rhs);
        UniformBool& operator=(const UniformBool&& rhs);*/
        
        /**
         *	@brief	Specify uniform value.
         */
        void setValue(bool value)
        {
            m_value = value;
        }

        bool isValue() const
        {
            return m_value;
        }
        
        bool getValue() const
        {
            return m_value;
        }
        
    protected:
        void checkUniform(void* uInfo);
    private:
        bool m_value;
    };

    typedef aex::shared_ptr<UniformBool> UniformBool_ptr;

}

#endif /* defined(__aexolGL__UniformBool__) */
