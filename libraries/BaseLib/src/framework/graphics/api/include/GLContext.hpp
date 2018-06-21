#ifndef __AEX_GLCONTEXT_HPP
#define __AEX_GLCONTEXT_HPP
#include "GLVertexAttribArrays.hpp"
#include <aex/Common.hpp>
#include <aex/Vector3.hpp>
#include <aex/Vector2.hpp>
#include <aex/api_definitions.hpp>
#include <aex/GPUApi.hpp>
#include <type_traits>
#include <boost/signals2.hpp>
#include <array>
#include <tuple>
#include <list>
#include <queue>
#include <exception>
#include <unordered_set>
#include <condition_variable>
#include <string>
#include <boost/signals2.hpp>

#ifndef GLEW
#   if __TARGET_PLATFORM__ == __AEX_WIN32__
#       ifdef GL_APIENTRY
#           ifndef GLAPIENTRY
#               define GLAPIENTRY GL_APIENTRY
#           endif // GLAPIENTRY
#       elif defined(APIENTRY) // GL_APIENTRY
#           ifndef GLAPIENTRY
#               define GLAPIENTRY APIENTRY
#           endif // GLAPIENTRY
#       else // GL_APIENTRY
#           if defined(__MINGW32__) || defined(__CYGWIN__) || (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__)
#               define AEX_APIENTRY
#               define APIENTRY __stdcall
#               ifndef GLAPIENTRY
#                   define __stdcall
#               endif //GLAPIENTRY
#           endif // defined(__MINGW32__) || defined(__CYGWIN__) || (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__)
#       endif // GL_APIENTRY
#   else // __TARGET_PLATFORM__ == __AEX_WIN32__
#       ifdef APIENTRY
#           ifndef GLAPIENTRY
#               define GLAPIENTRY APIENTRY
#           endif // GLAPIENTRY
#       endif // APIENTRY
#       ifndef GLAPIENTRY
#           ifdef GL_APIENTRY
#               define GLAPIENTRY GL_APIENTRY
#           else // GL_APIENTRY
#               define GLAPIENTRY
#           endif // GL_APIENTRY
#       endif // GLAPIENTRY
#   endif // __TARGET_PLATFORM__ == __AEX_WIN32__
#   ifdef AEX_APIENTRY
#       undef APIENTRY
#   endif // AEX_APIENTRY
#endif

namespace aex
{
class WindowHidden : public std::exception {
    public:
        WindowHidden(const char* file, int line) : m_msg(aex::string{"Uncaught window hidden at "}+file+":"+aex::to_string(line)){}
        const char* what() const NOEXCEPT {return m_msg.c_str();};
        const aex::string m_msg;
};
struct ContextObject;

namespace api_detail
{
struct DataFormatCommon;
}
struct Color;

/*****************COMMON****************/
template<class T, class S>
CONSTEXPR T enableIfEqual(const T& flagToEnable, const S& param1, const S& param2)
{
    return flagToEnable & (~static_cast<T>(param1 == param2) + 1);
}

template<class T, class S, class BinaryPredicate>
CONSTEXPR T enableIfEqual(const T& flagToEnable, const S& param1, const S& param2, BinaryPredicate p)
{
    return flagToEnable & (~static_cast<T>(p(param1, param2)) + 1);
}

class ShaderProgram;
class AexShaderParameterLocation;
class ShaderObject;
class VertexData;
class FramebufferObject;
class AWindow;
struct IndexBuffer;
struct aex_context;
typedef std::function<void(void)> GLCapabilitiesCommit;
class GLESBuffer;
struct GLFramebuffer;
struct GLTexture;

struct IndexBuffer
{
    std::size_t count;
    GLenum type;
};

enum RENDERER_TYPE
{
RENDERER_UNKNOWN,
RENDERER_ADRENO,
RENDERER_GEFORCE,
RENDERER_INTEL,
RENDERER_MALI,
RENDERER_POWERVR,
RENDERER_RADEON,
RENDERER_VIDEOCORE,
RENDERER_VIVANTE,
RENDERER_WEBGL
};

RENDERER_TYPE getRendererType();

struct GLCapabilitiesWrapper
{
    GLCapabilitiesCommit mCommitFunc;
    bool mQueued = false;

