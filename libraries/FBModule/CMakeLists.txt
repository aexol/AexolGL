set(configdir ${configdir}/AexolFB)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

set(INCLUDE_FILES
    include/aex/fb/fb-config.hpp
    include/aex/fb/FBAppEventsLogger.hpp
    include/aex/fb/FBGraphRequest.hpp
    include/aex/fb/FBLoginManager.hpp
    include/aex/fb/FBPermissions.hpp
    include/aex/fb/FBResults.hpp
    )

set(SOURCE_FILES
    src/FBAppEventsLogger.cpp
    src/FBGraphRequest.cpp
    src/FBLoginManager.cpp
    src/FBPermissions.cpp
    src/FBResults.cpp
    )

if(IOS)
    set(INCLUDE_FILES
        ${INCLUDE_FILES} 
        src/ios/SDL_uikitappdelegate_swizzling.h
        )
    # This files require FacebookSDK to be built.
    set(FACEBOOK_SUPPORT_SOURCES
        src/ios/FBAppEventsLoggerIOS.mm
        src/ios/FBGraphRequestIOS.mm
        src/ios/FBLoginManagerIOS.mm
        src/ios/SDL_uikitappdelegate_swizzling.m
        )
    set(SOURCE_FILES
        ${SOURCE_FILES}
        ${FACEBOOK_SUPPORT_SOURCES}
        )
elseif(ANDROID)
    set(SOURCE_FILES
        ${SOURCE_FILES}
        src/android/FBAppEventsLoggerAndroid.cpp
        src/android/FBGraphRequestAndroid.cpp
        src/android/FBLoginManagerAndroid.cpp
        src/android/JNIUtils.cpp

        src/android/FBBridge.hpp
        )
else() # No native facebook support aside from mobile.
    set(SOURCE_FILES
        ${SOURCE_FILES}
        src/dummy/FBAppEventsLoggerDummy.cpp
        src/dummy/FBGraphRequestDummy.cpp
        src/dummy/FBLoginManagerDummy.cpp
        )
endif()

add_library(AexolFBObject
    OBJECT
    ${SOURCE_FILES}
    )
# Inherit AexolGL includes.
target_include_directories(AexolFBObject PRIVATE $<TARGET_PROPERTY:AexolGL,INTERFACE_INCLUDE_DIRECTORIES>)

set(LIB_OPTS
    $<TARGET_OBJECTS:AexolFBObject>
    ${XCODE_DUMMY_SOURCE_WORKAROUND}
    )

if(ANDROID)
    set(LIB_OPTS ${LIB_OPTS}
        PACKAGE_NAME com.aexol.android.fb
        JAVA_SOURCES_ROOT java_android/java
        RESOURCES_ROOT java_android/res
        DEPENDENCIES "compile project(':libraries:BaseLib:aexlib')"
        "compile 'com.facebook.android:facebook-android-sdk:4.23.0'"
        ANDROID_MANIFEST_IN java_android/AndroidManifest.xml.in
        PROGUARD_FILE java_android/proguard-aex.pro
        ANDROID_SDK_API_VERSION 24
        ANDROID_MIN_SDK_API_VERSION 15
        ANDROID_TARGET_SDK_API_VERSION 15
        GRADLE_PLUGINS maven-publish
        )
    set(ANDROID_DIR_SUFFIX_SHARED ANDROID_DIR_SUFFIX fbmodule)
    set(ANDROID_DIR_SUFFIX_STATIC ANDROID_DIR_SUFFIX fbmodule_static/)
    set(ANDROID_ARTIFACT_ID_SHARED ANDROID_ID shared)
    set(ANDROID_ARTIFACT_ID_STATIC ANDROID_ID static)
endif()

add_library_common(AexolFB
    ${DEFAULT_LIB_TYPE}
    ${LIB_OPTS}
    ${ANDROID_DIR_SUFFIX_${DEFAULT_LIB_TYPE}}
    ${ANDROID_DIR_SUFFIX_${DEFAULT_LIB_TYPE}}
)

