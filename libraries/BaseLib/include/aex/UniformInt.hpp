/*
 * File:   UniformInt.hpp
 * Author: mritke
 *
 * Created on February 23, 2015, 4:24 PM
 */

#ifndef UNIFORMINT_HPP
#define	UNIFORMINT_HPP
#include <aex/UniformPrimitive.hpp>
#include <string>




namespace aex
{

    class AEX_PUBLIC_API UniformInt : public UniformPrimitive
    {
    public:
        /**
         *	@brief	Ctor
         *
         *	@param 	value 	Uniform value.
         *	@param 	name 	uniform name in program.
         */
        UniformInt();
        UniformInt(int value);
        UniformInt(const aex::string& name);
        UniformInt(int value, const aex::string& name);
        virtual ~UniformInt();
        /**
         *	@brief	Specify uniform value.
         */
        void setValue(int value);
        Uniform* copy();
        int getValue() const;
    protected:
        void checkUniform(void* uInfo);
    private:
        int m_value;
    };

    typedef aex::shared_ptr<UniformInt> UniformInt_ptr;
}
#endif	/* UNIFORMINT_HPP */

