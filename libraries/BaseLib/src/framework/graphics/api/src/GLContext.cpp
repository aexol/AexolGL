#include "../include/GLContext.hpp"
#include "../include/GLFramebuffer.hpp"
#include "../include/GLTexture.hpp"
#include "../../include/ShaderProgramInternal.hpp"
#include "../../include/ContextInternal.hpp"
#include "../../include/TextureInternal.hpp"
#include "../../include/FramebufferObjectInternal.hpp"
#include "../include/GLShaderProgram.hpp"
#include <aex/Common.hpp>
#include <aex/Logger.hpp>
#include <aex/ax_Macros.hpp>
#include <aex/utils.hpp>
#include <aex/api_definitions.hpp>
#include <aex/ShaderProgram.hpp>
#include <aex/VertexData.hpp>
#include <aex/GPUApi.hpp>
#include <aex/TLS.hpp>
#include <aex/AWindow.hpp>
#include <aex/GraphicsDevice.hpp>
#include <aex/Exceptions.hpp>
#include "../../../include/internal_sdl.hpp"
#include <cds/container/msqueue.h>
#include <cds/algo/atomic.h>
#include <map>
#include <set>
#include <algorithm>
#if __TARGET_PLATFORM__ == __ANDROID_OS__
#include <EGL/egl.h>
#endif

namespace aex
{
    /**
     * @brief Defines supported buffers configuration for platform.
     */
    struct SupportedGLBuffersConfig
    {
        /**
         * @brief Red channel size in bits.
         */
        const int red_size;
        /**
         * @brief Green channel size in bits.
         */
        const int green_size;
        /**
         * @brief Blue channel size in bits.
         */
        const int blue_size;
        /**
         * @brief Alpha channel size in bits.
         */
        const int alpha_size;
        /**
         * @brief Depth buffer size in bits.
         */
        const int depth_size;
        /**
         * @brief Stencil buffer size in bits.
         */
        const int stencil_size;
        /**
         * @brief Enable/disable double buffering.
         */
        const int double_buffer;
        /**
         * @brief Number of multisample buffers.
         */
        const int multisample_buffers;
    };

    /**
     * @brief Profile supported on platform.
     */
    struct SupportedGLProfile
    {
        /**
         * @brief OpenGL Profile (GL Core, GL Compatibility, GL ES).
         */
        const int profile;
        /**
         * @brief Profile major version.
         */
        const int profile_major_version;
        /**
         * @brief Profile minor version.
         */
        const int profile_minor_version;
    };


    /**
     * @brief Buffer profile pairs that should be tried.
     */
    struct SupportedGLConfig
    {
        /**
         * @brief Buffers config.
         */
        const SupportedGLBuffersConfig buffers_config;
        /**
         * @brief GL profile.
         */
        const SupportedGLProfile profile;
    };




    // Choose appropriate GL depending on OS
#if __TARGET_AEX_API__ == __GLES__
    //LOG("OpenGL ES");

    static CONSTEXPR SupportedGLBuffersConfig default_buffer_config = SupportedGLBuffersConfig{5, 6, 5, 0, 16, 8, -1, -1};

    static CONSTEXPR SupportedGLProfile GLES_2_0_PROFILE = SupportedGLProfile{SDL_GL_CONTEXT_PROFILE_ES, 2, 0};
#if defined BUILD_FOR_GLES3
    static CONSTEXPR SupportedGLProfile GLES_3_0_PROFILE = SupportedGLProfile{SDL_GL_CONTEXT_PROFILE_ES, 3, 0};
    //static CONSTEXPR SupportedGLConfig DEFAULT_GLES_3_0_CONFIG = SupportedGLConfig{default_buffer_config, GLES_3_0_PROFILE};
    static CONSTEXPR SupportedGLBuffersConfig default_doublebuffer_config = SupportedGLBuffersConfig{5, 6, 5, 0, 16, 8, 1, -1};
    static CONSTEXPR SupportedGLBuffersConfig RGBA8888_buffer_config = SupportedGLBuffersConfig{8, 8, 8, 8, 16, 8, -1, -1};
    static CONSTEXPR SupportedGLBuffersConfig RGBA8888_doublebuffer_config = SupportedGLBuffersConfig{8, 8, 8, 8, 16, 8, 1, -1};

    static CONSTEXPR std::array<SupportedGLConfig, 5> supported_gl_configs = std::array<SupportedGLConfig, 5>{{
            {default_buffer_config,          GLES_3_0_PROFILE},
            {default_doublebuffer_config,    GLES_3_0_PROFILE},
            {RGBA8888_buffer_config,         GLES_3_0_PROFILE},
            {RGBA8888_doublebuffer_config,   GLES_3_0_PROFILE},
            {default_buffer_config,          GLES_2_0_PROFILE}
        }};
#else // GLES2

    //static CONSTEXPR SupportedGLConfig DEFAULT_GLES_2_0_CONFIG = SupportedGLConfig{default_buffer_config, GLES_2_0_PROFILE};

       static CONSTEXPR std::array<SupportedGLConfig, 1> supported_gl_configs = std::array<SupportedGLConfig, 1>{{
            {default_buffer_config,          GLES_2_0_PROFILE}
        }};
#endif

#elif __TARGET_AEX_API__ == __GL__
    //LOG("OpenGL");
    static CONSTEXPR SupportedGLProfile COMPATIBILITY_3_1_PROFILE = SupportedGLProfile{SDL_GL_CONTEXT_PROFILE_COMPATIBILITY, 3, 1};
    static CONSTEXPR SupportedGLProfile COMPATIBILITY_3_0_PROFILE = SupportedGLProfile{SDL_GL_CONTEXT_PROFILE_COMPATIBILITY, 3, 0};
    static CONSTEXPR SupportedGLProfile COMPATIBILITY_2_1_PROFILE = SupportedGLProfile{SDL_GL_CONTEXT_PROFILE_COMPATIBILITY, 2, 1};
    static CONSTEXPR SupportedGLProfile COMPATIBILITY_2_0_PROFILE = SupportedGLProfile{SDL_GL_CONTEXT_PROFILE_COMPATIBILITY, 2, 0};

    static CONSTEXPR SupportedGLBuffersConfig default_doublebuffer_config = SupportedGLBuffersConfig{5, 6, 5, 0, 16, 8, 1, -1};
//    static CONSTEXPR SupportedGLBuffersConfig RGBA8888_buffer_config = SupportedGLBuffersConfig{8, 8, 8, 8, 16, 8, -1, -1};
//    static CONSTEXPR SupportedGLBuffersConfig RGBA8888_doublebuffer_config = SupportedGLBuffersConfig{8, 8, 8, 8, 16, 8, 1, -1};


    static CONSTEXPR std::array<SupportedGLConfig, 4> supported_gl_configs{{
                        { default_doublebuffer_config, COMPATIBILITY_3_1_PROFILE },
			{ default_doublebuffer_config, COMPATIBILITY_3_0_PROFILE },
			{ default_doublebuffer_config, COMPATIBILITY_2_1_PROFILE },
			{ default_doublebuffer_config, COMPATIBILITY_2_0_PROFILE }
		}};

//    static CONSTEXPR SupportedGLProfile CORE_3_1_PROFILE = SupportedGLProfile{SDL_GL_CONTEXT_PROFILE_CORE, 3, 1};
//    static CONSTEXPR SupportedGLProfile CORE_3_0_PROFILE = SupportedGLProfile{SDL_GL_CONTEXT_PROFILE_CORE, 3, 0};
//    static CONSTEXPR SupportedGLProfile CORE_2_1_PROFILE = SupportedGLProfile{SDL_GL_CONTEXT_PROFILE_CORE, 2, 1};
//    static CONSTEXPR SupportedGLProfile CORE_2_0_PROFILE = SupportedGLProfile{SDL_GL_CONTEXT_PROFILE_CORE, 2, 0};

#endif

    void set_sdl_gl_attribute(SDL_GLattr attr, int val)
    {
        if(val < 0)
            return;
        SDL_GL_SetAttribute(attr, val);
    }

    using api_detail::DataFormatCommon;
    using api_detail::PrimitiveType;

