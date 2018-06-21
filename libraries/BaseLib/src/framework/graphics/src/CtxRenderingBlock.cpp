#include <aex/CtxRenderingBlock.hpp>
#include <aex/AexRenderer.hpp>

namespace aex
{
    RenderContextBlockImpl::RenderContextBlockImpl(AexRenderer* gl): m_aexRenderer(gl){}

    void 
    RenderContextBlockImpl::setContext(AexRenderer* gl)
    {
        m_aexRenderer = gl;
    }

    AexRenderer* 
    RenderContextBlockImpl::getContext()
    {
        return m_aexRenderer;
    }

    bool
    RenderContextBlockImpl::init()
    {
        return m_aexRenderer != nullptr && m_aexRenderer->InitAPI(m_aexRenderer->GetWindow());
    }

    void
    RenderContextBlockImpl::beginFrame()
    {
        m_aexRenderer->BeginFrame();
    }

    void
    RenderContextBlockImpl::endFrame()
    {
        m_aexRenderer->EndFrame();
    }
} // end of aex
