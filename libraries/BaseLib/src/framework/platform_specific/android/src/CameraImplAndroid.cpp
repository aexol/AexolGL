#include "CameraImplAndroid.hpp"
#include "../../../graphics/api/include/GLTexture.hpp"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <aex/Logger.hpp>


namespace aex
{
    namespace cam
    {
        CameraImplAndroid::CameraImplAndroid(int texID) :
        m_texID(texID)
        {
        }

        void CameraImplAndroid::setHWTexture(HWTexture& hw)
        {
            GLTexture* tt = new GLTexture;
            tt->mTexture = m_texID;
            tt->mTarget = GL_TEXTURE_EXTERNAL_OES;
            tt->mTURef = 0; 

            p_texIDPtr = &tt->mTexture;

            HWTexture::impl_ptr hw_imp(tt);
            hw.setImpl(std::move(hw_imp));
        }

        void CameraImplAndroid::release()
        {
            
        }

        void CameraImplAndroid::replaceTextureID(int newTexID)
        {
            *p_texIDPtr = newTexID;
            LOG("new tex id: ", newTexID);
        }

        CameraImplAndroid::~CameraImplAndroid()
        {
        }

    } // end of namespace cam
} // end of namespace aex