    // Static data
    CONSTEXPR const std::size_t LOOKUP_SIZE = 4;
    bool windowResized = false;
    ThreadLocalStorage<aex_context>* CONTEXT = nullptr;

    // Local typedefs
    typedef std::pair<std::thread::id, aex_context*> context_lookup_t;
    typedef std::array<context_lookup_t, LOOKUP_SIZE> context_lookup_array_t;
    template<class T>
    using queue_type = cds::container::MSQueue<cds::gc::HP,
        T,
        typename cds::container::msqueue::make_traits<
            cds::opt::stat<cds::container::msqueue::stat<>>,
            cds::opt::item_counter<cds::atomicity::item_counter>
        >::type
    >;
    using age_queue_t = queue_type<size_t>;

    context_lookup_array_t FastLookup;
    std::unique_ptr<age_queue_t> LookupAge;
    GLContextGC::GLContextGC() {
        LookupAge.reset(new age_queue_t());
    }
    GLContextGC::~GLContextGC() {
        LookupAge.reset();
    }

    // Forward declarations
    static void initArrays(aex_context& ctx);
    static void initTextures(aex_context& ctx);
    static void initFramebuffers(aex_context& ctx);
    static void __enableStateCTX(aex_context& ctx, uint32_t capability);
    static void __disableStateCTX(aex_context& ctx, uint32_t capability);
    static void __setBlendStateCTX(aex_context& ctx, const BlendState& bState);
    static void __setDepthStateCTX(aex_context& ctx, const DepthState& dState);
    static void __colorMaskCtx(aex_context& ctx, bool r, bool g, bool b, bool a);
    static void __clearColorCtx(aex_context& ctx, float r, float g, float b, float a);
    static void __clearContext(aex_context& ctx);
    static void* makeGLContext(void* sdl_window);
    static GLuint defaultFbo();

    namespace gl_context_detail
    {
        void
            TrackedQueue::push(GLCapabilitiesWrapper* newElement)
            {
                if(newElement)
                    newElement->mQueued = true;
                BaseQueue::push(newElement);
            }

        void
            TrackedQueue::pop()
            {
                if(empty()) return;
                if(front())
                    front()->mQueued = false;
                BaseQueue::pop();
            }
    } // end of namespace gl_context_detail

    // Default GL init values.
    //void glInitParams()
    //{
    //    pickGlVer();
    //    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    //    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
    //    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    //    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 0);
    //    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    //    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    //    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    //}
    void glInitParams(const SupportedGLConfig& config)
    {
        set_sdl_gl_attribute(SDL_GL_CONTEXT_PROFILE_MASK, config.profile.profile);
        set_sdl_gl_attribute(SDL_GL_CONTEXT_MAJOR_VERSION, config.profile.profile_major_version);
        set_sdl_gl_attribute(SDL_GL_CONTEXT_MINOR_VERSION, config.profile.profile_minor_version);
        set_sdl_gl_attribute(SDL_GL_RED_SIZE, config.buffers_config.red_size);
        set_sdl_gl_attribute(SDL_GL_GREEN_SIZE, config.buffers_config.green_size);
        set_sdl_gl_attribute(SDL_GL_BLUE_SIZE, config.buffers_config.blue_size);
        set_sdl_gl_attribute(SDL_GL_ALPHA_SIZE, config.buffers_config.alpha_size);
        set_sdl_gl_attribute(SDL_GL_DEPTH_SIZE, config.buffers_config.depth_size);
        set_sdl_gl_attribute(SDL_GL_STENCIL_SIZE, config.buffers_config.blue_size);
        set_sdl_gl_attribute(SDL_GL_ACCELERATED_VISUAL, 1);
    }

    // Choose appropriate GL depending on OS
    //#if __TARGET_AEX_API__ == __GLES__
    //void pickGlVer()
    //{
    //    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    //    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    //    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    //}
    //#endif
    //#if __TARGET_AEX_API__ == __GL__
    //#   if __TARGET_PLATFORM__ == __APPLE_OSX__
    //void pickGlVer()
    //{
    //    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    //    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    //    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    //    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    //}
    //#   else
    //void pickGlVer()
    //{
    //    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    //    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    //    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    //}
    //#   endif
    //#endif

    /**
     * @brief Try to create GL Context using available configs.
     *
     * @tparam SIZE Number of available configs.
     * @param configs Available configs.
     * @param sdl_window Valid SDL_Window.
     *
     * @return Context on succes, nullptr on failure.
     */

    static const std::string& profile_to_string(int p)
    {
        static const std::string SDL_GL_CONTEXT_PROFILE_CORE_STRING = "SDL_GL_CONTEXT_PROFILE_CORE";
        static const std::string SDL_GL_CONTEXT_PROFILE_COMPATIBILITY_STRING = "SDL_GL_CONTEXT_PROFILE_COMPATIBILITY";
        static const std::string SDL_GL_CONTEXT_PROFILE_ES_STRING = "SDL_GL_CONTEXT_PROFILE_ES";
        static const std::string BAD_CONTEXT_STRING = "BAD_CONTEXT";

        switch(p)
        {
            case SDL_GL_CONTEXT_PROFILE_CORE:
                return SDL_GL_CONTEXT_PROFILE_CORE_STRING;

            case SDL_GL_CONTEXT_PROFILE_COMPATIBILITY:
                return SDL_GL_CONTEXT_PROFILE_COMPATIBILITY_STRING;

            case SDL_GL_CONTEXT_PROFILE_ES:
                return SDL_GL_CONTEXT_PROFILE_ES_STRING;
            default:
                return BAD_CONTEXT_STRING;
        }
    }

    template<size_t SIZE>
        void* try_create_context(const std::array<SupportedGLConfig, SIZE>& configs, void* sdl_window)
        {
            void* context = nullptr;

            for(size_t i = 0; !context && i < SIZE; i++)
            {
                glInitParams(configs[i]);
                context = SDL_GL_CreateContext(static_cast<SDL_Window*>(sdl_window));
                if(context)
                {
                    const std::string& profile = profile_to_string(configs[i].profile.profile);
                    LOG("Context created: ",
                            profile, ", ",
                            configs[i].profile.profile_major_version, ".",
                            configs[i].profile.profile_minor_version
                        );
                    LOG("     ","RGBA size: ",  configs[i].buffers_config.red_size,configs[i].buffers_config.green_size,configs[i].buffers_config.red_size,configs[i].buffers_config.alpha_size, ", ",
                            "depth size: " ,configs[i].buffers_config.depth_size, ", ",
                            "stencil size: " ,configs[i].buffers_config.stencil_size);
                }
                else
                {
                    const std::string& profile = profile_to_string(configs[i].profile.profile);
                    LOG("Could not create context(profile): ",profile, " ,",
                           configs[i].profile.profile_major_version, ".", configs[i].profile.profile_minor_version,
                           ": ", SDL_GetError());
                }
            }

            return context;
        }

    // Create GL context.
    void* makeGLContext(void* sdl_window)
    {
        void* context = try_create_context(supported_gl_configs, static_cast<SDL_Window*>(sdl_window));

        if(!context)
        {
            const char* err = SDL_GetError();
            LOG_ERR(err);
            throw FailedToCreateWindow
            {
                err
            };
        }

#ifdef GLEW
        glewInit();
#endif
		const GLubyte* extensions = glGetString(GL_EXTENSIONS);
        if(extensions != nullptr)
            LOG("Available extensions: ",reinterpret_cast<const char*>(extensions), ".");

        LOG("End of extensions.");
        SDL_GL_SetSwapInterval(0);

        return context;
    }

