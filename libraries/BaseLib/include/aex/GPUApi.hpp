#ifndef __AEX_GPUAPI_HPP
#define __AEX_GPUAPI_HPP
#include <aex/Point.hpp>
#include <aex/Box.hpp>
#if defined(_MSC_BUILD) && defined(__clang__)
#   include <boost/type_traits/is_default_constructible.hpp>
#   include <boost/type_traits/is_assignable.hpp>
#endif
#include <boost/signals2.hpp>


namespace aex
{
    class AWindow;
    typedef boost::signals2::signal<void()> OnContextDirtyType;
    struct AEX_PUBLIC_API ViewPort
    {
    int32_t w;
    int32_t h;
    int32_t x;
    int32_t y;
    };

    struct AEX_PUBLIC_API Color
    {
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t A;
    };

    /* Machine states */
    static CONSTEXPR uint32_t BLEND = 0x1;
    static CONSTEXPR uint32_t STENCIL_TEST = BLEND << 1;
    static CONSTEXPR uint32_t DEPTH_TEST = BLEND << 2;
    static CONSTEXPR uint32_t CULL_FACE = BLEND << 3;
    static CONSTEXPR uint32_t DITHER = BLEND << 4;
    static CONSTEXPR uint32_t POLYGON_OFFSET_FILL = BLEND << 5;
    static CONSTEXPR uint32_t SAMPLE_ALPHA_TO_COVERAGE = BLEND << 6;
    static CONSTEXPR uint32_t SAMPLE_COVERAGE = BLEND << 7;
    static CONSTEXPR uint32_t SCISSOR_TEST = BLEND << 8;
    static CONSTEXPR uint32_t UNSUPPORTED = BLEND << 9;

    /* Blend color factors */
    static CONSTEXPR uint32_t BLEND_ZERO = 1;
    static CONSTEXPR uint32_t BLEND_ONE = 2;
    static CONSTEXPR uint32_t BLEND_SRC_COLOR = 3;
    static CONSTEXPR uint32_t BLEND_ONE_MINUS_SRC_COLOR = 4;
    static CONSTEXPR uint32_t BLEND_DST_COLOR = 5;
    static CONSTEXPR uint32_t BLEND_ONE_MINUS_DST_COLOR = 6;
    static CONSTEXPR uint32_t BLEND_SRC_ALPHA = 7;
    static CONSTEXPR uint32_t BLEND_ONE_MINUS_SRC_ALPHA = 8;
    static CONSTEXPR uint32_t BLEND_DST_ALPHA = 9;
    static CONSTEXPR uint32_t BLEND_ONE_MINUS_DST_ALPHA = 10;
    static CONSTEXPR uint32_t BLEND_SRC_ALPHA_SATURATE = 11;
    static CONSTEXPR uint32_t BLEND_UNSUPPORTED = 12;

    /* Supprted blending functions */
    static CONSTEXPR uint32_t BLEND_FUNC_ADD = 1;
    static CONSTEXPR uint32_t BLEND_FUNC_SUBTRACT = 2;
    static CONSTEXPR uint32_t BLEND_FUNC_REVSUBTRACT = 3;
    static CONSTEXPR uint32_t BLEND_FUNC_UNSUPPORTED = 4;

    /***********Supported buffers**************/
    static CONSTEXPR uint32_t COLOR_BUFFER = 0x01;
    static CONSTEXPR uint32_t DEPTH_BUFFER = COLOR_BUFFER<<1;
    static CONSTEXPR uint32_t STENCIL_BUFFER = DEPTH_BUFFER<<1;

    /* Supprted read pixel data types */
    static CONSTEXPR uint32_t COLOR_INDEX = 0x01;
    static CONSTEXPR uint32_t STENCIL_INDEX = COLOR_INDEX<<1;
    static CONSTEXPR uint32_t DEPTH_COMPONENT = STENCIL_INDEX<<1;
    static CONSTEXPR uint32_t RED_CHANNEL = DEPTH_COMPONENT<<1;
    static CONSTEXPR uint32_t GREEN_CHANNEL = RED_CHANNEL<<1;
    static CONSTEXPR uint32_t BLUE_CHANNEL = GREEN_CHANNEL<<1;
    static CONSTEXPR uint32_t RGB_CHANNEL = BLUE_CHANNEL<<1;
    static CONSTEXPR uint32_t RGBA_CHANNEL = RGB_CHANNEL<<1;
    static CONSTEXPR uint32_t BGR_CHANNEL = RGBA_CHANNEL<<1;
    static CONSTEXPR uint32_t BGRA_CHANNEL = BGR_CHANNEL<<1;
    static CONSTEXPR uint32_t ALPHA_CHANNEL = BGRA_CHANNEL<<1;
    static CONSTEXPR uint32_t LUMINANCE_CHANNEL = ALPHA_CHANNEL<<1;
    static CONSTEXPR uint32_t LUMINANCE_ALPHA_CHANNEL = LUMINANCE_CHANNEL<<1;

