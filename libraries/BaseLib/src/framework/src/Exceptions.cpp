#include <aex/Exceptions.hpp>
#include <aex/Common.hpp>
#include "../include/internal_sdl.hpp"
#include <sstream>
#include <locale>
#include <string>

namespace aex
{
    static CONSTEXPR const char* separator = ": ";
    static std::string buildErrorMsg(const char* TAG, const aex::string& msg);

    BaseExcept::BaseExcept(const char* TAG, const aex::string& err_msg):
        BaseExcept(TAG, err_msg.c_str())
    {
    }

    BaseExcept::BaseExcept(const char* TAG, const char* err_msg):
        std::runtime_error(buildErrorMsg(TAG, err_msg))
    {
    }
    
    BaseExcept::~BaseExcept()
    {
         
    }

    DEFINE_AEX_EXCEPTION(MathError)
    DEFINE_AEX_EXCEPTION(FloatingPointError)
    DEFINE_AEX_EXCEPTION(UninitializedContextError)
    DEFINE_AEX_EXCEPTION(InvalidBufferError)
    DEFINE_AEX_EXCEPTION(ContextError)
    DEFINE_AEX_EXCEPTION(MeshImporterError)
    DEFINE_AEX_EXCEPTION(RenderHierarchyCorrupted)
    DEFINE_AEX_EXCEPTION(AexolError)
    DEFINE_AEX_EXCEPTION(FileNotFoundError)
    DEFINE_AEX_EXCEPTION(JsonError)
    DEFINE_AEX_EXCEPTION(FontError)
    DEFINE_AEX_EXCEPTION(TTFError)
    DEFINE_AEX_EXCEPTION(NullptrError)
    DEFINE_AEX_EXCEPTION(UnsupportedEnumError)
    DEFINE_AEX_EXCEPTION(UnsignedIntegerOverflow)
    
    SDLErrorEx::SDLErrorEx() : BaseExcept(TAG, getSDLError()){}
    
    SDLErrorEx::~SDLErrorEx()
    {
        SDL_ClearError();
    }


    static std::string buildErrorMsg(const char* TAG, const aex::string& msg)
    {
        std::stringstream ss;
        ss << TAG << separator;
        if(msg.empty())
            ss << "No error message.";
        else
            ss << msg;
        return ss.str();
    }
}
