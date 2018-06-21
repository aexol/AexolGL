//
//  platfrom_definitions.h
//  aexolGL
//
//  Created by Kamil Matysiewicz on 17.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#ifndef aexolGL_platfrom_definitions_h
#define aexolGL_platfrom_definitions_h

#include "aex-config.h"

#define __ANDROID_OS__  0
#define __APPLE_IOS__   1
#define __APPLE_OSX__   2
#define __AEX_WIN32__   3
#define __UNSUPPORTED__ 4

#define __TARGET_PLATFORM__ __UNSUPPORTED__

#ifdef  BUILD_FOR_ANDROID
#	undef __TARGET_PLATFORM__
#	define __TARGET_PLATFORM__ __ANDROID_OS__
#endif

#ifdef BUILD_FOR_IOS
#	undef __TARGET_PLATFORM__
#	define __TARGET_PLATFORM__ __APPLE_IOS__
#endif

#ifdef  BUILD_FOR_MAC
#	undef __TARGET_PLATFORM__
#	define __TARGET_PLATFORM__ __APPLE_OSX__
#endif

#if defined(BUILD_FOR_WIN32) || defined(BUILD_FOR_MINGW)
#	undef __TARGET_PLATFORM__
#	define __TARGET_PLATFORM__ __AEX_WIN32__
#endif

#if defined(_MSC_VER) || defined(__MINGW32__)
#	if defined(AEX_EXPORT_DLL)
#		define AEX_PUBLIC_API __declspec(dllexport)
#	else /* defined(AEX_EXPORT_DLL) */
#		define AEX_PUBLIC_API __declspec(dllimport)
#	endif /* defined(AEX_EXPORT_DLL) */
#	define AEX_PRIVATE_API
#else /* defined(_MSC_VER) */
#	define AEX_PUBLIC_API __attribute__((visibility("default")))
#	define AEX_PRIVATE_API __attribute__((visibility("hidden")))
#endif /* defined(_MSC_VER) */

#ifndef NO_NATIVE_TQ
#   if __TARGET_PLATFORM == __APPLE_IOS
#       define HAS_NATIVE_TQ
#   endif
#endif

#endif
