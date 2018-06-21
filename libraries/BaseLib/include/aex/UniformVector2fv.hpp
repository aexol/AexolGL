//
//  UniformVector2fv.h
//  aexolGL
//
//  Created by Kamil Matysiewicz on 22.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#ifndef __aexolGL__UniformVector2fv__
#define __aexolGL__UniformVector2fv__

#include <aex/UniformVectorBase.hpp>
#include <aex/Vector2.hpp>
#include <string>
#include <vector>

namespace aex
{

class AEX_PUBLIC_API UniformVector2fv : public UniformVectorBase<float, 2>
{
    typedef UniformVectorBase<float, 2> Base;
public:
    typedef Base::value_type value_type;
    /**
     *	@brief	Ctor
     *
     *	@param 	value 	Uniform value.
     *	@param 	name 	uniform name in program.
     */
    using Base::setValue;
    UniformVector2fv();
    UniformVector2fv(const aex::string& name);
    UniformVector2fv(const value_type& value);
    UniformVector2fv(const value_type& value, const aex::string& name);
    void setValue(const std::vector<aex::math::Vector2>& value);

    /**
     *	@brief	Specify uniform value.
     */
    Uniform* copy();
protected:
    void checkUniform(void* uInfo);
};
}

#endif /* defined(__aexolGL__UniformVector2fv__) */