    aex_context::aex_context(AWindow& window) :
        SDLWindow(nullptr),
        GlContext(nullptr),
        ArrayBufferBound{0},
        IndexBufferInfo {0, GL_TRIANGLES},
        IndexBufferBound{0},
        ArrayBufferUnbound{true},
        IndexBufferUnbound{true},
        VertexAttribArrays {},
        CurrentTextureUnit{},
        ShaderProgramState {0},
        MaxVertexAttribs {0},
        DefaultFbo{nullptr},
        VCurrentFbo{nullptr},
        RCurrentFbo{0},
        ctxClearColor(0.0f, 0.0f, 0.0f, 0.0f),
        PendingDisables {},
        UncommitedChanges {},
        Capabilities{{
            CapabilityEntry{GL_BLEND},
            CapabilityEntry{GL_STENCIL_TEST},
            CapabilityEntry{GL_DEPTH_TEST},
            CapabilityEntry{GL_CULL_FACE},
            CapabilityEntry{GL_DITHER},
            CapabilityEntry{GL_POLYGON_OFFSET_FILL},
            CapabilityEntry{GL_SAMPLE_ALPHA_TO_COVERAGE},
            CapabilityEntry{GL_SAMPLE_COVERAGE},
            CapabilityEntry{GL_SCISSOR_TEST}
        }},
        mWindowHidden(window.addHideWindowListener(boost::bind(&aex_context::onWindowHidden, this))),
        mWindowShown(window.addShowWindowListener(boost::bind(&aex_context::onWindowShown, this))),
        mWindowResize(window.addWindowSizeChange(boost::bind(&aex_context::onWindowResize, this, _1, _2))),
        Initialized{false},
        windowState{1},
        inFrame{false},
        waitingForEnd{0}
    {
        LOG_DEBUG("Context creataed.");
        clearCommit.mCommitFunc = [this]()
        {

            GL_CHECK(glClear((GL_COLOR_BUFFER_BIT & ((~(unsigned int) ctxClearBuffers.mClearColor) + 1)) |
                        (GL_DEPTH_BUFFER_BIT & ((~(unsigned int) ctxClearBuffers.mClearDepth) + 1)) |
                        (GL_STENCIL_BUFFER_BIT & ((~(unsigned int) ctxClearBuffers.mClearStencil) + 1))));
            ctxClearBuffers.mClearColor = false;
            ctxClearBuffers.mClearDepth = false;
            ctxClearBuffers.mClearStencil = false;
        };

        colorMaskCommit.mCommitFunc = [this]()
        {
            GL_CHECK(glColorMask(ctxColorMask.r, ctxColorMask.g, ctxColorMask.b, ctxColorMask.a));
        };
    }

    aex_context::~aex_context()
    {
        if(DefaultFbo)
            DefaultFbo->onContextDirty();

        if(GlContext)
        {
            SDL_GL_DeleteContext(GlContext);
        }

        LOG_DEBUG("Context Destruction.");
    }

    aex_context* __context_pointer()
    {
        if(!CONTEXT)
            throw UninitializedContextError("Context is not initialized. Did you call ::aex::context_init?");
        aex_context* ctx = nullptr;

        //
        // Small lookup table to keep at most 4 contexts for quick dereference.
        //
        // Allows quick check skipping thread local storage access which is quite expensive.
        //

        for(std::size_t ii = 0; ii < 4; ii++)
        {
            if(FastLookup[ii].first == std::this_thread::get_id())
            {
                ctx = FastLookup[ii].second;
                break;
            }
        }

        if(ctx == nullptr && CONTEXT != nullptr)
        {
            std::size_t lookupIdx = 0;
            std::size_t backOff = 0;
            ctx = &(**CONTEXT);

            //
            // Queue of free indexes in lookup table.
            //
            // TODO: Actually freeing them when context is destroyed.
            //
            while(!LookupAge->pop(lookupIdx) && backOff++ < 10)
            {
            } //Spin until index

            if(backOff < 10)
            {
                //If we don't get free index in 10 tries just skip.
                FastLookup[lookupIdx].first = std::this_thread::get_id();
                FastLookup[lookupIdx].second = ctx;
                LookupAge->push(lookupIdx);
            }
        }

        return ctx;
    }

    aex_context& __context()
    {
        return *__context_pointer();
    }

    aex_context& unsafeGetCurrentContext()
    {
        aex_context& ctx = __context();

        if(!ctx.Initialized)
        {
            throw UninitializedContextError("Uninitialized context. Possibly wrong thread?");
        }

        return ctx;
    }

    aex_context& getCurrentContext()
    {
        aex_context& ctx = unsafeGetCurrentContext();
        switch(ctx.windowState)
        {
            case aex_context::HIDDEN:
                {
//                    throw WindowHidden{__FILE__, __LINE__};
                }
            case aex_context::CHECK_CONTEXT:
                {
                    if(ctx.GlContext != SDL_GL_GetCurrentContext())
                    {
                        __clearContext(ctx);
                        ctx.windowState = aex_context::SHOWN;
                    }
                    break;
                }
            case aex_context::CHECK_VIEWPORT:
                {
#if __TARGET_PLATFORM__ == __ANDROID_OS__
                    int sw, sh;
                    auto disp = eglGetCurrentDisplay();
                    auto surf = eglGetCurrentSurface(EGL_DRAW);
                    eglQuerySurface(disp, surf,  EGL_WIDTH, &sw);
                    eglQuerySurface(disp, surf,  EGL_HEIGHT, &sh);
                    ctx.viewPortChange(sw, sh, ctx.CurrentViewPort.x, ctx.CurrentViewPort.y);
#endif
                    ctx.windowState = aex_context::SHOWN;
                    break;
                }
            case aex_context::SHOWN:
            default:
                {
                    break;
                }
        }
        return ctx;
    }

    ContextObject::ContextObject()
    {
        aex_context& ctx = __context();
        ctx.m_contextObjects.insert(this);
    }
    ContextObject::~ContextObject()
    {
        aex_context& ctx = __context();
        ctx.m_contextObjects.erase(this);
    }

