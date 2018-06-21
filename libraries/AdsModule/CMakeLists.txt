set(configdir ${configdir}/AexolAds)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
set(INCLUDE_FILES
    include/aex/ads/ads-config.hpp
    include/aex/ads/AdMob.hpp   
    include/aex/ads/iAds.hpp
    )

set(SOURCE_FILES
    src/AdMob.cpp
    src/AdInterface.cpp
    )


if(IOS)
    set(INCLUDE_FILES 
        ${INCLUDE_FILES} 
        src/ios/OverlayViewIOS.hh
        src/ios/adMobImplementation/BannerViewControllerAdMob.h
        src/ios/adMobImplementation/InterstitialViewControllerAdMob.h
        src/ios/iAdsImplementation/BannerViewController.h
        src/ios/iAdsImplementation/InterstitialViewController.hh
        src/ios/iAdsIos.h
        src/ios/iAdsIos.hh
    )

    set(SOURCE_FILES 
        ${SOURCE_FILES} 
        src/ios/OverlayViewIOS.mm
        src/ios/AdMobIos.cpp
        src/ios/adMobImplementation/BannerViewControllerAdMob.m
        src/ios/adMobImplementation/InterstitialViewControllerAdMob.m
        src/ios/iAdsIos.mm
        src/ios/iAdsImplementation/BannerViewController.m
        src/ios/iAdsImplementation/InterstitialViewController.mm
        )

elseif(ANDROID)
    set(SOURCE_FILES
        ${SOURCE_FILES} 
        src/android/AdMobAndroid.cpp
        src/dummy/IAdsDummy.cpp
        )
else()
    set(SOURCE_FILES
        ${SOURCE_FILES} 
        src/dummy/AdMobDummy.cpp
        src/dummy/IAdsDummy.cpp
        )
endif()

add_library(AexolAdsObject
    OBJECT
    ${SOURCE_FILES}
    )
# Inherit AexolGL includes.
target_include_directories(AexolAdsObject PRIVATE $<TARGET_PROPERTY:AexolGL,INTERFACE_INCLUDE_DIRECTORIES>)

set(LIB_OPTS
    $<TARGET_OBJECTS:AexolAdsObject>
    ${XCODE_DUMMY_SOURCE_WORKAROUND}
    )

if(ANDROID)
    set(LIB_OPTS ${LIB_OPTS}
        PACKAGE_NAME com.aexol.android.ads
        JAVA_SOURCES_ROOT java_android/java
        DEPENDENCIES "compile project(':libraries:BaseLib:aexlib')"
        "compile 'com.google.android.gms:play-services-ads:11.0.1'"
        GRADLE_PLUGINS maven-publish
        ANDROID_MANIFEST_IN java_android/AndroidManifest.xml.in
        PROGUARD_FILE java_android/proguard-aex.pro
        ANDROID_SDK_API_VERSION 24
        ANDROID_MIN_SDK_API_VERSION 15
        ANDROID_TARGET_SDK_API_VERSION 15
        )
    set(ANDROID_DIR_SUFFIX_SHARED ANDROID_DIR_SUFFIX adsmodule)
    set(ANDROID_DIR_SUFFIX_STATIC ANDROID_DIR_SUFFIX adsmodule_static/)
    set(ANDROID_ARTIFACT_ID_SHARED ANDROID_ID shared)
    set(ANDROID_ARTIFACT_ID_STATIC ANDROID_ID static)
endif()

add_library_common(AexolAds
    ${DEFAULT_LIB_TYPE}
    ${LIB_OPTS}
    ${ANDROID_DIR_SUFFIX_${DEFAULT_LIB_TYPE}}
    ${ANDROID_DIR_SUFFIX_${DEFAULT_LIB_TYPE}}
    )

# iOS has only static target
# so skip this.
if(NOT IOS)
    add_library_common(AexolAdsStatic
        STATIC
        ${LIB_OPTS}
        ${ANDROID_DIR_SUFFIX_STATIC}
        ${ANDROID_DIR_SUFFIX_STATIC}
        )
    target_link_libraries(AexolAdsStatic PUBLIC AexolGLStatic)
