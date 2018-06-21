//
//  UniformVector1f.h
//  aexolGL
//
//  Created by Kamil Matysiewicz on 22.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#ifndef __aexolGL__UniformVector1f__
#define __aexolGL__UniformVector1f__

#include <aex/UniformVectorBase.hpp>
#include <string>
#include <vector>
#include <array>

namespace aex
{
class AEX_PUBLIC_API UniformVector1fv : public UniformVectorBase<float, 1>
{
    typedef UniformVectorBase<float, 1> Base;
public:
    typedef Base::value_type value_type;
    /**
     *	@brief	Ctor
     *
     *	@param 	value 	Uniform value.
     *	@param 	name 	uniform name in program.
     */
    using Base::setValue;
    UniformVector1fv();
    UniformVector1fv(const aex::string& name);
    UniformVector1fv(const value_type& value);
    UniformVector1fv(const value_type& value, const aex::string& name);
    /**
     *	@brief	Specify uniform value.
     */
    Uniform* copy();
protected:
    void checkUniform(void* uInfo);
};
}

#endif /* defined(__aexolGL__UniformVector1f__) */