    void __clearContext(aex_context& ctx)
    {
        for(auto& ctxObject : ctx.m_contextObjects)
            ctxObject->onContextDirty();
        while(!ctx.PendingDisables.empty()) ctx.PendingDisables.pop();
        while(!ctx.UncommitedChanges.empty()) ctx.UncommitedChanges.pop();
        ctx.IndexBufferInfo = IndexBuffer{0, GL_TRIANGLES};
        ctx.IndexBufferBound = nullptr;
        ctx.ArrayBufferBound = nullptr;
        ctx.ShaderProgramState = {0};
        ViewPort vp = ctx.CurrentViewPort;
        ctx.CurrentViewPort = {0, 0, 0, 0};
        ctx.viewPortChange(vp.w, vp.h, vp.x, vp.y);
        BlendState bState = ctx.ctxBlendState;
        ctx.ctxBlendState = BlendState{};
        __setBlendStateCTX(ctx, bState);
        DepthState dState = ctx.ctxDepthState;
        ctx.ctxDepthState = DepthState{};
        __setDepthStateCTX(ctx, dState);
        ColorMask cMask = ctx.ctxColorMask;
        ctx.ctxColorMask = ColorMask{};
        __colorMaskCtx(ctx, cMask.r, cMask.g, cMask.b, cMask.a);
        gl_context_detail::Color clearColor = ctx.ctxClearColor;
        ctx.ctxClearColor = gl_context_detail::Color{-1.0f, 0.0f, 0.0f, 0.0f};
        __clearColorCtx(ctx, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    ctx.Capabilities = std::array<CapabilityEntry, 9>{{
                CapabilityEntry{GL_BLEND},
                CapabilityEntry{GL_STENCIL_TEST},
                CapabilityEntry{GL_DEPTH_TEST},
                CapabilityEntry{GL_CULL_FACE},
                CapabilityEntry{GL_DITHER},
                CapabilityEntry{GL_POLYGON_OFFSET_FILL},
                CapabilityEntry{GL_SAMPLE_ALPHA_TO_COVERAGE},
                CapabilityEntry{GL_SAMPLE_COVERAGE},
                CapabilityEntry{GL_SCISSOR_TEST}
        }};

        ctx.RCurrentFbo = 0;
        ctx.ArrayBufferUnbound = true;
        ctx.IndexBufferUnbound = true;
        ctx.Initialized = true;

        initFramebuffers(ctx);
        initArrays(ctx);
        initTextures(ctx);
        ctx.GlContext = SDL_GL_GetCurrentContext();
        ctx.mContextDirty();
    }

    void context_destroy()
    {
        LOG_DEBUG("Aex context destroy.");
        delete CONTEXT;
        CONTEXT = nullptr;
    }

    void context_init(AWindow& window)
    {
        if(CONTEXT == nullptr)
        {
            CONTEXT = new ThreadLocalStorage<aex_context>(window);
            FastLookup = context_lookup_array_t {};

            for(std::size_t ii = 0; ii < LOOKUP_SIZE; ii++)
            {
                FastLookup[ii] = context_lookup_t {std::thread::id
                    {}, nullptr};
                LookupAge->push(ii);
            }
        }

        if(!__context_pointer())
        {
            CONTEXT->init(window);
        }
        aex_context& ctx = __context();

        if(ctx.Initialized)
        {
            LOG_DEBUG("context already initialized: ", &ctx);
            return;
        }
        ctx.Initialized = true;
    }

    static bool checkVideoDevice(void* device) // Wrap device in internal and carry window type.
    {
        return device != nullptr;
    }

    void setVideoDevice(void* device)
    {
        auto& ctx = getCurrentContext();
        if(!checkVideoDevice(device))
        {
            throw aex::ContextError("Bad video device.");
        }
        ctx.SDLWindow = device;
        ctx.GlContext = makeGLContext(device);
        GLint vp[4];
        GL_CHECK(glGetIntegerv(GL_VIEWPORT, vp));
        ctx.CurrentViewPort = ViewPort {vp[2], vp[3], vp[0], vp[1]};
        initFramebuffers(ctx);
        initArrays(ctx);
        initTextures(ctx);
    }

    bool wasContextInit()
    {
        if(CONTEXT == nullptr)
        {
            return false;
        }

        aex_context* ctx = __context_pointer();
        return ctx && ctx->Initialized;
    }

    void setIndexBuffer(const IndexBuffer& ib)
    {
        context_guard ctxGuard {};
        ctxGuard([](aex_context & ctx, const IndexBuffer & ib)
                {
                ctx.IndexBufferInfo.type = ib.type;
                ctx.IndexBufferInfo.count = ib.count;
                }, ib);
    }

    bool setCurrentProgram(uint32_t prog)
    {
        aex_context& ctx = unsafeGetCurrentContext();

        if(ctx.ShaderProgramState != prog)
        {
            ctx.ShaderProgramState = prog;
            return true;
        }

        return false;
    }

    IndexBuffer getIndexBuffer(aex_context& ctx)
    {
        return ctx.IndexBufferInfo;
    }

    void
        aex_context::viewPortChange(int32_t w, int32_t h, int32_t x, int32_t y)
        {
            if(w != CurrentViewPort.w || h != CurrentViewPort.h || x != CurrentViewPort.x || y != CurrentViewPort.y)
            {
                CurrentViewPort = {w, h, x, y};
                viewPortChangeCommit.mCommitFunc = [w, h, x, y]()
                {
                    GL_CHECK(glViewport(x, y, w, h));
                };

                UncommitedChanges.push(&viewPortChangeCommit);
            }
        }

    void
        aex_context::clearBuffers()
        {
            if(!colorMaskCommit.mQueued)
            {
                commitChanges(*this);
                UncommitedChanges.push(&clearCommit);
            }
        }

    void aex_context::colorMaskBuffer()
    {
        colorMaskCommit.mQueued = true;
        UncommitedChanges.push(&colorMaskCommit);
    }

    void
        aex_context::clearColorBuffer()
        {
            ctxClearBuffers.mClearColor = true;
            clearBuffers();
        }

    void
        aex_context::clearDepthBuffer()
        {
            ctxClearBuffers.mClearDepth = true;
            clearBuffers();
        }

    void
        aex_context::clearStencilBuffer()
        {
            ctxClearBuffers.mClearStencil = true;
            clearBuffers();
        }

    template<class T = decltype(aex_context::waitingForEnd)>
        struct WaitGuard
        {
            explicit WaitGuard(T& t) : m_cnt(t){m_cnt++;}
            ~WaitGuard(){m_cnt--;};
            T& m_cnt;
        };
    //
    // All window state changes are delayed until frame is done.
    //
    void
        aex_context::onWindowHidden()
        {
            std::mutex mt;
            std::unique_lock<std::mutex> lk(mt);
            {
                WaitGuard<> wg(waitingForEnd);
                frameEnd.wait(lk, [this](){return inFrame == false;});
                windowState = aex_context::HIDDEN;
            }
            notifyHandled.notify_one();
        }

    void
        aex_context::onWindowShown()
        {
            std::mutex mt;
            std::unique_lock<std::mutex> lk(mt);
            {
                WaitGuard<> wg(waitingForEnd);
                frameEnd.wait(lk, [this](){return inFrame == false;});
                windowState = aex_context::CHECK_CONTEXT;
            }
            notifyHandled.notify_one();
        }

    void
        aex_context::onWindowResize(size_t w, size_t h)
        {
            UNUSED(w);
            UNUSED(h);
            std::mutex mt;
            std::unique_lock<std::mutex> lk(mt);
            {
                WaitGuard<> wg(waitingForEnd);
                frameEnd.wait(lk, [this](){return inFrame == false;});
                if(windowState == aex_context::SHOWN)
                    windowState = aex_context::CHECK_VIEWPORT;
            }
            notifyHandled.notify_one();
        }

    struct overload_int
    {

        overload_int() : att(4) { }
        int att;
    };
    static overload_int MaxVertexAttribs(aex_context& ctx);

    overload_int MaxVertexAttribs(aex_context& ctx)
    {
        overload_int attribs;
        attribs.att = ctx.MaxVertexAttribs;

        if(!attribs.att)
        {
            GL_CHECK(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attribs.att));
            ctx.MaxVertexAttribs = attribs.att;
        }

        return attribs;
    }
    int MaxVertexAttribs()
    {
        context_guard ctxGuard {};
        return ctxGuard([](aex_context & ctx)
                {
                return MaxVertexAttribs(ctx);
                }).att;
    }

    ViewPort CurrentViewport()
    {
        aex_context& ctx = unsafeGetCurrentContext();
        return ctx.CurrentViewPort;
    }

    void initArrays(aex_context& ctx)
    {
        int attribCap = MaxVertexAttribs(ctx).att;
        ctx.VertexAttribArrays.clear();
        ctx.VertexAttribArrays.resize(attribCap);
        LOG_DEBUG("MaxVertexAttribs: ", attribCap);

        std::fill(
                ctx.VertexAttribArrays.begin(),
                ctx.VertexAttribArrays.end(),
                AttribArrayState
                {
                    4, api_detail::PrimitiveType::FLOAT, 0, 0, 0, false, false, false
                }
                );
    }

    void setArray(uint32_t index, const api_detail::DataFormatCommon& f)
    {
        context_guard ctxGuard {};
        return ctxGuard([](aex_context & ctx, uint32_t index, const api_detail::DataFormatCommon & f)
                {
                if(index >= ctx.VertexAttribArrays.size())
                {
                return;    //MAGIC TODO!!!!!!!!!!!!!
                }

                AttribArrayState state = ctx.VertexAttribArrays[index];

                if(!ctx.ArrayBufferUnbound)
                {
                if(
                        state.buffer != ctx.ArrayBufferBound ||
                        state.size != f.count ||
                        state.type != f.type ||
                        state.stride != f.stride ||
                        state.offset != f.offset
                  )
                {
                state = AttribArrayState {f.count, f.type, f.stride, f.offset, ctx.ArrayBufferBound, false, true, false};
                setVertexAttribArray(index, state);
                }

                state.enabled = true;
                state.markedForDisable = false;

                if(!ctx.VertexAttribArrays[index].enabled && !ctx.VertexAttribArrays[index].markedForDisable)
                {
                    enableVertexAttribArray(index);
                }

                ctx.VertexAttribArrays[index] = state;
                }
                }, index, f);
    }

    void unsetArray(uint32_t index)
    {
        aex_context& ctx = unsafeGetCurrentContext();

        if(index > ctx.VertexAttribArrays.size())
        {
            return;
        }

        if(ctx.VertexAttribArrays[index].enabled)
        {
            //            GL_CHECK(glDisableVertexAttribArray(index));
            ctx.VertexAttribArrays[index].enabled = false;
            ctx.VertexAttribArrays[index].markedForDisable = true;
            ctx.PendingDisables.push(index);
        }
    }

