/**************************************************************
 *                                                            * 
 *      Filename:   Dynload.cpp                               * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2016 mar 09 13:55:08                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#include <aex/Common.hpp>
#include <aex/Logger.hpp>
#include "../../include/DynloadFuncs.hpp"
#if __TARGET_AEX_API__ == __GLES__
#include "../../../include/internal_sdl.hpp"
#endif
#include <boost/filesystem.hpp>
#include <windows.h>

namespace aex
{
	using boost::filesystem::path;
	static HINSTANCE _handle;
	/* 
	 * @brief Append AexolGL.dll path to DLL search path.
	 */
	static void append_lib_dir_to_dynload_path()
	{
		LPTSTR _lpath = new TCHAR[2048];
		GetModuleFileName(_handle, _lpath, 2048);
		// Get index of the last path element before AexolGL.dll
		// and set it to null character.
		const int size = ::WideCharToMultiByte(CP_UTF8, 0, path(_lpath).parent_path().native().c_str(), -1, _lpath, 0, 0, NULL);
		_lpath[size] = 0;
		LOG_DEBUG("SetDllDirectory(\"", _lpath, "\")");
		SetDllDirectory(_lpath);
		delete[] _lpath;
	}
}
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
/* For python module dll load path
 * should be the same as the path of
 * AexolGL.dll.
 */
		aex::_handle = hinstDLL;
		aex::append_lib_dir_to_dynload_path();
		break;

	case DLL_PROCESS_DETACH:
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}