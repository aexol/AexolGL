/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ShaderOverride.hpp
 * Author: rafa
 *
 * Created on July 5, 2016, 12:37 PM
 */

#ifndef SHADEROVERRIDE_HPP
#define SHADEROVERRIDE_HPP

#include <aex/ShaderDrw.hpp>

namespace aex{
    
    typedef std::string (*source_update_func)(const std::string&);
    typedef void (*drw_override_func)(AexRenderer*, ShaderDrw*);

    
    class OverrideTechnique
    {
        public:

            OverrideTechnique();
            OverrideTechnique(const OverrideTechnique& ocpy);
            OverrideTechnique(OverrideTechnique&& ocpy);

            OverrideTechnique& operator=( OverrideTechnique& ovrd);
            OverrideTechnique& operator=(OverrideTechnique&& ovrd);

            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);
            virtual ShaderDrw* overrideShader(ShaderDrw* shader);

            void setPassID(int passID);
            int getPassID() const;
            void setOverrideFragFunc(source_update_func overrideFragFunc);
            void setOverrideVertFunc(source_update_func overrideVertFunc);
            void setOverrideFinishDrawFunc(drw_override_func overrideFinishDrawFunc);
            void setOverrideDrawFunc(drw_override_func overrideDrawFunc);
            bool isInit() const;

        protected:
            source_update_func m_overrideVertFunc = nullptr;
            source_update_func m_overrideFragFunc = nullptr;

            drw_override_func m_overrideDrawFunc = nullptr;
            drw_override_func m_overrideFinishDrawFunc = nullptr;

            bool m_isInit=false;
            int m_passID = -1;

            aex::unique_ptr<ShaderDrw> m_shader;


    };

    
    
    
    class MultiPassShader :public ShaderDrw
    {
    public:
        using ShaderDrw::ShaderDrw;
        
        void init();
        
        void draw(AexRenderer* gl);
        void finishdraw(AexRenderer* gl);
        void addOverride(int i, OverrideTechnique* newoverride);
        
    private:
        
        std::map<int, bool > m_noOverride;
        std::map<int, OverrideTechnique*> m_overrides;

    protected:
    };
    
}

#endif /* SHADEROVERRIDE_HPP */

