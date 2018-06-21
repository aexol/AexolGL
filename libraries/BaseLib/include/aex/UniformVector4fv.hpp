//
//  UniformVector4fv.h
//  aexolGL
//
//  Created by Kamil Matysiewicz on 22.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#ifndef __aexolGL__UniformVector4fv__
#define __aexolGL__UniformVector4fv__

#include <aex/UniformVectorBase.hpp>
#include <aex/ax_Macros.hpp>
#include <string>
#include <vector>

namespace aex
{
class AEX_PUBLIC_API UniformVector4fv : public UniformVectorBase<float, 4>
{
    typedef UniformVectorBase<float, 4> Base;
public:
    typedef Base::value_type value_type;
    /**
     *	@brief	Ctor
     *
     *	@param 	value 	Uniform value.
     *	@param 	name 	uniform name in program.
     */
    using Base::setValue;
    UniformVector4fv();
    UniformVector4fv(const aex::string& name);
    UniformVector4fv(const value_type& value);
    UniformVector4fv(const value_type& value, const aex::string& name);
    /**
     *	@brief	Specify uniform value.
     */
    Uniform* copy();
protected:
    void checkUniform(void* uInfo);
};

typedef aex::shared_ptr<UniformVector4fv> UniformVector4fv_ptr;
}

#endif /* defined(__aexolGL__UniformVector4fv__) */
