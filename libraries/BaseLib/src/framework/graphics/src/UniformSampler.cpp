//
//  UniformSampler.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 22.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//
#include "../include/TextureInternal.hpp"
#include "../include/UniformInternal.hpp"
#include <aex/UniformSampler.hpp>
#include <aex/Texture.hpp>
namespace aex {

    UniformSampler::UniformSampler() : UniformSampler(nullptr, "") {}
	UniformSampler::UniformSampler(const aex::string& name) : UniformSampler(nullptr, name) {}
	UniformSampler::UniformSampler(const Texture* value) : UniformSampler(value, "") {}
	UniformSampler::UniformSampler(const Texture* value, const aex::string& name) :
    UniformPrimitive(name),
    m_value(value) {
    }

    UniformSampler::~UniformSampler()
    {
        
    }
    
    void
    UniformSampler::checkUniform(void* uInfo) {
        const HWTexture* hwTex;

        if (m_value && (hwTex = static_cast<const HWTexture*> (m_value->getTexture()))) {
            UniformLib::setSamplerShaderParam(uInfo, hwTex);
            //            hwTex->setSampler(uInfo);
        }
    }

    Uniform*
    UniformSampler::copy() {
        return new UniformSampler{*this};
    }

    void
    UniformSampler::setValue(const Texture* tex) {
        m_value = tex;
    }

    const Texture*
    UniformSampler::getValue() const {
        return m_value;
    }
    
    /*UniformSampler&
    UniformSampler::operator=(const UniformSampler& rhs)
    {
        this->m_name = rhs.m_name;
        this->m_base_name = rhs.m_base_name;
        this->m_value = rhs.m_value;
        return *this;
    }

    UniformSampler&
    UniformSampler::operator=(UniformSampler&& rhs)
    {
        this->m_name = std::move(rhs.m_name);
        this->m_base_name = std::move(rhs.m_base_name);
        this->m_value = std::move(rhs.m_value);
        return *this;
    }*/


}