    struct AEX_PUBLIC_API BlendState
    {
    uint32_t mSrcColorBlend;
    uint32_t mDstColorBlend;
    uint32_t mColorBlendOp;
    uint32_t mSrcAlphaBlend;
    uint32_t mDstAlphaBlend;
    uint32_t mAlphaBlendOp;
    BlendState(uint32_t srcColorBlend, uint32_t dstColorBlend, uint32_t colorBlendOp, uint32_t srcAlphaBlend, uint32_t dstAlphaBlend, uint32_t alphaBlendOp) :
        mSrcColorBlend {srcColorBlend},
        mDstColorBlend {dstColorBlend},
        mColorBlendOp {colorBlendOp},
        mSrcAlphaBlend {srcAlphaBlend},
        mDstAlphaBlend {dstAlphaBlend},
        mAlphaBlendOp {alphaBlendOp}
    {}
    BlendState(uint32_t srcBlend, uint32_t dstBlend, uint32_t blendOp):
        BlendState(srcBlend, dstBlend, blendOp, srcBlend, dstBlend, blendOp) {}
    BlendState(uint32_t srcBlend, uint32_t dstBlend):
        BlendState(srcBlend, dstBlend, BLEND_FUNC_ADD) {}
    BlendState():
        BlendState(BLEND_ONE, BLEND_ZERO, BLEND_FUNC_ADD) {}
    };

    static CONSTEXPR uint32_t DEPTH_MASK_WRITE_ENABLED = 0x01;
    static CONSTEXPR uint32_t DEPTH_MASK_WRITE_DISABLED = DEPTH_MASK_WRITE_ENABLED << 1;
    static CONSTEXPR uint32_t DEPTH_FUNC_NEVER = 0x1;
    static CONSTEXPR uint32_t DEPTH_FUNC_LESS = DEPTH_FUNC_NEVER << 1;
    static CONSTEXPR uint32_t DEPTH_FUNC_EQUAL = DEPTH_FUNC_NEVER << 2;
    static CONSTEXPR uint32_t DEPTH_FUNC_LEQUAL = DEPTH_FUNC_NEVER << 3;
    static CONSTEXPR uint32_t DEPTH_FUNC_GREATER = DEPTH_FUNC_NEVER << 4;
    static CONSTEXPR uint32_t DEPTH_FUNC_NOT_EQUAL = DEPTH_FUNC_NEVER << 5;
    static CONSTEXPR uint32_t DEPTH_FUNC_GEQUAL = DEPTH_FUNC_NEVER << 6;
    static CONSTEXPR uint32_t DEPTH_FUNC_ALWAYS = DEPTH_FUNC_NEVER << 7;

    struct AEX_PUBLIC_API DepthState
    {
    uint32_t mDepthMask;
    uint32_t mDepthFunc;
    DepthState(uint32_t depthMask, uint32_t depthFunc):
        mDepthMask {depthMask},
        mDepthFunc {depthFunc} {}
    DepthState(): DepthState(DEPTH_MASK_WRITE_ENABLED, DEPTH_FUNC_LESS) {}
    };

