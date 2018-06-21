#ifndef __AEX_CONTEXTINTERNAL_HPP
#define __AEX_CONTEXTINTERNAL_HPP
#include <cstdint>
#include <memory>
#include <aex/Box.hpp>
#include <aex/GPUApi.hpp>
#include <aex/GraphicsDevice.hpp>
namespace aex
{
    class AWindow;
    struct BlendState;
    class FramebufferObject;
    void context_init(AWindow& window);
    void setVideoDevice(void* device);
    bool wasContextInit();
    void context_destroy();
    void enableStateCTX(uint32_t capability);
    void disableStateCTX(uint32_t capability);
    void setBlendStateCTX(const BlendState& bState);
    const BlendState& getBlendStateCTX();
    int MaxVertexAttribs();
    std::vector<Color> getPixelColorCTX(const IBox2D& box);
    std::vector<float> getPixelDepthCTX(const IBox2D& box);
    void clearColorCTX(float r, float g, float b, float a);
    void colorMaskCTX(bool r, bool g, bool b, bool a);
    void setDepthStateCTX(const DepthState& dState);
    const DepthState& getDepthStateCTX();
    void clearColorBufferCTX();
    void clearDepthBufferCTX();
    void clearStencilBufferCTX();
    void setScissorAreaCTX(int32_t w, int32_t h, int32_t x, int32_t y);
    void cullFaceFrontCTX();
    void cullFaceBackCTX();
    void cullFaceFrontAndBackCTX();
    void stencilClearValueCTX(int32_t value);
    void getPixelDataCTX(int xOffset, int yOffset, int xSize, int ySize, void* data,  uint32_t data_type);
    void getTextureDataCTX(int target, int level, void* data,  uint32_t dataType, uint32_t textureType);
    void setFramebuffer(const void* fbo);
    void setDrawBuffers(int size);
    const void* getFramebuffer();
    size_t MaxTextures();
    void beginContextFrame();
    void endContextFrame();
    void swapWindow();
    void enableWireframeModeCtx();
    void enableFillModeCtx();
    void enablePointModeCtx();

    class IGraphicsDeviceAPIFactory
    {
        public:
            virtual std::unique_ptr<IGraphicsDeviceAPI> create_device() = 0;
            virtual ~IGraphicsDeviceAPIFactory() = 0;
    };
    inline IGraphicsDeviceAPIFactory::~IGraphicsDeviceAPIFactory(){}
    std::unique_ptr<IGraphicsDeviceAPIFactory> get_device_factory();
}

#endif
