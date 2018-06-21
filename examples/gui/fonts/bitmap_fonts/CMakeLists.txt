CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
project("bitmapFont")
set(ANDROID_SDK_API_VERSION 24)
set(ANDROID_BUILD_TOOLS_VERSION 25.0.0)
set(ANDROID_MIN_SDK_API_VERSION 15)
set(ANDROID_TARGET_SDK_API_VERSION 18)
include(${AEXOL_SDK}/lib/cmake/utils/helper_macros.cmake)

#General debugging flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG_GL")
if(ANDROID)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNDK_DEBUG=1")
endif(ANDROID)

if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -ffast-math ")
endif()

#Library is build using new C++11 features that's why this line is required no matter the enviroment
#except for MSVC which has it enabled by default

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -ObjC")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++ -ObjC")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -stdlib=libc++ -ObjC")
endif( APPLE )

set(SOURCE_FILES
    bitmap_font.cpp
    )

set(APP_OPTS ${SOURCE_FILES})
if(ANDROID)
    set(BASE_ACTIVITY_NAME "AexActivity")
    set(ANDROID_DEPENDENCIES
        "compile 'com.aexol.android:lib:1.0'"
        )
    set(APP_OPTS ${APP_OPTS}
        PACKAGE_NAME com.aexol.examples.bitmap.font
        DEPENDENCIES ${ANDROID_DEPENDENCIES}
        ASSETS_ROOT assets
        )
elseif(APPLE)
    set(APP_OPTS ${APP_OPTS}
        assets/Data
        )
endif()

find_package(AexolGL)
add_executable_common(${PROJECT_NAME} ${APP_OPTS})

target_link_libraries(${PROJECT_NAME} AexolGL::AexolGL)