endif()
target_link_libraries(AexolAds PUBLIC AexolGL)

if(APPLE)
    set(FRAMEWORKS_macOS)
    set(FRAMEWORKS_iOS iAd)
    set(FRAMEWORKS ${FRAMEWORKS_${apple_os_name}})
    foreach(FW ${FRAMEWORKS})
        target_link_libraries(AexolAds PUBLIC "-framework ${FW}")
    endforeach(FW ${FRAMEWORKS})
endif()

if(TARGET AexolAds)
    set(_INSTALL_TARGETS ${_INSTALL_TARGETS} AexolAds)
endif()

# Set output name for static lib if exists
# along with interface libraries.
if(TARGET AexolAdsStatic)
    set(AEXOLADS_STATIC_NAME AexolAds)
    if(WIN32)
        set_target_properties(AexolAdsStatic PROPERTIES DEBUG_POSTFIX "d")
        set(AEXOLADS_STATIC_NAME AexolAds_static)
    endif()
    set_target_properties(AexolAdsStatic PROPERTIES OUTPUT_NAME ${AEXOLADS_STATIC_NAME})
    set(AEXOLADS_STATIC_DEPS)
    foreach(fn ${NEEDED_LIBRARIES} ${INTERFACE_LIBRARIES})
        get_filename_component(lib_name ${fn} NAME_WE)
        string(REGEX REPLACE "^lib" "" lib_name ${lib_name})
        set(AEXOLADS_STATIC_DEPS "${AEXOLADS_STATIC_DEPS} ${lib_name}")
    endforeach()
    set(_INSTALL_TARGETS ${_INSTALL_TARGETS} AexolAdsStatic)
    if(APPLE)
        foreach(FW ${FRAMEWORKS})
            target_link_libraries(AexolAdsStatic INTERFACE "-framework ${FW}")
        endforeach(FW ${FRAMEWORKS})
    endif()
endif()

configure_file(${CMAKE_SOURCE_DIR}/cmake/AexolAdsConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/AexolAds/AexolAdsConfig.cmake
    @ONLY)

# Install AexolAds
install(TARGETS ${_INSTALL_TARGETS} EXPORT AEXOLADS_TARGETS
    LIBRARY DESTINATION ${libdir}
    ARCHIVE DESTINATION ${libdir}
    RUNTIME DESTINATION ${libdir}
    INCLUDES DESTINATION ${includedir}
    )

export(EXPORT AEXOLADS_TARGETS
    FILE "${CMAKE_CURRENT_BINARY_DIR}/AexolAds/AexolAdsTargets.cmake"
    NAMESPACE AexolGL::
    )

install(EXPORT AEXOLADS_TARGETS
    FILE AexolAdsTargets.cmake
    NAMESPACE AexolGL::
    DESTINATION ${configdir}
    )

install(DIRECTORY "include/aex"
    DESTINATION ${includedir}
    COMPONENT Devel 
    )

install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/AexolAds/AexolAdsConfig.cmake
    DESTINATION ${configdir}
    COMPONENT Devel
    )
SET( AEXOL_ADS_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include" PARENT_SCOPE )

if(ANDROID)
    install_aar_name("${CMAKE_CURRENT_BINARY_DIR}/adsmodule/build/outputs/aar/adsmodule-" adsmodule_aar)
    install(FILES
        ${adsmodule_aar}
        DESTINATION ${libdir}/cmake/android_gradle/support
        RENAME adsmodule.aar
    )
if(TARGET AexolAdsStatic)
        install_aar_name("${CMAKE_CURRENT_BINARY_DIR}/adsmodule_static/build/outputs/aar/adsmodule_static-" adsmodule_static_aar)
        install(FILES
            ${adsmodule_static_aar}
            DESTINATION ${libdir}/cmake/android_gradle/support
            RENAME adsmodule_static.aar
        )
    endif()
endif()
