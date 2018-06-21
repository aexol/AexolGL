/*
 * File:   CameraImplAndroid.hpp
 * Author: darek
 *
 * Created on February 26, 2015, 1:35 PM
 */

#ifndef CAMERAIMPLANDROID_HPP
#define	CAMERAIMPLANDROID_HPP

#include "../../include/CameraImpl.hpp"

namespace aex {
    namespace cam {
        
        class CameraImplAndroid : public CameraImpl {
        public:
            CameraImplAndroid(int texID);
            virtual void setHWTexture(HWTexture& hw);
            virtual void release();
            virtual void replaceTextureID(int newTexID);
            virtual ~CameraImplAndroid();

        private:
            int m_texID;
            GLuint* p_texIDPtr;
        };
    } // end of namespace cam
} // end of namespace aex

#endif	/* CAMERAIMPLANDROID_HPP */

