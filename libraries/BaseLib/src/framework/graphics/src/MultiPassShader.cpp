/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <aex/MultiPassShader.hpp>

namespace aex {

    OverrideTechnique::OverrideTechnique() {

    }

    OverrideTechnique::OverrideTechnique(const OverrideTechnique& ocpy) {
        
    }

    OverrideTechnique::OverrideTechnique(OverrideTechnique&& ocpy) {
        *this= ocpy;
    }

    OverrideTechnique& OverrideTechnique::operator=(OverrideTechnique& ovrd) {

        if (this != &ovrd) {
            m_overrideVertFunc = ovrd.m_overrideVertFunc;
            m_overrideFragFunc = ovrd.m_overrideFragFunc;

            m_overrideDrawFunc = ovrd.m_overrideDrawFunc;
            m_overrideFinishDrawFunc = ovrd.m_overrideFinishDrawFunc;

            m_isInit = ovrd.m_isInit;
            m_passID = ovrd.m_passID;

            m_shader = std::move(ovrd.m_shader);
        }

        return *this;
    }

    OverrideTechnique& OverrideTechnique::operator=(OverrideTechnique&& ovrd) {
        if (this != &ovrd) {
            m_overrideVertFunc = ovrd.m_overrideVertFunc;
            m_overrideFragFunc = ovrd.m_overrideFragFunc;

            m_overrideDrawFunc = ovrd.m_overrideDrawFunc;
            m_overrideFinishDrawFunc = ovrd.m_overrideFinishDrawFunc;

            m_isInit = ovrd.m_isInit;
            m_passID = ovrd.m_passID;

            m_shader = std::move(ovrd.m_shader);
        }

        return *this;
    }

    ShaderDrw* OverrideTechnique::overrideShader(ShaderDrw* shader) {
        
        std::string nfrag, nvert;
        if(m_overrideFragFunc)
        {
            nfrag = m_overrideFragFunc(shader->getFragment_lines());
            
        }else{
            nfrag = shader->getFragment_lines();
        }
        
        if(m_overrideFragFunc)
        {
            nvert = m_overrideVertFunc(shader->getVertex_lines());
        }
        else
        {
            nvert=shader->getVertex_lines();
        }

        m_shader.reset(new ShaderDrw(nfrag, nvert, shader->getVertex_attr()));

        m_isInit = true;
        return m_shader.get();
    }

    void OverrideTechnique::setPassID(int passID) {
        m_passID = passID;
    }

    int OverrideTechnique::getPassID() const {
        return m_passID;
    }

    void OverrideTechnique::setOverrideFragFunc(source_update_func overrideFragFunc) {
        m_overrideFragFunc = overrideFragFunc;
    }

    void OverrideTechnique::setOverrideVertFunc(source_update_func overrideVertFunc) {
        m_overrideVertFunc = overrideVertFunc;
    }

    void OverrideTechnique::setOverrideFinishDrawFunc(drw_override_func overrideFinishDrawFunc) {
        m_overrideFinishDrawFunc = overrideFinishDrawFunc;
    }

    void OverrideTechnique::setOverrideDrawFunc(drw_override_func overrideDrawFunc) {
        m_overrideDrawFunc = overrideDrawFunc;
    }

    bool OverrideTechnique::isInit() const {
        return m_isInit;
    }



    void OverrideTechnique::draw(AexRenderer* gl) {
        if (m_overrideDrawFunc) {
            m_overrideDrawFunc(gl, m_shader.get());
        } else {
            m_shader->draw(gl);
        }
    }

    void OverrideTechnique::finishdraw(AexRenderer* gl) {
        if (m_overrideFinishDrawFunc) {
            m_overrideFinishDrawFunc(gl, m_shader.get());
        } else {
            m_shader->finishdraw(gl);
        }
    }


    //    MultiPassShader::MultiPassShader() {
    //
    //    }

    void MultiPassShader::draw(AexRenderer* gl) {

        int passID = gl->getCurrentPassID();
        
        if (passID == 0 || m_noOverride[passID]) {
            ShaderDrw::draw(gl);
        } else if (m_overrides.find(passID) != m_overrides.end()) {
            m_overrides[passID]->draw(gl);
        } else {
            if (m_overrides.find(passID) != m_overrides.end()) {
                m_overrides[passID]->overrideShader(this);
                m_overrides[passID]->draw(gl);
                m_noOverride[passID] = false;
            } else {
                m_noOverride[passID] = true;
                ShaderDrw::draw(gl);
            }
        }

    }

    void MultiPassShader::addOverride(int i, OverrideTechnique* newoverride) {
        m_overrides.emplace(i, newoverride);
    }

    void MultiPassShader::finishdraw(AexRenderer* gl) {
        int passID = gl->getCurrentPassID();
        if (passID == 0 || m_noOverride[passID]) {
            ShaderDrw::finishdraw(gl);
        } else if (m_overrides.find(passID) != m_overrides.end()) {
            m_overrides[passID]->finishdraw(gl);
        }

    }






}
