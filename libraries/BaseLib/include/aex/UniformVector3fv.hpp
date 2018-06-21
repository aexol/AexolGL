//
//  UniformVector3fv.h
//  aexolGL
//
//  Created by Kamil Matysiewicz on 22.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#ifndef __aexolGL__UniformVector3fv__
#define __aexolGL__UniformVector3fv__

#include <aex/UniformVectorBase.hpp>
#include <aex/Vector3.hpp>
#include <string>
#include <vector>

namespace aex
{
class AEX_PUBLIC_API UniformVector3fv : public UniformVectorBase<float, 3>
{
    typedef UniformVectorBase<float, 3> Base;
public:
    typedef Base::value_type value_type;
    /**
     *	@brief	Ctor
     *
     *	@param 	value 	Uniform value.
     *	@param 	name 	uniform name in program.
     */
    using Base::setValue;
    UniformVector3fv();
    UniformVector3fv(const aex::string& name);
    UniformVector3fv(const value_type& value);
    UniformVector3fv(const aex::math::Vector3& value);
    UniformVector3fv(const value_type& value, const aex::string& name);
    UniformVector3fv(const aex::math::Vector3& value, const aex::string& name);
    /**
     *	@brief	Specify uniform value.
     */
    Uniform* copy();
    void setValue(const std::vector<aex::math::Vector3>& value);
protected:
    void checkUniform(void* uInfo);
};

typedef aex::shared_ptr<UniformVector3fv> UniformVector3fv_ptr;
}

#endif /* defined(__aexolGL__UniformVector3fv__) */