if(NOT IOS)
    add_library_common(AexolFBStatic
        STATIC
        ${LIB_OPTS}
        ${ANDROID_DIR_SUFFIX_STATIC}
        ${ANDROID_DIR_SUFFIX_STATIC}
    )
    target_link_libraries(AexolFBStatic PUBLIC AexolGLStatic)
endif()

if("${DEFAULT_LIB_TYPE}" STREQUAL "SHARED")
    target_link_libraries(AexolFB PRIVATE ${JSONCPP_LIBRARY})
endif()
target_link_libraries(AexolFB PUBLIC AexolGL)

# Handle Apple frameworks.
if(APPLE)
    SET(FRAMEWORKS_macOS)
    SET(FRAMEWORKS_iOS
        FBSDKCoreKit
        FBSDKLoginKit
        )

    # Add path to FacebooSDK while compiling for iOS.
    if(IOS)
        set_source_files_properties(${FACEBOOK_SUPPORT_SOURCES} 
            PROPERTIES COMPILE_FLAGS "-F${FacebookSDK_INCLUDE_DIR}")
    endif()
    foreach(fw ${FRAMEWORKS_${apple_os_name}})
        target_link_libraries(AexolFB PUBLIC "-framework ${fw}")
    endforeach()
endif()

if(TARGET AexolFB)
    set(INSTALL_TARGETS_LIST ${INSTALL_TARGETS_LIST} AexolFB)
endif()

if(TARGET AexolFBStatic)
    set_target_properties(AexolFBStatic
        PROPERTIES
        OUTPUT_NAME AexolFB)

    set(INSTALL_TARGETS_LIST ${INSTALL_TARGETS_LIST} AexolFBStatic)

    # Handle interface frameworks if static target defined.
    if(APPLE)
        foreach(fw ${FRAMEWORKS_${apple_os_name}})
            target_link_libraries(AexolFBStatic INTERFACE "-framework ${fw}")
        endforeach()
    endif()
endif()

configure_file(${CMAKE_SOURCE_DIR}/cmake/AexolFBConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/AexolFB/AexolFBConfig.cmake
    @ONLY)

# Install AexolFB
install(TARGETS ${INSTALL_TARGETS_LIST} EXPORT AEXOLFB_TARGETS
    LIBRARY DESTINATION ${libdir}
    ARCHIVE DESTINATION ${libdir}
    RUNTIME DESTINATION ${libdir}
    INCLUDES DESTINATION ${includedir}
    )

export(EXPORT AEXOLFB_TARGETS
    FILE "${CMAKE_CURRENT_BINARY_DIR}/AexolFB/AexolFBTargets.cmake"
    NAMESPACE AexolGL::
    )

install(EXPORT AEXOLFB_TARGETS
    FILE AexolFBTargets.cmake
    NAMESPACE AexolGL::
    DESTINATION ${configdir}
    )

install(DIRECTORY "include/aex"
    DESTINATION ${includedir}
    COMPONENT Devel 
    )

install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/AexolFB/AexolFBConfig.cmake
    DESTINATION ${configdir}
    COMPONENT Devel
    )
SET( AEXOL_FB_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include" PARENT_SCOPE )

if(ANDROID)
    install_aar_name("${CMAKE_CURRENT_BINARY_DIR}/fbmodule/build/outputs/aar/fbmodule-" fbmodule_aar)
    install(FILES
        ${fbmodule_aar}
        DESTINATION ${libdir}/cmake/android_gradle/support
        RENAME fbmodule.aar
    )
    if(TARGET AexolFBStatic)
        install_aar_name("${CMAKE_CURRENT_BINARY_DIR}/fbmodule_static/build/outputs/aar/fbmodule_static-" fbmodule_static_aar)
        install(FILES
            ${fbmodule_static_aar}
            DESTINATION ${libdir}/cmake/android_gradle/support
            RENAME fbmodule_static.aar
        )
    endif()
endif()
