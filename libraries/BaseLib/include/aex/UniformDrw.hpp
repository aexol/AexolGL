/* 
 * File:   UniformDrw.hpp
 * Author: mritke
 *
 * Created on 24 lutego 2016, 13:48
 */

#ifndef UNIFORMDRW_HPP
#define	UNIFORMDRW_HPP
#include <aex/AexDrawable.hpp>
#include <aex/Uniforms.hpp>

namespace aex 
{
    
    class AEX_PUBLIC_API UniformListDrw : public DrawObject
    {

    public:
        UniformListDrw();
        UniformListDrw(std::vector<Uniform*> uniforms);
        ~UniformListDrw();
        
        void draw(AexRenderer* gl);
        void finishdraw(AexRenderer* gl);
        void needsUpdate();
        
        void addUniform(Uniform* uniform);
        void removeUniform(Uniform* uniform);
        
    protected: 
        std::vector<Uniform*> m_uniforms;
        
    };
    
    
    typedef aex::shared_ptr<UniformListDrw> UniformListDrw_ptr;
}


#endif	/* UNIFORMDRW_HPP */

