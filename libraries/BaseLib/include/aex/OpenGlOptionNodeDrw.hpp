/*
 * File:   OpenGlOptionNodeDrw.hpp
 * Author: mritke
 *
 * Created on February 9, 2015, 3:25 PM
 */

#ifndef OPENGLOPTIONNODEDRW_HPP
#define	OPENGLOPTIONNODEDRW_HPP
#include <aex/Common.hpp>
#include <aex/Vector3.hpp>
#include <aex/Matrix.hpp>
#include <aex/BoundingSphereObject.hpp>
#include <aex/AexDrawable.hpp>



namespace aex
{

    class AEX_PUBLIC_API OpenGlOptionNodeDrw : public DrawObject, public DrawVisibility
    {
    private:


    protected:

    public:

        OpenGlOptionNodeDrw();
        virtual void draw(AexRenderer* gl) = 0;
        virtual void finishdraw(AexRenderer* gl) = 0;
        virtual void needsUpdate();

    };

    typedef aex::shared_ptr<OpenGlOptionNodeDrw> OpenGlOptionNodeDrw_ptr;
}

#endif	/* OPENGLOPTIONNODEDRW_HPP */

