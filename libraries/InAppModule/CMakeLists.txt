set(configdir ${configdir}/AexolInApp)
set(INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/include)

include_directories(${INCLUDE_DIRS})



set(INCLUDE_FILES
    include/aex/ShopInterface.hpp
    include/aex/ShopHelper.hpp
    include/aex/IShop.hpp
    include/aex/IAPCallback.hpp
   )

set(SOURCE_FILES
    src/ShopHelper.cpp
    src/IAPCallback.cpp
   #src/ShopInterface.cpp
   )

if(ANDROID)

    set(INCLUDE_FILES 
        ${INCLUDE_FILES}
        src/android/AndroidBridge.hpp
       )

    set(SOURCE_FILES 
        ${SOURCE_FILES} 
        src/android/AndroidShop.cpp
        src/dummy/IShopDummy.cpp
       )

elseif(IOS)

    set(INCLUDE_FILES 
        ${INCLUDE_FILES}
        src/ios/IAPHelper_objc.hh
        src/ios/IAPWrapper.hh
        )

    set(SOURCE_FILES 
        ${SOURCE_FILES} 
        src/ios/IAPHelper_objc.mm
        src/ios/IAPWrapper.mm
        src/ios/IShop.cpp
        )

else()
    set(DUMMY_INCLUDE_FILES

       )

    set(DUMMY_SOURCE_FILES

       )

endif()

add_library(AexolInAppObject
    OBJECT
    ${SOURCE_FILES}
    )
# Inherit AexolGL includes.
target_include_directories(AexolInAppObject PRIVATE $<TARGET_PROPERTY:AexolGL,INTERFACE_INCLUDE_DIRECTORIES>)

set(LIB_OPTS
    $<TARGET_OBJECTS:AexolInAppObject>
    ${XCODE_DUMMY_SOURCE_WORKAROUND}
    )


if(ANDROID)
    set(LIB_OPTS ${LIB_OPTS}
        PACKAGE_NAME com.aexolgl.inapp
        JAVA_SOURCES_ROOT java_android/java
        DEPENDENCIES "compile project(':libraries:BaseLib:aexlib')"
        "compile 'com.android.billingclient:billing:1.0'"
        ANDROID_MANIFEST_IN java_android/AndroidManifest.xml.in
        PROGUARD_FILE java_android/proguard-aex.pro
        ANDROID_SDK_API_VERSION 24
        ANDROID_MIN_SDK_API_VERSION 15
        ANDROID_TARGET_SDK_API_VERSION 15
        GRADLE_PLUGINS maven-publish
        )

    set(ANDROID_DIR_SUFFIX_SHARED  ANDROID_DIR_SUFFIX inappmodule)
    set(ANDROID_DIR_SUFFIX_STATIC  ANDROID_DIR_SUFFIX inappmodule_static/)
    set(ANDROID_ARTIFACT_ID_SHARED ANDROID_ID shared)
    set(ANDROID_ARTIFACT_ID_STATIC ANDROID_ID static)
endif()

add_library_common(AexolInApp
    ${DEFAULT_LIB_TYPE}
    ${LIB_OPTS}
    ${ANDROID_DIR_SUFFIX_${DEFAULT_LIB_TYPE}}
    ${ANDROID_DIR_SUFFIX_${DEFAULT_LIB_TYPE}}
    )


# iOS has only static target
# so skip this.
if(NOT IOS)
    add_library_common(AexolInAppStatic
        STATIC
        ${LIB_OPTS}
        ${ANDROID_DIR_SUFFIX_STATIC}
        ${ANDROID_DIR_SUFFIX_STATIC}
        )
    target_link_libraries(AexolInAppStatic PUBLIC AexolGLStatic)
endif()

target_link_libraries(AexolInApp PUBLIC AexolGL)


if(APPLE)
    set(FRAMEWORKS_macOS)
#    set(FRAMEWORKS_iOS InApp)
    set(FRAMEWORKS ${FRAMEWORKS_${apple_os_name}})
    foreach(FW ${FRAMEWORKS})
        target_link_libraries(AexolInApp PUBLIC "-framework ${FW}")
    endforeach(FW ${FRAMEWORKS})
endif()

if(TARGET AexolInApp)
    set(_INSTALL_TARGETS ${_INSTALL_TARGETS} AexolInApp)
endif()


# Set output name for static lib if exists
# along with interface libraries.
if(TARGET AexolInAppStatic)
    set(AEXOLINAPP_STATIC_NAME AexolInApp)
    if(WIN32)
        set_target_properties(AexolInAppStatic PROPERTIES DEBUG_POSTFIX "d")
        set(AEXOLINAPP_STATIC_NAME AexolInApp_static)
    endif()
    set_target_properties(AexolInAppStatic PROPERTIES OUTPUT_NAME ${AEXOLINAPP_STATIC_NAME})
    set(AEXOLINAPP_STATIC_DEPS)
    foreach(fn ${NEEDED_LIBRARIES} ${INTERFACE_LIBRARIES})
        get_filename_component(lib_name ${fn} NAME_WE)
        string(REGEX REPLACE "^lib" "" lib_name ${lib_name})
        set(AEXOLINAPP_STATIC_DEPS "${AEXOLINAPP_STATIC_DEPS} ${lib_name}")
    endforeach()
    set(_INSTALL_TARGETS ${_INSTALL_TARGETS} AexolInAppStatic)
    if(APPLE)
        foreach(FW ${FRAMEWORKS})
            target_link_libraries(AexolInAppStatic INTERFACE "-framework ${FW}")
        endforeach(FW ${FRAMEWORKS})
    endif()
endif()

configure_file(${CMAKE_SOURCE_DIR}/cmake/AexolInAppConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/AexolInApp/AexolInAppConfig.cmake
    @ONLY)


# Install AexolInApp
install(TARGETS ${_INSTALL_TARGETS} EXPORT AEXOLINAPP_TARGETS
    LIBRARY DESTINATION ${libdir}
    ARCHIVE DESTINATION ${libdir}
    RUNTIME DESTINATION ${libdir}
    INCLUDES DESTINATION ${includedir}
    )

export(EXPORT AEXOLINAPP_TARGETS
    FILE "${CMAKE_CURRENT_BINARY_DIR}/AexolInApp/AexolInAppTargets.cmake"
    NAMESPACE AexolGL::
    )

install(EXPORT AEXOLINAPP_TARGETS
    FILE AexolInAppTargets.cmake
    NAMESPACE AexolGL::
    DESTINATION ${configdir}
    )

install(DIRECTORY "include/aex"
    DESTINATION ${includedir}
    COMPONENT Devel 
    )

install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/AexolInApp/AexolInAppConfig.cmake
    DESTINATION ${configdir}
    COMPONENT Devel
    )
SET( AEXOL_INAPP_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include" PARENT_SCOPE )

if(ANDROID)
    install_aar_name("${CMAKE_CURRENT_BINARY_DIR}/inappmodule/build/outputs/aar/inappmodule-" inappmodule_aar)
    install(FILES
        ${inappmodule_aar}
        DESTINATION ${libdir}/cmake/android_gradle/support
        RENAME inappmodule.aar
    )
if(TARGET AexolInAppStatic)
        install_aar_name("${CMAKE_CURRENT_BINARY_DIR}/inappmodule_static/build/outputs/aar/inappmodule_static-" inappmodule_static_aar)
        install(FILES
            ${inappmodule_static_aar}
            DESTINATION ${libdir}/cmake/android_gradle/support
            RENAME inappmodule_static.aar
        )
    endif()
endif()