    void doPendingDisables(aex_context& ctx)
    {
        while(!ctx.PendingDisables.empty())
        {
            uint32_t index = ctx.PendingDisables.front();
            ctx.PendingDisables.pop();

            if(ctx.VertexAttribArrays[index].markedForDisable)
            {
                disableVertexAttribArray(index);
                ctx.VertexAttribArrays[index].enabled = false;
                ctx.VertexAttribArrays[index].markedForDisable = false;
            }
        }
    }

    bool setArrayBuffer(const GLESBuffer* index)
    {
        aex_context& ctx = unsafeGetCurrentContext();

        if(ctx.ArrayBufferUnbound && index != 0)
        {
            ctx.ArrayBufferUnbound = false;
        }

        if(index != ctx.ArrayBufferBound)
        {
            ctx.ArrayBufferBound = index;
            return true;
        }

        return false;
    }

    void unsetArrayBuffer(const GLESBuffer* index)
    {
        aex_context& ctx = unsafeGetCurrentContext();

        if(index == ctx.ArrayBufferBound)
        {
            ctx.ArrayBufferUnbound = true;
        }
    }

    void bufferDeletedCTX(const GLESBuffer* index)
    {
        aex_context& ctx = unsafeGetCurrentContext();

        if(index == ctx.ArrayBufferBound && index != 0)
        {
            ctx.ArrayBufferUnbound = true;
            ctx.ArrayBufferBound = 0;
        }
    }

    bool setIndexBuffer(const GLESBuffer* index, const IndexBuffer& ib)
    {
        aex_context& ctx = unsafeGetCurrentContext();

        if(ctx.IndexBufferUnbound)
        {
            ctx.IndexBufferUnbound = false;
        }

        if(index != ctx.IndexBufferBound || ib.count != ctx.IndexBufferInfo.count || ib.type != ctx.IndexBufferInfo.type)
        {
            ctx.IndexBufferBound = index;
            ctx.IndexBufferInfo = ib;
            return true;
        }

        return false;
    }

    void unsetIndexBuffer(const GLESBuffer* index)
    {
        aex_context& ctx = unsafeGetCurrentContext();

        if(index == ctx.IndexBufferBound || index == 0)
        {
            ctx.IndexBufferUnbound = true;
            ctx.IndexBufferInfo = {0, GL_TRIANGLES};
        }
    }

    void deleteBuffer(const GLESBuffer* index)
    {
        context_guard ctxGuard {};
        return ctxGuard([](aex_context & ctx, const GLESBuffer * index)
                {
                if(ctx.ArrayBufferBound == index)
                {
                    ctx.ArrayBufferBound = 0;
                }

                if(ctx.IndexBufferBound == index)
                {
                    ctx.IndexBufferBound = 0;
                }
                }, index);
    }

    void CommitCapabilityChanges(aex_context& ctx)
    {
        while(!ctx.UncommitedChanges.empty())
        {
            if(*ctx.UncommitedChanges.front())
            {
                ctx.UncommitedChanges.front()->mCommitFunc();
            }

            ctx.UncommitedChanges.pop();
        }
    }

    void enableStateCTX(uint32_t capability)
    {
        if(capability >= UNSUPPORTED)
            throw std::runtime_error
            {
                "Invalid value."
            };

        context_guard ctxGuard {};
        return ctxGuard([](aex_context& ctx, uint32_t capability)
                {
                __enableStateCTX(ctx, capability);
                }, capability);
    }

    void __enableStateCTX(aex_context& ctx, uint32_t capability)
    {
        size_t n = 0;

        while(capability)
        {
            if(capability & 0x1)
            {
                _AEX_ASSERT_LOG(n < ctx.Capabilities.size());
                CapabilityEntry& c = ctx.Capabilities[n];

                if(!c.mIsEnabled && !c.mQueuedState)
                {
                    c.mQueuedState = true;
                    c.mCommit.mCommitFunc = [&c]()
                    {
                        c.mIsEnabled = true;
                        GL_CHECK(glEnable(c.mGlCapability));
                    };

                    ctx.UncommitedChanges.push(&c.mCommit);
                }
                else if(c.mIsEnabled && !c.mQueuedState)
                {
                    c.mQueuedState = true;
                    c.mCommit.mCommitFunc = []()
                    {
                    };
                }
            }

            capability >>= 1;
            n++;
        }
    }

    void disableStateCTX(uint32_t capability)
    {
        if(capability >= UNSUPPORTED)
            throw std::runtime_error
            {
                "Invalid value."
            };

        context_guard ctxGuard {};

        return ctxGuard([](aex_context & ctx, uint32_t capability)
                {
                    __disableStateCTX(ctx, capability);
                }, capability);
    }



    void __disableStateCTX(aex_context& ctx, uint32_t capability)
    {
        size_t n = 0;

        while(capability)
        {
            if(capability & 0x1)
            {
                _AEX_ASSERT_LOG(n < ctx.Capabilities.size());
                CapabilityEntry& c = ctx.Capabilities[n];

                if(c.mIsEnabled && c.mQueuedState)
                {
                    c.mQueuedState = false;
                    c.mCommit.mCommitFunc = [&c]()
                    {
                        c.mIsEnabled = false;
                        GL_CHECK(glDisable(c.mGlCapability));
                    };

                    ctx.UncommitedChanges.push(&c.mCommit);
                }
                else if(!c.mIsEnabled && c.mQueuedState)
                {
                    c.mQueuedState = false;
                    c.mCommit.mCommitFunc = []()
                    {
                    };
                }
            }

            capability >>= 1;
            n++;
        }
    }

    GLenum getGlBlendFunc(const uint32_t blendFunc)
    {
        GLenum rGlBlendFunc = GL_ZERO;

        switch(blendFunc)
        {
            case BLEND_ZERO:
                rGlBlendFunc = GL_ZERO;
                break;

            case BLEND_ONE:
                rGlBlendFunc = GL_ONE;
                break;

            case BLEND_SRC_COLOR:
                rGlBlendFunc = GL_SRC_COLOR;
                break;

            case BLEND_ONE_MINUS_SRC_COLOR:
                rGlBlendFunc = GL_ONE_MINUS_SRC_COLOR;
                break;

            case BLEND_DST_COLOR:
                rGlBlendFunc = GL_DST_COLOR;
                break;

            case BLEND_ONE_MINUS_DST_COLOR:
                rGlBlendFunc = GL_ONE_MINUS_DST_COLOR;
                break;

            case BLEND_SRC_ALPHA:
                rGlBlendFunc = GL_SRC_ALPHA;
                break;

            case BLEND_ONE_MINUS_SRC_ALPHA:
                rGlBlendFunc = GL_ONE_MINUS_SRC_ALPHA;
                break;

            case BLEND_DST_ALPHA:
                rGlBlendFunc = GL_DST_ALPHA;
                break;

            case BLEND_ONE_MINUS_DST_ALPHA:
                rGlBlendFunc = GL_ONE_MINUS_DST_ALPHA;
                break;

            case BLEND_SRC_ALPHA_SATURATE:
                rGlBlendFunc = GL_SRC_ALPHA_SATURATE;
                break;

            default:
                break;
        }

        return rGlBlendFunc;
    }

    GLenum getGlBlendEquation(const uint32_t blendFunc)
    {
        GLenum rGlBlendEquation = GL_FUNC_ADD;

        switch(blendFunc)
        {
            case BLEND_FUNC_ADD:
                rGlBlendEquation = GL_FUNC_ADD;
                break;

            case BLEND_FUNC_SUBTRACT:
                rGlBlendEquation = GL_FUNC_SUBTRACT;
                break;

            case BLEND_FUNC_REVSUBTRACT:
                rGlBlendEquation = GL_FUNC_REVERSE_SUBTRACT;
                break;

            default:
                break;
        }

        return rGlBlendEquation;
    }

