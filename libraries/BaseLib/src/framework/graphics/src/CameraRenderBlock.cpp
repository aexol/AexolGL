#include <aex/CameraRenderBlock.hpp>
#include <aex/AexRenderer.hpp>
#include <aex/AWindow.hpp>

namespace aex
{
    CameraRenderBlockImpl::CameraRenderBlockImpl(): m_aexRenderer{}, m_camera{}{}

    void 
    CameraRenderBlockImpl::setContext(AexRenderer* gl)
    {
        m_aexRenderer = gl;
    }

    void 
    CameraRenderBlockImpl::setCamera(QuaternionCamera* camera)
    {
        m_camera = camera;
    }

    bool
    CameraRenderBlockImpl::init()
    {
        if(m_camera)
        {
            m_camera->set_behavior(aex::CAMERA_MODES::FP_MODE);
            if(m_aexRenderer && m_aexRenderer->GetWindow())
            {
                m_camera->perspective(70.0f, (GLfloat) m_aexRenderer->GetWindow()->w(), (GLfloat) m_aexRenderer->GetWindow()->h(), 1.0f, 500.0f);
            }
            m_camera->ortho(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 5000.0f);
            m_camera->lookAt(Vector(0.0f, 0.0f, 0.0f), Vector(1.0f, 0.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f));
        }
        return m_aexRenderer != nullptr && m_camera != nullptr;
    }

    void
    CameraRenderBlockImpl::beginFrame()
    {
        m_aexRenderer->setQuatCamera(m_camera);
        if(m_camera->isCameraMoved())
        {
            m_aexRenderer->setViewProjectionMatrix(m_camera->transforms());
            m_aexRenderer->setViewMatrix(m_camera->get_view_matrix());
        }
    }

    void
    CameraRenderBlockImpl::endFrame()
    {
    }
} // end of aex