    bool operator()() const
    {
        return (bool)mCommitFunc;
    }

    operator bool() const
    {
        return (bool)mCommitFunc;
    }
};

namespace gl_context_detail
{
    typedef std::queue<GLCapabilitiesWrapper*> BaseQueue;
    class TrackedQueue : public BaseQueue
    {
        public:
            void push(GLCapabilitiesWrapper* newElement);
            void pop();
    };
    struct Color {
        union {
            struct {
                    float r, g, b, a;
            };
            float rgba[4];
        };
        Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a){}
    };
}
typedef gl_context_detail::TrackedQueue CommitArray;

struct CapabilityEntry
{
    GLCapabilitiesWrapper mCommit;
    GLenum mGlCapability;
    bool mIsEnabled;
    bool mQueuedState;

    explicit CapabilityEntry(GLenum glCapability) :
        mCommit(),
        mGlCapability(glCapability),
        mIsEnabled(false),
        mQueuedState(false)
    {
    }
};

struct ClearBuffers
{
    bool mClearColor;
    bool mClearDepth;
    bool mClearStencil;

    ClearBuffers() :
        mClearColor
    {
        false
    },
    mClearDepth {false},
                mClearStencil {false}
    {
    }
};

struct ColorMask
{
    bool r = true;
    bool g = true;
    bool b = true;
    bool a = true;

    void setRGBA(bool tr, bool tg, bool tb, bool ta)
    {
        r = tr;
        g = tg;
        b = tb;
        a = ta;
    }

};

class PendingCommands
{
public:
    virtual void operator()() = 0;
    virtual ~PendingCommands() = 0;
};
struct GLTextureUnit
{
    GLTexture* mTexture;
    bool mSkip;
};
struct TextureUnits
{
    std::queue<size_t> mAvailable;
    std::vector<GLTextureUnit> mTUs;
};
struct TextureUnitsState
{
    TextureUnits m2D;
    TextureUnits mCubes;
    std::map<GLenum, TextureUnits> mCustom;
};
inline PendingCommands::~PendingCommands() {}

struct aex_context
{
    typedef boost::signals2::signal<void(int32_t, int32_t)> ContextResizeSignalType;
    typedef void(GLAPIENTRY* MIPMAP_FUNC)(GLenum);
    void* SDLWindow;
    void* GlContext;
    /*
     * Buffer states
     */
    const GLESBuffer* ArrayBufferBound;
    IndexBuffer IndexBufferInfo;
    const GLESBuffer* IndexBufferBound;
    bool ArrayBufferUnbound;
    bool IndexBufferUnbound;

    /*
     * Vertex attrib arrays
     */
    std::vector<AttribArrayState> VertexAttribArrays;

    /*
     * Textures states
     */
    TextureUnitsState TextureUnits;
    size_t CurrentTextureUnit;
    size_t MaxTexUnits;

    /*
     * Program states
     */
    uint32_t ShaderProgramState;
    int32_t MaxVertexAttribs;

    /*
     * Framebuffer states
     */

    GLFramebuffer* DefaultFbo;
    const void* VCurrentFbo;
    GLuint RCurrentFbo;

    /*
     * Framebuffer cube current Side
     */



    /*
     * Machine states
     */
    BlendState ctxBlendState;
    DepthState ctxDepthState;
    ViewPort CurrentViewPort;
    ClearBuffers ctxClearBuffers;
    ColorMask ctxColorMask;
    gl_context_detail::Color ctxClearColor;


    /*
     * Uncommited changes to state
     */
    std::queue<uint32_t> PendingDisables;
    CommitArray UncommitedChanges;
    GLCapabilitiesWrapper viewPortChangeCommit;
    std::array<CapabilityEntry, 9> Capabilities;
    GLCapabilitiesWrapper clearCommit;
    GLCapabilitiesWrapper colorMaskCommit;

    MIPMAP_FUNC generateMipmaps = nullptr;
    /*
     * Handle window state change
     */
    boost::signals2::scoped_connection mWindowHidden;
    boost::signals2::scoped_connection mWindowShown;
    boost::signals2::scoped_connection mWindowResize;

    /*
     * Signal context destruction.
     */
    OnContextDirtyType mContextDirty;

