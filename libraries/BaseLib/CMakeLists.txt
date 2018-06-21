set(configdir ${configdir}/AexolGL)
add_definitions(-DAEX_EXPORT_DLL=1)
include(${CMAKE_CURRENT_LIST_DIR}/src/CMakeLists.txt)
set(INCLUDE_FILES
    ${SRC_GROUP_INCLUDE}
    ${SRC_ALGORITHMS_INCLUDE}
    ${SRC_AUDIO_INCLUDE}
    ${SRC_CONTAINERS_INCLUDE}
    ${SRC_FRAMEWORK_GRAPHICS_INCLUDE}
    ${SRC_FRAMEWORK_GRAPHICS_API_INCLUDE}
    ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_INCLUDE}
    ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_IOS_INCLUDE}
    ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_ANDROID_INCLUDE}
    ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_BOOST_INCLUDE}
    ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_WINDOWS_INCLUDE}
    ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_DUMMY_INCLUDE}
    ${SRC_FRAMEWORK_INCLUDE}
    ${SRC_GEOMETRY_INCLUDE}
    ${SRC_MATH_INCLUDE}
    )

set(SOURCE_FILES
    ${SRC_GROUP}
    ${SRC_ALGORITHMS}
    ${SRC_AUDIO}
    ${SRC_CONTAINERS}
    ${SRC_FRAMEWORK}
    ${SRC_FRAMEWORK_GRAPHICS}
    ${SRC_FRAMEWORK_GRAPHICS_API}
    ${SRC_FRAMEWORK_PLATFORM_SPECIFIC}
    ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_IOS}
    ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_ANDROID}
    ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_BOOST}
    ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_WINDOWS}
    ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_DUMMY}
    ${SRC_GEOMETRY}
    ${SRC_MATH}
    )
