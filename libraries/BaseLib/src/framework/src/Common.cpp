#include <aex/Common.hpp>
#include "../include/internal_sdl.hpp"
#include <string>
#include <aex/Exceptions.hpp>

namespace aex
{
    const char* getSDLError()
    {
        return SDL_GetError();
    }

    void throw_sdl_error()
    {
        
                throw SDLErrorEx();
    }
    
    std::string wstring_to_string(const std::wstring& wstr)
    {
        
#   ifdef AEX_HAS_CXX11_CODECVT
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.to_bytes(wstr);
#   else
        return aex::string( wstr.begin(), wstr.end() );
#   endif
        
    }
	std::tm localtime(const std::time_t* t)
	{
		std::tm _tm;
#if defined(_MSC_VER)
		::localtime_s(&_tm, t);
#else // defined(_MSC_VER)
		_tm = *std::localtime(t);
#endif // defined(_MSC_VER)
		return _tm;
	}
}
