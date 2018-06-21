/*
 * File:   CameraImplAndroid.hpp
 * Author: darek
 *
 * Created on February 26, 2015, 1:35 PM
 */

#ifndef CAMERAIMPLIOS_HPP
#define	CAMERAIMPLIOS_HPP

#include "../../include/CameraImpl.hpp"
#include <aex/FrameBuffer.hpp>

namespace aex {
    namespace cam {

        class CameraImplIOS_Y_UV : public CameraImpl {
        public:
            static const int SET_Y_TEXTURE_CODE = 9;
            static const int SET_UV_TEXTURE_CODE = 6;

            CameraImplIOS_Y_UV(int target);
            virtual ~CameraImplIOS_Y_UV();

            virtual void setHWTexture(HWTexture& hw);
            virtual void release();
            void nextCameraFrame(int y_tex_id, int uv_tex_id);
            void setNextSettedTextureCode(int code);

            
        private:
            int m_nextSetTexCode;
            int m_createdTexturesNum;
            int m_target;
            GLuint* m_texid_y_ptr;
            GLuint* m_texid_uv_ptr;
            HWTexture* m_hwTextureY;
            HWTexture* m_hwTextureUV;
        };
        
    } // end of namespace cam
} // end of namespace aex

#endif	/* CAMERAIMPLANDROID_HPP */

