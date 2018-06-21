#include "CameraImplIOS_Y_UV.hpp"
#include "../../../graphics/api/include/GLTexture.hpp"
#include <aex/Logger.hpp>


namespace aex
{
    namespace cam
    {
        CameraImplIOS_Y_UV::CameraImplIOS_Y_UV(int target) :
        m_target(target)
        {
            m_createdTexturesNum = 0;
            m_nextSetTexCode = SET_Y_TEXTURE_CODE;
        }


        CameraImplIOS_Y_UV::~CameraImplIOS_Y_UV()
        {
        }

        void CameraImplIOS_Y_UV::setHWTexture(HWTexture& hw)
        {
            if (m_createdTexturesNum > 2)
            {
                LOG_ERR("BAD USING CameraImplIOS_Y_UV. setHWTexture can be only invoked 3 times for Y, UV  textures");
                return;
            }

            m_createdTexturesNum++;

            GLTexture* tt = new GLTexture;
            tt->mTexture = 0; // setted on first and next frame, and next etc...
            tt->mTarget = m_target; // GL_TEXTURE_2D
            tt->mTURef = 99999; // must be bigger than MaxTUnits value in OGL api to be visible as totally new tex


            HWTexture::impl_ptr hw_imp(tt);
            hw.setImpl(std::move(hw_imp));

            // gives possibility to change id always when new ios camera frame appear and genereta new textures in texture cache
            if (m_nextSetTexCode == SET_Y_TEXTURE_CODE)
            {
                m_texid_y_ptr = &tt->mTexture;
                m_hwTextureY = &hw;
            }

            if (m_nextSetTexCode == SET_UV_TEXTURE_CODE)
            {
                m_texid_uv_ptr = &tt->mTexture;
                m_hwTextureUV = &hw;
            }
        }

        void CameraImplIOS_Y_UV::release()
        {
            *m_texid_y_ptr = 0; // glDeleteTextures on id 0 do nothing, something like delete null;
            *m_texid_uv_ptr = 0;
        }

        void CameraImplIOS_Y_UV::nextCameraFrame(int y_tex_id, int uv_tex_id)
        {
            GLTexture* ttY = new GLTexture;
            ttY->mTexture = y_tex_id;
            ttY->mTarget = m_target;
            ttY->mTURef = 99999; // must be bigger than MaxTUnits value in OGL api to be visible as totally new tex

            *m_texid_y_ptr = 0;  // when previous tex will be removed in aex api, tex=0 removes nothing so its ok (this textures removes iOS api)
            HWTexture::impl_ptr hwY_imp(ttY);
            m_hwTextureY->setImpl(std::move(hwY_imp));
            m_texid_y_ptr = &ttY->mTexture;

            GLTexture* ttUV = new GLTexture;
            ttUV->mTexture = uv_tex_id;
            ttUV->mTarget = m_target;
            ttUV->mTURef = 99999; // must be bigger than MaxTUnits value in OGL api to be visible as totally new tex

            *m_texid_uv_ptr = 0; // when previous tex will be removed in aex api, tex=0 removes nothing so its ok (this textures removes iOS api)
            HWTexture::impl_ptr hwUV_imp(ttUV);
            m_hwTextureUV->setImpl(std::move(hwUV_imp));
            m_texid_uv_ptr = &ttUV->mTexture;

            m_hwTextureY->refreshTexParams(3,3); // force use CLAMP_TO_EDGE always using not pow2 size tex (in specification of setTexParams method))
            m_hwTextureUV->refreshTexParams(3,3);
        }

        void CameraImplIOS_Y_UV::setNextSettedTextureCode(int code)
        {
            m_nextSetTexCode = code;
        }

    } // end of namespace cam
} // end of namespace aex
