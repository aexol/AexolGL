#include "../include/ContextInternal.hpp"
#include "../include/ApiInternal.hpp"
#include <aex/GPUApi.hpp>
#include <aex/AWindow.hpp>

namespace aex
{
    void InitAexContext(AWindow& window, void* sdl_window)
    {
        context_init(window);
        setVideoDevice(sdl_window);
    }
    bool WasContextInit()
    {
        return wasContextInit();
    }
    void DestroyAexContext()
    {
        context_destroy();
    }

    void enableState(uint32_t capability)
    {
        enableStateCTX(capability);
    }

    void disableState(uint32_t capability)
    {
        disableStateCTX(capability);
    }

    void enableBlending()
    {
        //LOG_DEBUG("enableBlending");
        enableState(BLEND);
    }

    void disableBlending()
    {
         //LOG_DEBUG("disableBlending");
        disableState(BLEND);
    }

    void enableStenctilTest()
    {
        enableState(STENCIL_TEST);
    }

    void disableStenctilTest()
    {
        disableState(STENCIL_TEST);
    }

    void enableDepthTest()
    {
        enableState(DEPTH_TEST);
    }

    void disableDepthTest()
    {
        disableState(DEPTH_TEST);
    }

    void enableCullFace()
    {
        enableState(CULL_FACE);
    }

    void disableCullFace()
    {
        disableState(CULL_FACE);
    }

    void enableDither()
    {
        enableState(DITHER);
    }

    void disableDither()
    {
        disableState(DITHER);
    }

    void enablePolygonOffsetFill()
    {
        enableState(POLYGON_OFFSET_FILL);
    }

    void disablePolygonOffsetFill()
    {
        disableState(POLYGON_OFFSET_FILL);
    }

    void enableSampleCoverage()
    {
        enableState(SAMPLE_COVERAGE);
    }

    void disableSampleCoverage()
    {
        disableState(SAMPLE_COVERAGE);
    }

    void enableScissorTest()
    {
        enableState(SCISSOR_TEST);
    }

    void disableScissorTest()
    {
        disableState(SCISSOR_TEST);
    }

    void setScissorArea(int32_t w, int32_t h, int32_t x, int32_t y)
    {
      setScissorAreaCTX(w,h, x, y);
    }

    int32_t getVertexDataLimit()
            {
        return MaxVertexAttribs();
    }

    bool validate_blend_state(const BlendState& bState)
    {
        bool isValid = true;
        isValid &= bState.mSrcColorBlend < BLEND_UNSUPPORTED;
        isValid &= bState.mDstColorBlend < BLEND_UNSUPPORTED;
        isValid &= bState.mColorBlendOp < BLEND_FUNC_UNSUPPORTED;
        isValid &= bState.mSrcAlphaBlend < BLEND_UNSUPPORTED;
        isValid &= bState.mDstAlphaBlend < BLEND_UNSUPPORTED;
        isValid &= bState.mAlphaBlendOp < BLEND_FUNC_UNSUPPORTED;
        return isValid;
    }

    void setBlendState(const BlendState& bState)
    {
        if(!validate_blend_state(bState))
            throw std::runtime_error
        {
            "Invalid blend state."
        };

        setBlendStateCTX(bState);
    }

    BlendState getBlendState()
    {
        return getBlendStateCTX();
    }

    bool operator==(const BlendState& lhs, const BlendState& rhs)
    {
        return lhs.mSrcColorBlend == rhs.mSrcColorBlend &&
               lhs.mDstColorBlend == rhs.mDstColorBlend &&
               lhs.mColorBlendOp == rhs.mColorBlendOp &&
               lhs.mSrcAlphaBlend == rhs.mSrcAlphaBlend &&
               lhs.mDstAlphaBlend == rhs.mDstAlphaBlend &&
               lhs.mAlphaBlendOp == rhs.mAlphaBlendOp;
    }

    bool operator!=(const BlendState& lhs, const BlendState& rhs)
    {
        return !(lhs == rhs);
    }

    std::vector<Color> getPixelColor(const IBox2D& box)
    {
        return getPixelColorCTX(box);
    }

    std::vector<float> getPixelDepth(const IBox2D& box)
    {
        return getPixelDepthCTX(box);
    }

    void clearColor(float r, float g, float b, float a)
    {
        clearColorCTX(r, g, b, a);
    }

    void colorMask(bool r, bool g, bool b, bool a)
    {
        colorMaskCTX(r, g, b, a);
    }

    void clearColor(const Color& c)
    {
        const float d = 255.0f;
        const float r = (float(c.R)) / d;
        const float g = (float(c.G)) / d;
        const float b = (float(c.B)) / d;
        const float a = (float(c.A)) / d;
        clearColor(r, g, b, a);
    }

    bool validate_depth_state(const DepthState& dState)
    {
        bool valid = true;
        valid &= is_pow_2(dState.mDepthMask);
        valid &= dState.mDepthMask <= DEPTH_MASK_WRITE_DISABLED;
        valid &= is_pow_2(dState.mDepthFunc);
        valid &= dState.mDepthFunc <= DEPTH_FUNC_ALWAYS;
        return valid;
    }

    void setDepthState(const DepthState& dState)
    {
        if(!validate_depth_state(dState))
            throw std::runtime_error
        {
            "Invalid depth state."
        };

        setDepthStateCTX(dState);
    }

    const DepthState& getDepthState()
    {
        return getDepthStateCTX();
    }

    void clearColorBuffer()
    {
        clearColorBufferCTX();
    }

    void clearDepthBuffer()
    {
        clearDepthBufferCTX();
    }

    void clearStencilBuffer()
    {
        clearStencilBufferCTX();
    }

    void clearColorDepthBuffers()
    {
        clearColorBuffer();
        clearDepthBuffer();
    }

    void clearColorDepthStencilBuffers()
    {
        clearColorBuffer();
        clearDepthBuffer();
        clearStencilBuffer();
    }

    void cullFaceFront()
    {
        cullFaceFrontCTX();
    }

    void cullFaceBack()
    {
        cullFaceBackCTX();
    }
    void cullFaceFrontAndBack()
    {
        cullFaceFrontAndBackCTX();
    }

    void stencilClearValue(int32_t value)
    {
        stencilClearValueCTX(value);
    }

    void getPixelData(int xOffset, int yOffset, int xSize, int ySize, void* data,  uint32_t data_type)
    {
       getPixelDataCTX( xOffset,  yOffset,  xSize,  ySize, data, data_type);
    }

    void  getTextureData(int target, int lvl, void* data, uint32_t data_type, uint32_t texureType)
    {
        getTextureDataCTX( target,  lvl, data,  data_type, texureType);
    }

    void enableWireframeMode()
    {
        enableWireframeModeCtx();
    }

    void enableFillMode()
    {
        enableFillModeCtx();
    }

    void enablePointMode()
    {
        enablePointModeCtx();
    }
}
