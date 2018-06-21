#ifndef _AEX_EXCEPTIONS_HPP
#define _AEX_EXCEPTIONS_HPP

#include <exception>
#include <string>
#include <aex/Common.hpp>

namespace aex
{
    class AEX_PUBLIC_API BaseExcept : public std::runtime_error
    {
        public:
            explicit BaseExcept(const char* TAG, const aex::string& err_msg);
            explicit BaseExcept(const char* TAG, const char* err_msg = "");
            virtual ~BaseExcept();
    };

#define DECLARE_AEX_EXCEPTION(ExceptionName) class AEX_PUBLIC_API ExceptionName : public BaseExcept\
    {\
        static CONSTEXPR const char* TAG = #ExceptionName ;\
        public:\
               explicit ExceptionName(const aex::string& err_msg);\
               explicit ExceptionName(const char* err_msg = "");\
    }
#define DEFINE_AEX_EXCEPTION(ExceptionName) ExceptionName::ExceptionName(const aex::string& err_msg) : BaseExcept(TAG, err_msg){}\
    ExceptionName::ExceptionName(const char* err_msg) : BaseExcept(TAG, err_msg){}
    
    DECLARE_AEX_EXCEPTION(RenderHierarchyCorrupted);
    DECLARE_AEX_EXCEPTION(MathError);
    DECLARE_AEX_EXCEPTION(FloatingPointError);
    DECLARE_AEX_EXCEPTION(UninitializedContextError);
    DECLARE_AEX_EXCEPTION(InvalidBufferError);
    DECLARE_AEX_EXCEPTION(ContextError);
    DECLARE_AEX_EXCEPTION(MeshImporterError);
    DECLARE_AEX_EXCEPTION(AexolError);
    DECLARE_AEX_EXCEPTION(FileNotFoundError);
    DECLARE_AEX_EXCEPTION(JsonError);
    DECLARE_AEX_EXCEPTION(FontError);
    DECLARE_AEX_EXCEPTION(TTFError);
    DECLARE_AEX_EXCEPTION(NullptrError);
    DECLARE_AEX_EXCEPTION(UnsupportedEnumError);
    DECLARE_AEX_EXCEPTION(UnsignedIntegerOverflow);
    DECLARE_AEX_EXCEPTION(NegativeIntegerTo);
    
    class AEX_PUBLIC_API SDLErrorEx : public BaseExcept
    {
        static CONSTEXPR const char* TAG = "SDLErrorEx";
        public:
            SDLErrorEx();
            ~SDLErrorEx();
    };
}

#endif // _AEX_EXCEPTIONS_HPP
