//
//  UniformStruct.h
//  aexolGL
//
//  Created by Kamil Matysiewicz on 22.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#ifndef __aexolGL__UniformStruct__
#define __aexolGL__UniformStruct__
#include <aex/Uniform.hpp>
#include <aex/UniformPrimitive.hpp>
#include <vector>
#include <memory>
#include <string>

namespace aex
{
    class AEX_PUBLIC_API UniformStruct : public Uniform
    {
    public:
        /**
         *	@brief	Ctor
         *
         *	@param 	name 	Name of uniform in program.
         */
        UniformStruct(const aex::string& name = "");
        
        
        
        /**
         *	@brief	Specify uniform value in program.
         *
         *	@param 	programHandler 	Program handler.
         */
        void        setUniform(const AexShader& programHandler);
        void        setUniform(const ShaderDrw& shader);
        /**
         *	@brief	Add content to uniform struct.
         *
         *	@param 	uniforms 	uniforms to be added.
         */
        
        void        setContent(std::vector<Uniform*> uniforms);
        
        void        addContent(Uniform*);
        /**
         *	@brief	Set struct name.
         *
         *	@param 	name 	New name.
         */    
        void        setName(const aex::string& name);
        /**
         *	@brief	Is empty struct.
         *
         *	@retval True if yes.
         *  @retval False if no.
         */
        bool empty()
        {
            return m_empty;
        };
        Uniform* copy();

        ~UniformStruct();
    private:
        std::vector<Uniform*> uniforms;
        bool    m_empty;
    };
}

#endif /* defined(__aexolGL__UniformStruct__) */
