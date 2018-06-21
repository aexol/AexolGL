/* 
 * File:   UniformUpdateDrw.hpp
 * Author: mritke
 *
 * Created on 22 stycznia 2016, 11:12
 */

#ifndef UNIFORMUPDATEDRW_HPP
#define	UNIFORMUPDATEDRW_HPP

#include <aex/AexDrawable.hpp>
#include <aex/Uniforms.hpp>
namespace aex
{

    class AEX_PUBLIC_API UniformUpdateDrw : public DrawObject
    {
    private:
        std::vector<Uniform*> m_uniforms;
        
     
    protected:

    public:

        UniformUpdateDrw();
        
        void draw(AexRenderer* gl);
        void finishdraw(AexRenderer* gl);
        void needsUpdate(){}
        

    };


}

#endif	/* UNIFORMUPDATEDRW_HPP */