    /*
     * Aex context state.
     */
    static CONSTEXPR uint32_t HIDDEN = 0;
    static CONSTEXPR uint32_t SHOWN = 1;
    static CONSTEXPR uint32_t CHECK_CONTEXT = 2;
    static CONSTEXPR uint32_t CHECK_VIEWPORT = 3;
    bool Initialized;
    uint32_t windowState;
    bool inFrame;
    uint32_t waitingForEnd;
    std::condition_variable frameEnd;
    std::condition_variable notifyHandled;
    std::mutex frameEndLock;
    /*
     * Ctor and dtor
     */
    explicit aex_context(AWindow& window);
    ~aex_context();

    void viewPortChange(int32_t w, int32_t h, int32_t x, int32_t y);
    void clearBuffers();
    void clearColorBuffer();
    void clearDepthBuffer();
    void clearStencilBuffer();
    void colorMaskBuffer();
    void onWindowHidden();
    void onWindowShown();
    void onWindowResize(size_t w, size_t h);
    std::unordered_set<ContextObject*> m_contextObjects;
};
aex_context& getCurrentContext();
void setIndexBuffer(const IndexBuffer& ib);
bool setCurrentProgram(uint32_t prog);
IndexBuffer getIndexBuffer(aex_context& ctx);
void setArray(uint32_t index, const api_detail::DataFormatCommon& f);
void unsetArray(uint32_t index);
bool setArrayBuffer(const GLESBuffer* index);
void unsetArrayBuffer(const GLESBuffer* index);
bool setIndexBuffer(const GLESBuffer* index, const IndexBuffer& ib);
void unsetIndexBuffer(const GLESBuffer* index = 0);
void deleteBuffer(const GLESBuffer*  index);
void setTextureCTX(GLTexture* t);
void releaseTextureCTX(GLTexture* t);
void destroyTextureCTX(GLTexture* t);
void bufferDeletedCTX(const GLESBuffer* index);
void generateMipmapCTX(GLenum target);
void commitChanges(aex_context& ctx);

template<class Func, class RType, class... Args>
class func_wrapper : public PendingCommands   //http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
{
    template<int ...>
    struct seq { };

    template<int N, int ...S>
    struct gens : gens < N - 1, N - 1, S... > { };

    template<int ...S>
    struct gens<0, S...>
    {
        typedef seq<S...> type;
    };
    RType delayed_dispatch()
    {
        return callFunc(typename gens<sizeof...(Args)>::type());
    }
public:
    func_wrapper(Func&& f, const Args& ... args) :
        mFunc {std::move(f)},
        mParams {args...}
    {
    }
    void operator()()
    {
        delayed_dispatch();
    }

    template<int ...S>
    RType callFunc(seq<S...>)
    {
        return mFunc(getCurrentContext(), std::get<S>(mParams) ...);
    }
    std::function<RType(aex_context&, Args...)> mFunc;
    std::tuple<Args...> mParams;
};

struct context_guard
{
    template<class RType>
    struct ReturnType
    {
        template<class Func, class... Args>
        ReturnType(Func&& f, Args&& ... args)
        {
            r = f(getCurrentContext(), std::forward<Args>(args)...);
        }
        RType operator()() const
        {
            return r;
        }
        RType r;
    };
    template<class Func, class... Args>
    auto operator()(Func&& f, Args&& ... args) -> decltype(f(getCurrentContext(), std::forward<Args>(args)...))
    {
        typedef decltype(f(getCurrentContext(), std::forward<Args>(args)...)) RType;

        try
        {
            auto retVal = ReturnType<RType> {f, std::forward<Args>(args)...};
            return retVal();
        }
        catch(WindowHidden& e)
        {
            return RType();
        }
    }
};
template<>
struct context_guard::ReturnType<void>
{
    template<class Func, class... Args>
    ReturnType(Func&& f, Args&& ... args)
    {
        f(getCurrentContext(), std::forward<Args>(args)...);
    }
    void operator()() {}
};

struct ContextObject{
    ContextObject();
    virtual ~ContextObject();
    virtual void onContextDirty() = 0;
};

struct GLContextGC {
    GLContextGC();
    ~GLContextGC();
};
}
#endif