    void setBlendStateCTX(const BlendState& bState)
    {
        context_guard ctxGuard {};
        return ctxGuard([](aex_context & ctx, const BlendState & bState)
                {
                __setBlendStateCTX(ctx, bState);
                }, bState);
    }
    void __setBlendStateCTX(aex_context& ctx, const BlendState& bState)
    {
        if(ctx.ctxBlendState != bState)
        {
            ctx.ctxBlendState = bState;
            GLenum glBlendEnums[6]
            {
                getGlBlendFunc(bState.mSrcColorBlend),
                getGlBlendFunc(bState.mDstColorBlend),
                getGlBlendEquation(bState.mColorBlendOp),
                getGlBlendFunc(bState.mSrcAlphaBlend),
                getGlBlendFunc(bState.mDstAlphaBlend),
                getGlBlendEquation(bState.mAlphaBlendOp)
            };
            GL_CHECK(glBlendFuncSeparate(glBlendEnums[0], glBlendEnums[1], glBlendEnums[3], glBlendEnums[4]));

            if(bState.mColorBlendOp != ctx.ctxBlendState.mColorBlendOp || bState.mAlphaBlendOp != ctx.ctxBlendState.mAlphaBlendOp)
            {
                GL_CHECK(glBlendEquationSeparate(glBlendEnums[2], glBlendEnums[5]));
            }

        }
    }

    const BlendState& getBlendStateCTX()
    {
        aex_context& ctx = unsafeGetCurrentContext();
        return ctx.ctxBlendState;
    }

    std::vector<Color> getPixelColorCTX(const IBox2D& box)
    {
        std::vector<Color> colors;
        int w = box[1].x();
        int h = box[1].y();
        colors.resize(w * h);
        GL_CHECK(glReadPixels(box[0].x(), box[0].y(), w, h, GL_RGBA, GL_UNSIGNED_BYTE, colors.data()));
        return colors;
    }

    std::vector<float> getPixelDepthCTX(const IBox2D& box)
    {
        std::vector<float> depth;
        int w = box[1].x() - box[0].x();
        int h = box[1].y() - box[0].y();
        depth.resize(w * h);
        GL_CHECK(glReadPixels(box[0].x(), box[0].y(), w, h, GL_DEPTH_COMPONENT, GL_FLOAT, depth.data()));
        return depth;
    }

    void setDepthStateCTX(const DepthState& dState)
    {
        context_guard ctxGuard {};
        return ctxGuard([](aex_context & ctx, const DepthState & dState)
                {
                    __setDepthStateCTX(ctx, dState);
                }, dState);
    }

    void __setDepthStateCTX(aex_context& ctx, const DepthState& dState)
    {
        if(dState.mDepthFunc != ctx.ctxDepthState.mDepthFunc)
        {
            ctx.ctxDepthState.mDepthFunc = dState.mDepthFunc;
            GLenum nGlDepthFunc = GL_LESS;

            switch(dState.mDepthFunc)
            {
                case DEPTH_FUNC_NEVER:
                    nGlDepthFunc = GL_NEVER;
                    break;

                case DEPTH_FUNC_LESS:
                    nGlDepthFunc = GL_LESS;
                    break;

                case DEPTH_FUNC_EQUAL:
                    nGlDepthFunc = GL_EQUAL;
                    break;

                case DEPTH_FUNC_LEQUAL:
                    nGlDepthFunc = GL_LEQUAL;
                    break;

                case DEPTH_FUNC_GREATER:
                    nGlDepthFunc = GL_GREATER;
                    break;

                case DEPTH_FUNC_NOT_EQUAL:
                    nGlDepthFunc = GL_NOTEQUAL;
                    break;

                case DEPTH_FUNC_GEQUAL:
                    nGlDepthFunc = GL_GEQUAL;
                    break;

                case DEPTH_FUNC_ALWAYS:
                    nGlDepthFunc = GL_ALWAYS;
                    break;

                default:
                    break;
            }

            GL_CHECK(glDepthFunc(nGlDepthFunc));
        }

        if(dState.mDepthMask != ctx.ctxDepthState.mDepthMask)
        {
            ctx.ctxDepthState.mDepthMask = dState.mDepthMask;
            GLboolean nGlDepthMask = GL_TRUE;

            switch(dState.mDepthMask)
            {
                case DEPTH_MASK_WRITE_ENABLED:
                    nGlDepthMask = GL_TRUE;
                    break;

                case DEPTH_MASK_WRITE_DISABLED:
                    nGlDepthMask = GL_FALSE;
                    break;

                default:
                    break;
            }

            GL_CHECK(glDepthMask(nGlDepthMask));
        }
    }

    const DepthState& getDepthStateCTX()
    {
        aex_context& ctx = unsafeGetCurrentContext();
        return ctx.ctxDepthState;
    }

    void clearColorCTX(float r, float g, float b, float a)
    {
        context_guard ctxGuard {};
        return ctxGuard([](aex_context & ctx, float r, float g, float b, float a)
                {
                __clearColorCtx(ctx, r, g, b, a);
                }, r, g, b, a);
    }

    void __clearColorCtx(aex_context& ctx, float r, float g, float b, float a)
    {
        if(r != ctx.ctxClearColor.r || g != ctx.ctxClearColor.b || b != ctx.ctxClearColor.b || a != ctx.ctxClearColor.a)
        {
            ctx.ctxClearColor = gl_context_detail::Color{r, g, b, a};
            GL_CHECK(glClearColor(r, g, b, a));
        }
    }

    void clearColorBufferCTX()
    {
        aex_context& ctx = unsafeGetCurrentContext();
        ctx.clearColorBuffer();
    }

    void clearDepthBufferCTX()
    {
        aex_context& ctx = unsafeGetCurrentContext();
        ctx.clearDepthBuffer();
    }

    void clearStencilBufferCTX()
    {
        aex_context& ctx = unsafeGetCurrentContext();
        ctx.clearStencilBuffer();
    }

    void colorMaskCTX(bool r, bool g, bool b, bool a)
    {
        aex_context& ctx = unsafeGetCurrentContext();
        __colorMaskCtx(ctx, r, g, b, a);
    }


    void __colorMaskCtx(aex_context& ctx, bool r, bool g, bool b, bool a)
    {
        ctx.ctxColorMask.setRGBA(r, g, b, a);
        ctx.colorMaskBuffer();
    }



    //*********************HANDLE WINDOW EVENTS****************/

    void setViewPort(int32_t w, int32_t h, int32_t x, int32_t y)
    {
        context_guard ctxGuard {};
        return ctxGuard([](aex_context & ctx, int32_t w, int32_t h, int32_t x, int32_t y)
                {
                ctx.viewPortChange(w, h, x, y);
                }, w, h, x, y);
    }

    void setScissorAreaCTX(int32_t w, int32_t h, int32_t x, int32_t y)
    {
        glScissor( x, y, w, h);
    }

    void cullFaceFrontCTX()
    {
        GL_CHECK(glCullFace(GL_FRONT));
    }

    void cullFaceBackCTX()
    {
        GL_CHECK(glCullFace(GL_BACK));
    }

    void cullFaceFrontAndBackCTX()
    {
        GL_CHECK(glCullFace(GL_FRONT_AND_BACK));
    }

    void stencilClearValueCTX(int32_t value)
    {
        GL_CHECK(glClearStencil(value));
    }