set(local_includes ${CMAKE_CURRENT_LIST_DIR}/include)
set(external_includes
    ${Boost_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${SDL2IMAGE_INCLUDE_DIR}
    ${SDL2TTF_INCLUDE_DIR}
    ${GL_INCLUDE}
    ${JSONCPP_INCLUDE_DIR}
)
set(build_includes ${local_includes} ${external_includes})
include_directories(${local_includes})
include_directories(SYSTEM ${external_includes})

# Create main library. Needed on all platforms.
add_library(AexolGLMain STATIC src/framework/src/MainEntry.cpp)

if(ASSIMP_FOUND)
    include_directories(SYSTEM ${assimp_INCLUDE_DIRS})
endif()

add_library(AexolGLObject
    OBJECT
    ${SOURCE_FILES}
    )

set(LIB_OPTS
    $<TARGET_OBJECTS:AexolGLObject>
    ${XCODE_DUMMY_SOURCE_WORKAROUND}
    )

if(ANDROID)
    set(LIB_OPTS ${LIB_OPTS}
        PACKAGE_NAME com.aexol.android.lib
        JAVA_SOURCES_ROOT java_android/java
        RESOURCES_ROOT java_android/res
        PROGUARD_FILE java_android/proguard-aex.pro
        GRADLE_PLUGINS maven-publish
        )
    set(ANDROID_DIR_SUFFIX_SHARED ANDROID_DIR_SUFFIX aexlib/)
    set(ANDROID_DIR_SUFFIX_STATIC ANDROID_DIR_SUFFIX aexlib_static/)
    set(ANDROID_ARTIFACT_ID_SHARED ANDROID_ID shared)
    set(ANDROID_ARTIFACT_ID_STATIC ANDROID_ID static)
endif()

# For Android we're not using
# default add_library functions.
add_library_common(AexolGL
    ${DEFAULT_LIB_TYPE}
    ${LIB_OPTS}
    ${ANDROID_DIR_SUFFIX_${DEFAULT_LIB_TYPE}}
    ${ANDROID_DIR_SUFFIX_${DEFAULT_LIB_TYPE}}
    )

# iOS has only static target
# so skip this.
if(NOT IOS)
    add_library_common(AexolGLStatic
        STATIC
        ${LIB_OPTS}
        ${ANDROID_DIR_SUFFIX_STATIC}
        ${ANDROID_DIR_SUFFIX_STATIC}
        )
    # All needed libraries are private for shared AexolGL.
    target_link_libraries(AexolGL PRIVATE ${NEEDED_LIBRARIES} ${INTERFACE_LIBRARIES})
endif()

# Export includes for all targets that depend
# on AexolGLMain/AexolGL/AexolGLStatic. 
# As those directories are absolute keep them only
# during build and do not export to config.
foreach(b_interface ${build_includes})
    target_include_directories(AexolGLMain SYSTEM INTERFACE $<BUILD_INTERFACE:${b_interface}>)
    target_include_directories(AexolGL SYSTEM INTERFACE $<BUILD_INTERFACE:${b_interface}>)
    if(TARGET AexolGLStatic)
        target_include_directories(AexolGLStatic SYSTEM INTERFACE $<BUILD_INTERFACE:${b_interface}>)
    endif()
endforeach()

# Set interface linkage to smallest subset by default.
set(AexolGL_INTERFACE ${INTERFACE_LIBRARIES})

if(ANDROID)
    target_link_host_libraries_android(AexolGL ${NEEDED_LIBRARIES})
    target_link_libraries(AexolGL PRIVATE ${NEEDED_SYSTEM_LIBRARIES})
endif()

if(WIN32)
    set_target_properties(AexolGL PROPERTIES DEBUG_POSTFIX "d")
endif()

# Handle Apple framework dependencies..

if(APPLE)
    # On iOS we need all dependencies in INTERFACE.
    if(IOS)
        set(AexolGL_INTERFACE ${NEEDED_LIBRARIES} ${INTERFACE_LIBRARIES})
    endif()

    # Use PRIVATE_FRAMWORK_[iOS|macOS] to define
    # unexported frameworks. Only really useful on macOS.
    SET(PRIVATE_FRAMEWORKS_macOS
        Foundation
        AudioToolbox
        CoreGraphics
        QuartzCore
        ImageIO
        Cocoa
        CoreAudio
        IOKit
        AudioUnit
        ForceFeedback
        Carbon
        OpenAL
        )
    # Use FRAMWORK_[iOS|macOS] to define
    # exported frameworks.
    set(FRAMEWORKS_iOS
        Foundation
        OpenGLES
        AVFoundation
        AudioToolbox
        AssetsLibrary
        CoreGraphics
        CoreMotion
        CoreVideo
        CoreMedia
        GameController
        QuartzCore
        MobileCoreServices
        ImageIO
        UIKit
        GLKit
        OpenAL
        )

    set(PRIVATE_FRAMEWORKS ${PRIVATE_FRAMEWORKS_${apple_os_name}})
    set(FRAMEWORKS ${FRAMEWORKS_${apple_os_name}})
    foreach(FW ${PRIVATE_FRAMEWORKS})
        target_link_libraries(AexolGL PRIVATE "-framework ${FW}")
    endforeach(FW ${FRAMEWORKS})
    foreach(FW ${FRAMEWORKS})
        target_link_libraries(AexolGL PUBLIC "-framework ${FW}")
    endforeach(FW ${FRAMEWORKS})

    # Static library needs to have both private and interface frameworks
    # exported either way.
    if(TARGET AexolGLStatic)
        foreach(FW ${PRIVATE_FRAMEWORKS} ${FRAMEWORKS})
            target_link_libraries(AexolGLStatic INTERFACE "-framework ${FW}")
        endforeach(FW ${FRAMEWORKS})
    endif()
endif()

if(TARGET AexolGLMain)
    set(_INSTALL_TARGETS ${_INSTALL_TARGETS} AexolGLMain)
endif()

if(TARGET AexolGL)
    set(_INSTALL_TARGETS ${_INSTALL_TARGETS} AexolGL)
endif()

# Set output name for static lib if exists
# along with interface libraries.
if(TARGET AexolGLStatic)
    set(AEXOLGL_STATIC_NAME AexolGL)
    if(WIN32)
        set_target_properties(AexolGLStatic PROPERTIES DEBUG_POSTFIX "d")
        set(AEXOLGL_STATIC_NAME AexolGL_static)
    endif()
    set_target_properties(AexolGLStatic PROPERTIES OUTPUT_NAME ${AEXOLGL_STATIC_NAME})
    add_relacatable_interface_libraries(AexolGLStatic ${NEEDED_LIBRARIES} 
        ${NEEDED_SYSTEM_LIBRARIES}
        ${INTERFACE_LIBRARIES})
    set(_INSTALL_TARGETS ${_INSTALL_TARGETS} AexolGLStatic)
endif()

# xcode properties
if(APPLE)
    string(TOLOWER "${CMAKE_BUILD_TYPE}" lbt)
    if ("${lbt}" STREQUAL "debug")
        set_xcode_property(AexolGL GCC_GENERATE_DEBUGGING_SYMBOLS "YES")
        set_xcode_property(AexolGL COPY_PHASE_STRIP "NO")
    endif()
endif(APPLE)

# Install and export targets
# to AexolGLTargets.cmake and AexolGLConfig.cmake
add_relacatable_interface_libraries(AexolGL ${AexolGL_INTERFACE}) 
target_link_libraries(AexolGLMain INTERFACE ${INTERFACE_LIBRARIES})
if(SDL2MAIN_LIBRARY)
    target_link_libraries(AexolGL INTERFACE ${SDL2MAIN_LIBRARY})
endif()
target_link_libraries(AexolGL INTERFACE AexolGLMain)
if(TARGET AexolGLStatic)
    if(SDL2MAIN_LIBRARY)
        target_link_libraries(AexolGLStatic INTERFACE ${SDL2MAIN_LIBRARY})
    endif()
    target_link_libraries(AexolGLStatic INTERFACE AexolGLMain)
endif()

configure_file(${CMAKE_SOURCE_DIR}/cmake/AexolGLConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/AexolGL/AexolGLConfig.cmake
    @ONLY
    )

install(TARGETS ${_INSTALL_TARGETS} EXPORT AEXOLGL_TARGETS
    LIBRARY DESTINATION ${libdir}
    ARCHIVE DESTINATION ${libdir}
    RUNTIME DESTINATION ${libdir}
    INCLUDES DESTINATION ${includedir}
    )

export(EXPORT AEXOLGL_TARGETS
    FILE "${CMAKE_CURRENT_BINARY_DIR}/AexolGL/AexolGLTargets.cmake"
    NAMESPACE Upstream::
    )

install(EXPORT AEXOLGL_TARGETS
    FILE AexolGLTargets.cmake
    NAMESPACE AexolGL::
    DESTINATION ${configdir}
    )

install(DIRECTORY "include/aex"
    DESTINATION ${includedir}
    COMPONENT Devel
    )

install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/AexolGL/AexolGLConfig.cmake
    DESTINATION ${configdir}
    COMPONENT Devel
    )

if(ANDROID)
    install_aar_name("${CMAKE_CURRENT_BINARY_DIR}/aexlib/build/outputs/aar/aexlib-" aexlib_aar)
    install(FILES
        ${aexlib_aar}
        DESTINATION ${libdir}/cmake/android_gradle/support
        RENAME aexlib.aar
    )
    if(TARGET AexolGLStatic)
        install_aar_name("${CMAKE_CURRENT_BINARY_DIR}/aexlib_static/build/outputs/aar/aexlib_static-" aexlib_static_aar)
        install(FILES
            ${aexlib_static_aar}
            DESTINATION ${libdir}/cmake/android_gradle/support
            RENAME aexlib_static.aar
        )
    endif()
endif()
