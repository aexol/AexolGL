//
//  UniformMatrix4fv.h
//  aexolGL
//
//  Created by Kamil Matysiewicz on 22.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#ifndef __aexolGL__UniformMatrix4fv__
#define __aexolGL__UniformMatrix4fv__

#include <aex/UniformMatrixBase.hpp>
#include <string>
#include <array>

namespace aex
{
    namespace math
    {
        class Matrix;
    }

    class AEX_PUBLIC_API UniformMatrix4fv : public UniformMatrixBase<float, 16>
    {
        typedef aex::math::Matrix Matrix;
        typedef UniformMatrixBase<float, 16> Base;
    public:
        typedef Base::value_type value_type;
        /**
         *	@brief	Ctor
         *
         *	@param 	value 	Uniform value.
         *	@param 	name 	uniform name in program.
         */
        using Base::setValue;
        UniformMatrix4fv();
        explicit UniformMatrix4fv(const aex::string& name);
        explicit UniformMatrix4fv(const std::vector<float>& value);
        explicit UniformMatrix4fv(const value_type& value);
        explicit UniformMatrix4fv(const Matrix& value);
        UniformMatrix4fv(const std::vector<float>& value, const aex::string& name);
        UniformMatrix4fv(const value_type& value, const aex::string& name);
        UniformMatrix4fv(const Matrix& value, const aex::string& name);
        /**
         *	@brief	Specify uniform value.
         */
        Uniform* copy();
        void setValue(const Matrix& value);
    protected:
        void checkUniform(void* uInfo);
    };
}

#endif /* defined(__aexolGL__UniformMatrix4fv__) */