    GLint glChannelType(uint32_t dataType)
    {
#if defined(GL_LUMINANCE)
        const GLint channel_fallback = GL_LUMINANCE;
#elif defined(GL_RED)
        const GLint channel_fallback = GL_RED;
#else
        const GLint channel_fallback = GL_ALPHA;
#endif

        UNUSED(channel_fallback);
        switch(dataType)
        {


            case     DEPTH_COMPONENT        :
                    return GL_DEPTH_COMPONENT;
                    break;
            case     RED_CHANNEL            :
#if defined(GL_RED)
                    return GL_RED;
#else
                    return channel_fallback;
#endif
                    break;
            case     GREEN_CHANNEL          :
#if defined(GL_BLUE)
                    return GL_BLUE;
#else
                    return channel_fallback;
#endif
                    break;
            case     BLUE_CHANNEL           :
#if defined(GL_GREEN)
                    return GL_GREEN;
#else
                    return channel_fallback;
#endif
                    break;
            case     RGB_CHANNEL            :
                    return GL_RGB;
                    break;
            case     RGBA_CHANNEL           :
                    return GL_RGBA;
                    break;
            case     ALPHA_CHANNEL            :
                    return GL_ALPHA;
                    break;
            case     LUMINANCE_CHANNEL      :

#ifdef  GL_LUMINANCE                         //\/ \/ \/ \/ \/ \/ \/ Wtftftft
                     return GL_LUMINANCE;
#else
                    return GL_RED;
#endif
                    break;
            case     LUMINANCE_ALPHA_CHANNEL:
#ifdef  GL_LUMINANCE_ALPHA
                    return GL_LUMINANCE_ALPHA;
#else
                    return GL_ALPHA;
#endif

                    break;

#if __TARGET_AEX_API__ != __GLES__
            case     COLOR_INDEX            :
#   ifdef  GL_COLOR_INDEX
                    return GL_COLOR_INDEX;
#   else
                    return GL_RGB;
#   endif
                    break;
            case     STENCIL_INDEX          :
                    return GL_STENCIL_INDEX;
                    break;
            case     BGR_CHANNEL            :
                    return GL_BGR;
                    break;
            case     BGRA_CHANNEL           :
                    return GL_BGRA;
                    break;
#endif

            default :
                   return GL_RGBA;
                   break;
        }
    }

    GLint getDataType( uint32_t dataType)
    {
        GLint  gDataType = GL_UNSIGNED_BYTE;

        if(dataType == RGBA_CHANNEL || dataType == BGRA_CHANNEL)
        {
            gDataType = GL_UNSIGNED_BYTE;
        }
        else if(dataType == RGB_CHANNEL || dataType == BGR_CHANNEL)
        {
            gDataType = GL_UNSIGNED_BYTE;

        }
        else if(dataType == DEPTH_COMPONENT || dataType == RED_CHANNEL || dataType == GREEN_CHANNEL  || dataType == BLUE_CHANNEL || dataType == ALPHA_CHANNEL)
        {
            gDataType = GL_FLOAT;

        }
        else if(dataType == STENCIL_INDEX || dataType == COLOR_INDEX)
        {
            gDataType = GL_UNSIGNED_BYTE;
        }

        return gDataType;
    }

    GLint geTextType(uint32_t textureType,int  level)
    {
        switch (textureType) {

            case TEX_TYPE_2D:
                LOG_DEBUG("GL_TEXTURE_2D");
                return GL_TEXTURE_2D;
                break;
            case TEX_TYPE_CUBEMAP:
                LOG_DEBUG("TEX_TYPE_CUBEMAP");
                if (level == 0)
                {
                    return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
                } else if (level == 1)
                {
                    return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
                } else if (level == 2)
                {
                    return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
                } else if (level == 3)
                {
                    return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
                } else if (level == 4)
                {
                    return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
                } else if (level == 5)
                {
                    return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
                }
                return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
                break;
            case TEX_TYPE_3D:
                LOG_DEBUG("GL_TEXTURE_3D");
                #ifndef BUILD_FOR_GLES2
                    return GL_TEXTURE_3D;
                #else
                    return GL_TEXTURE_2D;
                #endif

                break;

            default :
                LOG_DEBUG("GL_TEXTURE_3D");
                  return GL_TEXTURE_2D;
                   break;
        }
        return GL_TEXTURE_2D;
    }

    void getTextureDataCTX(int target, int level, void* data,  uint32_t dataType, uint32_t texureType)
    {

 #if  __TARGET_PLATFORM__ != __ANDROID_OS__ && __TARGET_PLATFORM__ != __APPLE_IOS__
        //glGetTexImage(GLenum target,GLint level,GLenum format,GLenum type, GLvoid * pixels);
        GLint type= geTextType(texureType, level);
        GLint gDataType= getDataType(dataType);
        GLint bufferType = glChannelType(dataType);
        LOG_DEBUG("level: ",level," gDataType:", gDataType,", bufferType: ", bufferType);
        GL_CHECK(glGetTexImage(type, 0, GL_RGBA, gDataType, data));
#endif
    }

    void getPixelDataCTX(int xOffset, int yOffset, int xSize, int ySize, void* data,  uint32_t dataType)
    {

        GLint gDataType= getDataType(dataType);
        GLint  bufferType = glChannelType(dataType);
        LOG_DEBUG("gDataType:", gDataType,", bufferType: ", bufferType);
        //glReadPixels( GLint x, GLint y,GLsizei width, GLsizei height,GLenum format, GLenum type,GLvoid *pixels );
         GL_CHECK(glReadPixels(xOffset, yOffset, xSize, ySize, bufferType, gDataType, data));
    }

    void enableWireframeModeCtx()
    {
#if __TARGET_AEX_API__ == __GL__
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
    }

     void enableFillModeCtx()
    {
#if __TARGET_AEX_API__ == __GL__
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
    }

     void enablePointModeCtx()
    {
#if __TARGET_AEX_API__ == __GL__
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
#endif
    }

    //*********************HANDLE TEXTURES*******************/

    void initTextures(aex_context& ctx)
    {
        GLint max_tex_units = 0;
        GL_CHECK(glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_tex_units));
        ctx.MaxTexUnits = static_cast<GLint>(max_tex_units);
        ctx.TextureUnits.m2D.mTUs.clear();
        ctx.TextureUnits.mCubes.mTUs.clear();
        for(auto& tu : ctx.TextureUnits.mCustom)
            tu.second.mTUs.clear();
        ctx.TextureUnits.m2D.mTUs.resize(ctx.MaxTexUnits);

        std::fill(ctx.TextureUnits.m2D.mTUs.begin(), ctx.TextureUnits.m2D.mTUs.end(), GLTextureUnit
                {
                nullptr,
                });
        ctx.TextureUnits.mCubes.mTUs.resize(ctx.MaxTexUnits);

        std::fill(ctx.TextureUnits.mCubes.mTUs.begin(), ctx.TextureUnits.mCubes.mTUs.end(), GLTextureUnit
                {
                nullptr, false
                });

        GLint n = ctx.MaxTexUnits > 0 ? ctx.MaxTexUnits : 0;
        ctx.CurrentTextureUnit = 0;
        LOG_DEBUG("MaxTexUnits: ", ctx.MaxTexUnits);
        GLint m = n;
        GL_CHECK(glActiveTexture(GL_TEXTURE0));