    void AEX_PUBLIC_API InitAexContext(AWindow& window, void* sdl_window);
    bool AEX_PUBLIC_API WasContextInit();
    void AEX_PUBLIC_API DestroyAexContext();
    void AEX_PUBLIC_API enableState(uint32_t state);
    void AEX_PUBLIC_API disableState(uint32_t state);
    void AEX_PUBLIC_API enableBlending();
    void AEX_PUBLIC_API disableBlending();
    void AEX_PUBLIC_API enableStenctilTest();
    void AEX_PUBLIC_API disableStenctilTest();
    void AEX_PUBLIC_API enableDepthTest();
    void AEX_PUBLIC_API disableDepthTest();
    void AEX_PUBLIC_API enableCullFace();
    void AEX_PUBLIC_API disableCullFace();
    void AEX_PUBLIC_API enableDither();
    void AEX_PUBLIC_API disableDither();
    void AEX_PUBLIC_API enablePolygonOffsetFill();
    void AEX_PUBLIC_API disablePolygonOffsetFill();
    void AEX_PUBLIC_API enableSampleCoverage();
    void AEX_PUBLIC_API disableSampleCoverage();
    void AEX_PUBLIC_API enableScissorTest();
    void AEX_PUBLIC_API disableScissorTest();
    void AEX_PUBLIC_API setScissorArea(int32_t w, int32_t h, int32_t x = 0, int32_t y = 0);
    void AEX_PUBLIC_API setViewPort(int32_t w, int32_t h, int32_t x = 0, int32_t y = 0);
    ViewPort AEX_PUBLIC_API CurrentViewport();
    IPoint2D AEX_PUBLIC_API GetWindowBounds();
    void AEX_PUBLIC_API setBlendState(const BlendState& bState);
    BlendState AEX_PUBLIC_API getBlendState();
    bool AEX_PUBLIC_API operator==(const BlendState& lhs, const BlendState& rhs);
    bool AEX_PUBLIC_API operator!=(const BlendState& lhs, const BlendState& rhs);
    std::vector<Color> AEX_PUBLIC_API getPixelColor(const IBox2D& box);
    std::vector<float> AEX_PUBLIC_API getPixelDepth(const IBox2D& box);
    void AEX_PUBLIC_API clearColor(float r, float g, float b, float a = 1.0f);
    void AEX_PUBLIC_API clearColor(const Color& c);
    void AEX_PUBLIC_API colorMask(bool r, bool g, bool b, bool a);
    void AEX_PUBLIC_API setDepthState(const DepthState& dState);
    const AEX_PUBLIC_API DepthState& getDepthState();
    void AEX_PUBLIC_API clearColorBuffer();
    void AEX_PUBLIC_API clearDepthBuffer();
    void AEX_PUBLIC_API clearStencilBuffer();
    void AEX_PUBLIC_API clearColorDepthBuffers();
    void AEX_PUBLIC_API clearColorDepthStencilBuffers();
    void AEX_PUBLIC_API cullFaceFront();
    void AEX_PUBLIC_API cullFaceBack();
    void AEX_PUBLIC_API cullFaceFrontAndBack();
    void AEX_PUBLIC_API stencilClearValue(int32_t value);
    void AEX_PUBLIC_API getPixelData(int xOffset, int yOffset, int xSize, int ySize, void* data, uint32_t data_type = RGBA_CHANNEL );
    void AEX_PUBLIC_API getTextureData(int target, int lvl, void* data, uint32_t data_type = RGBA_CHANNEL , uint32_t texureType = 0);
    boost::signals2::connection AEX_PUBLIC_API OnContextDirty(const OnContextDirtyType::slot_type& listener);
    void AEX_PUBLIC_API enableWireframeMode();
    void AEX_PUBLIC_API enableFillMode();
    void AEX_PUBLIC_API enablePointMode();
    /*
    * @brief Inteface for OnContextDirty signal dispatch.
    *
    * OnContextDirty signal is dispatched when contents of the context can no longer be considered valid. For example after context was lost and had to be recreated.
    */
    class AEX_PUBLIC_API IOnContextDirty
    {
    private:
        boost::signals2::scoped_connection m_onContextDirtyCon;
        void __onContextDirty();
    protected:
        /*
         * @brief Specifiecs behaviour of object when context is dirty.
         */
        virtual void onContextDirty() = 0;
    public:
        /*
         * Default, copy and move constructors and assignment operators.
         */
        IOnContextDirty();
        explicit IOnContextDirty(const IOnContextDirty& drwObj);
        explicit IOnContextDirty(IOnContextDirty&& drwObj);
        IOnContextDirty& operator=(const IOnContextDirty&);
        IOnContextDirty& operator=(IOnContextDirty&&);
        ~IOnContextDirty();
    };
}

#endif
