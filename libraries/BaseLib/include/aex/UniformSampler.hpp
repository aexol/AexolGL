//
//  UniformSampler.h
//  aexolGL
//
//  Created by Kamil Matysiewicz on 22.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#ifndef __aexolGL__UniformSampler__
#define __aexolGL__UniformSampler__

#include <aex/UniformPrimitive.hpp>
#include <string>

namespace aex
{

    class Texture;
    class AEX_PUBLIC_API UniformSampler : public UniformPrimitive
    {
    public:

        /**
         *	@brief	Ctor
         *
         *	@param 	value 	Uniform value.
         *	@param 	name 	uniform name in program.
         */
        UniformSampler();
        UniformSampler(const Texture* value);
        UniformSampler(const aex::string& name);
        UniformSampler(const Texture* value, const aex::string& name);
        ~UniformSampler();
        
        /*UniformSampler& operator=(const UniformSampler& rhs);
        UniformSampler& operator=(const UniformSampler&& rhs);*/
        /**
         *	@brief	Specify uniform value.
         */
        void setValue(const Texture* tex);
        const Texture* getValue() const;
        Uniform* copy();
    protected:
        void checkUniform(void* uInfo);
    private:
        const Texture* m_value;
    };
}

#endif /* defined(__aexolGL__UniformSampler__) */