        while(n)
        {
            GLint k = m - n;
            ctx.TextureUnits.m2D.mAvailable.push(k);
            ctx.TextureUnits.mCubes.mAvailable.push(k);
            //LOG_DEBUG(k);
            n--;
        }
    }

    void setTextureCTX(GLTexture* t)
    {
        context_guard ctxGuard {};
        return ctxGuard([](aex_context & ctx, GLTexture * t)
                {
                    std::vector<GLTextureUnit>* TU = nullptr;
                    std::queue<size_t>* AvailTUs = nullptr;
                    pickTexBuffers(ctx, t, &TU, &AvailTUs);

                    if(!isTextureActive(ctx, t, *TU))
                    {
                        setActiveTex(t, *AvailTUs, *TU);
                    }
                    else
                    {
                        size_t unit = t->mTURef;
                        (*TU)[unit].mSkip = true;
                        setTextureInUnit(ctx, t, unit, false);
                    }
                }, t);
    }

    void releaseTextureCTX(GLTexture* t)
    {
        context_guard ctxGuard;
        ctxGuard([](aex_context & ctx, GLTexture * t)
                {
                //            aex_context &ctx = unsafeGetCurrentContext();
                    std::vector<GLTextureUnit>* TU = nullptr;
                    std::queue<size_t>* AvailTUs = nullptr;
                    pickTexBuffers(ctx, t, &TU, &AvailTUs);

                    if(isTextureActive(ctx, t, *TU))
                    {
                        size_t unit = t->mTURef;
                        (*TU)[unit].mSkip = false;
                        (*AvailTUs).push(unit);
                    }
                }, t);
    }

    void destroyTextureCTX(GLTexture* t)
    {
        aex_context& ctx = unsafeGetCurrentContext();
        std::vector<GLTextureUnit>* TU = nullptr;
        std::queue<size_t>* AvailTUs = nullptr;
        pickTexBuffers(ctx, t, &TU, &AvailTUs);

        if(isTextureActive(ctx, t, *TU))
        {
            size_t unit = t->mTURef;
            (*TU)[unit].mSkip = false;
            (*AvailTUs).push(unit);
            (*TU)[unit].mTexture = nullptr;
        }
    }

    size_t MaxTextures()
    {
        aex_context& ctx = unsafeGetCurrentContext();
        return ctx.MaxTexUnits;
    }

    void initFramebuffers(aex_context& ctx)
    {
        if(ctx.DefaultFbo)
            delete ctx.DefaultFbo;
        ctx.DefaultFbo = new GLFramebuffer();
        ctx.DefaultFbo->id = defaultFbo();
        ctx.VCurrentFbo = ctx.DefaultFbo;
    }

    GLuint aex_gl_color_attachments[16] = {
                                    GL_COLOR_ATTACHMENT0,   GL_COLOR_ATTACHMENT0+1, GL_COLOR_ATTACHMENT0+2, GL_COLOR_ATTACHMENT0+3,
                                    GL_COLOR_ATTACHMENT0+4, GL_COLOR_ATTACHMENT0+5, GL_COLOR_ATTACHMENT0+6, GL_COLOR_ATTACHMENT0+7,
                                    GL_COLOR_ATTACHMENT0+8, GL_COLOR_ATTACHMENT0+9, GL_COLOR_ATTACHMENT0+10,GL_COLOR_ATTACHMENT0+11,
                                    GL_COLOR_ATTACHMENT0+12,GL_COLOR_ATTACHMENT0+13,GL_COLOR_ATTACHMENT0+14,GL_COLOR_ATTACHMENT0+15
                    };

    void setDrawBuffers(int size)
    {
        #ifdef GL_MAX_COLOR_ATTACHMENTS
        GL_CHECK(glDrawBuffers(size,  aex_gl_color_attachments));
        #else
        UNUSED(size);
        #endif
    }

    void setFramebuffer(const void* fbo)
    {
        aex_context& ctx = unsafeGetCurrentContext();
        const GLFramebuffer& cFbo = *static_cast<const GLFramebuffer*>(ctx.VCurrentFbo);
        const GLFramebuffer& nFbo = *static_cast<const GLFramebuffer*>(fbo);

        if(cFbo.id != nFbo.id)
        {
            //LOG_DEBUG(" nFbo.id: ", cFbo.id, " ", nFbo.id);
            ctx.VCurrentFbo = fbo;
        }
        commitChanges(ctx);
    }

    const void* getFramebuffer()
    {
        aex_context& ctx = unsafeGetCurrentContext();
        return ctx.VCurrentFbo;
    }

    void CheckFbo(aex_context& ctx)
    {
        const GLFramebuffer& cFbo = *static_cast<const GLFramebuffer*>(ctx.VCurrentFbo);

        if(cFbo.id != ctx.RCurrentFbo)
        {
            GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, cFbo.id));
            ctx.RCurrentFbo = cFbo.id;

        }

        if(cFbo.isCube)
        {

        }
    }

    void generateMipmapCTX(GLenum target)
    {
        context_guard ctxGuard {};
        return ctxGuard([](aex_context & ctx, GLenum target)
                {
                if(!ctx.generateMipmaps)  //Set-up during context init?
                {
#if defined( GLEW )
                ctx.generateMipmaps = __glewGenerateMipmap;
#elif __TARGET_AEX_API__ == __GLES__ || defined( GLEXT )
                ctx.generateMipmaps = &glGenerateMipmap;
#else
                char* extensions = (char*) glGetString(GL_EXTENSIONS);
                typedef void(APIENTRY * MIPMAP_FUNC)(GLenum);

                if(strstr(extensions, "GL_SGIS_generate_mipmap"))
                {
                    ctx.generateMipmaps = (MIPMAP_FUNC) SDL_GL_GetProcAddress("glGenerateMipmap");
                }

#endif
                }

                ctx.generateMipmaps(target);
                }, target);
    }

    GLuint defaultFbo()
    {
        GLint fbo;
        GL_CHECK(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo));
		//_AEX_ASSERT_LOG(fbo >= 0);
        return fbo;
    }

    void swapWindow()
    {
        context_guard ctxGuard;
        ctxGuard([](aex_context & ctx)
                {
                if(ctx.SDLWindow)
                SDL_GL_SwapWindow((SDL_Window*) ctx.SDLWindow);
                });
    }
    void commitChanges(aex_context& ctx)
    {
        if(ctx.windowState == aex_context::HIDDEN)
            return;
        CommitCapabilityChanges(ctx);
        doPendingDisables(ctx);
        CheckFbo(ctx);
    }
    boost::signals2::connection OnContextDirty(const OnContextDirtyType::slot_type& listener)
    {
        aex_context& ctx = getCurrentContext();
        return ctx.mContextDirty.connect(listener);
    }
    void beginContextFrame(aex_context& ctx)
    {
        ctx.inFrame = true;
    }
    void endContextFrame(aex_context& ctx)
    {
        ctx.inFrame = false;
        ctx.frameEnd.notify_all();
        std::unique_lock<std::mutex> lk(ctx.frameEndLock);
        ctx.notifyHandled.wait(lk, [&ctx](){return ctx.waitingForEnd == 0;});
    }

    class GLGraphicsDevice final : public IGraphicsDeviceAPI
    {
        public:
            GLGraphicsDevice()
            {
            }

            bool initAPI(AWindow* window, uint32_t enables, uint32_t disables)
            {
                UNUSED(window);
                enableStateCTX(enables);
                disableStateCTX(disables);
                return true;
            }

            void beginFrame()
            {
                beginFrame(COLOR_BUFFER | DEPTH_BUFFER | STENCIL_BUFFER);
            }

            void beginFrame(uint32_t buffers)
            {
                aex_context& ctx = unsafeGetCurrentContext();
                beginContextFrame(ctx);
                ctx.ctxClearBuffers.mClearColor = buffers & COLOR_BUFFER;
                ctx.ctxClearBuffers.mClearDepth = buffers & DEPTH_BUFFER;
                ctx.ctxClearBuffers.mClearStencil = buffers & STENCIL_BUFFER;
                ctx.clearBuffers();
            }

            void finishAPI()
            {

            }

            void endFrame()
            {
                aex_context& ctx = unsafeGetCurrentContext();
                swapWindow();
                endContextFrame(ctx);
            }
    };
    
    class GLGraphicsDeviceFactory final : public IGraphicsDeviceAPIFactory
    {
        public:
            virtual std::unique_ptr<IGraphicsDeviceAPI> create_device()
            {
                return std::unique_ptr<IGraphicsDeviceAPI>(new GLGraphicsDevice);
            }
    };

    std::unique_ptr<IGraphicsDeviceAPIFactory> get_device_factory()
    {
        return std::unique_ptr<IGraphicsDeviceAPIFactory>(new GLGraphicsDeviceFactory);
    }

    RENDERER_TYPE getRendererType()
    {
        char const * Renderer = reinterpret_cast<char const *> (glGetString(GL_RENDERER));
        if (strstr(Renderer, "Tegra") || strstr(Renderer, "GeForce") || strstr(Renderer, "NV"))
            return RENDERER_GEFORCE;
        else if (strstr(Renderer, "PowerVR") || strstr(Renderer, "Apple"))
            return RENDERER_POWERVR;
        else if (strstr(Renderer, "Mali"))
            return RENDERER_MALI;
        else if (strstr(Renderer, "Adreno"))
            return RENDERER_ADRENO;
        else if (strstr(Renderer, "AMD") || strstr(Renderer, "ATI"))
            return RENDERER_RADEON;
        else if (strstr(Renderer, "Intel"))
            return RENDERER_INTEL;
        else if (strstr(Renderer, "Vivante"))
            return RENDERER_VIVANTE;
        else if (strstr(Renderer, "VideoCore"))
            return RENDERER_VIDEOCORE;
        else if (strstr(Renderer, "WebKit") || strstr(Renderer, "Mozilla") || strstr(Renderer, "ANGLE"))
            return RENDERER_WEBGL;
        else return RENDERER_UNKNOWN;
    }
}
