/*
 * File:   CameraImpl.hpp
 * Author: darek
 *
 * Created on February 26, 2015, 1:14 PM
 */

#ifndef CAMERAIMPL_HPP
#define	CAMERAIMPL_HPP

#include <aex/Common.hpp>
#include <aex/AexDrawable.hpp>
#include <aex/ShaderDrw.hpp>
#include <aex/GameEvent.hpp>
#include <aex/HudElement.hpp>
#include <aex/VertexData.hpp>
#include <aex/UniformMatrix4fv.hpp>
#include <aex/UniformSampler.hpp>
#include <aex/ShaderProgram.hpp>
#include <array>

#include "../../graphics/include/TextureInternal.hpp"
#include <aex/Camera.hpp>

namespace aex {
    namespace cam {

        class CameraImpl {
        protected:

        public:
            virtual void setHWTexture(HWTexture& hw) = 0;
            virtual void release() = 0;

            virtual ~CameraImpl() {
            }

        };
    } // end of namespace cam
} // end of namespace aex

#endif	/* CAMERAIMPL_HPP */

