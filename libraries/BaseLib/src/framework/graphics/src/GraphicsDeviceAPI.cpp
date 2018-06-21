/**************************************************************
 *                                                            * 
 *      Filename:   GraphicsDeviceAPI.cpp                     * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2016 lut 15 15:43:49                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/

#include <aex/GraphicsDevice.hpp>
#include "../include/ContextInternal.hpp"

namespace aex
{
    GraphicsDevice::GraphicsDevice() : m_api(get_device_factory()->create_device()){}
    GraphicsDevice::GraphicsDevice(const GraphicsDevice& gd) : m_api(get_device_factory()->create_device()){}
    GraphicsDevice& GraphicsDevice::operator=(const GraphicsDevice& gd)
    {
        if(this != &gd)
            m_api = get_device_factory()->create_device();
        return *this;
    }
    GraphicsDevice::GraphicsDevice(GraphicsDevice&& gd) : m_api(std::move(gd.m_api)){}
    GraphicsDevice& GraphicsDevice::operator=(GraphicsDevice&& gd)
    {
        if(this != &gd)
            m_api = std::move(gd.m_api);
        return *this;
    }
    bool GraphicsDevice::initAPI(AWindow* window, uint32_t enables, uint32_t disables)
    {
        bool success = false;
        // Depth test enabled by default.
        if(!(disables & DEPTH_TEST))
            enables |= DEPTH_TEST;

        // Face culling disabled by default.
        if(!(enables & CULL_FACE))
            disables |= CULL_FACE;

        // Disable stencil test by default.
        if(!(enables & STENCIL_TEST))
            disables |= STENCIL_TEST;

        // Enable blending by default.
        if(!(disables & BLEND))
            enables |= BLEND;
        success = m_api->initAPI(window, enables, disables);

        // Set default blending state and disable blending.
        aex::setBlendState(BlendState {BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA});
        disableBlending();

        // Set default depth state.
        setDepthState(DepthState());

        // Front face culling.
        cullFaceFront();
        clearColorDepthStencilBuffers();

        return success;
    }
    void GraphicsDevice::finishAPI()
    {
        m_api->finishAPI();
    }
    void GraphicsDevice::beginFrame()
    {
        m_api->beginFrame();
    }
    void GraphicsDevice::beginFrame(uint32_t buffers)
    {
        m_api->beginFrame(buffers);
    }
    void GraphicsDevice::endFrame()
    {
        m_api->endFrame();
    }
}
