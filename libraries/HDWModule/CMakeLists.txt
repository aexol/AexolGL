set(configdir ${configdir}/AexolHDW)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

set(INCLUDE_FILES
   include/aex/hdw/hdw-config.hpp
   include/aex/hdw/SensorManager.hpp
   include/aex/hdw/Sensor.hpp
)

set(SOURCE_FILES
  src/SensorManager.cpp
  src/Sensor.cpp
)

if(IOS)
    set(SOURCE_FILES
        ${SOURCE_FILES}
        src/ios/SensorManagerIOS.mm
        src/ios/SensorIOS.mm
    )

    # include_directories(${FACEBOOK_INCLUDE_DIR})
elseif(ANDROID)
    set(SOURCE_FILES
        ${SOURCE_FILES}
        src/android/JNIUtils.cpp
        src/android/SensorManagerAndroid.cpp
        src/android/SensorAndroid.cpp
        src/android/com_aexolgl_android_hdw_HDWBridge.h
    )
else() # No Sensor support aside from mobiles.
    set(SOURCE_FILES
        ${SOURCE_FILES}
        src/dummy/SensorManagerDummy.cpp
        src/dummy/SensorDummy.cpp
    )
endif()

add_library(AexolHDWObject
    OBJECT
    ${SOURCE_FILES}
  )
# Inherit AexolGL includes.
target_include_directories(AexolHDWObject PRIVATE $<TARGET_PROPERTY:AexolGL,INTERFACE_INCLUDE_DIRECTORIES>)

set(LIB_OPTS
    $<TARGET_OBJECTS:AexolHDWObject>
    ${XCODE_DUMMY_SOURCE_WORKAROUND}
    )

if(ANDROID)
    set(LIB_OPTS ${LIB_OPTS}
        PACKAGE_NAME com.aexol.android.hdw
        JAVA_SOURCES_ROOT java_android/java
        DEPENDENCIES "compile project(':libraries:BaseLib:aexlib')"
        ANDROID_MANIFEST_IN java_android/AndroidManifest.xml.in
        PROGUARD_FILE java_android/proguard-aex.pro
        GRADLE_PLUGINS maven-publish
        )
    set(ANDROID_DIR_SUFFIX_SHARED ANDROID_DIR_SUFFIX hdwmodule)
    set(ANDROID_DIR_SUFFIX_STATIC ANDROID_DIR_SUFFIX hdwmodule_static/)
    set(ANDROID_ARTIFACT_ID_SHARED ANDROID_ID shared)
    set(ANDROID_ARTIFACT_ID_STATIC ANDROID_ID static)
endif()

add_library_common(AexolHDW
    ${DEFAULT_LIB_TYPE}
    ${LIB_OPTS}
    ${ANDROID_DIR_SUFFIX_${DEFAULT_LIB_TYPE}}
    ${ANDROID_DIR_SUFFIX_${DEFAULT_LIB_TYPE}}
    )

if(NOT IOS)
    add_library_common(AexolHdwStatic
        STATIC
        ${LIB_OPTS}
        ${ANDROID_DIR_SUFFIX_STATIC}
        ${ANDROID_DIR_SUFFIX_STATIC}
        )
    target_link_libraries(AexolHdwStatic PUBLIC AexolGLStatic)
endif()

if("${DEFAULT_LIB_TYPE}" STREQUAL SHARED)
    target_link_libraries(AexolHDW PUBLIC AexolGL)
endif()
target_link_libraries(AexolHDW PRIVATE ${JSONCPP_LIBRARY})

if(APPLE)
      SET(FRAMEWORKS_macOS)
      SET(FRAMEWORKS_iOS
          CoreMotion
         )
     foreach(FW ${FRAMEWORKS_${apple_os_name}})
         target_link_libraries(AexolHDW INTERFACE "-framework ${FW}")
     endforeach()
endif()

if(TARGET AexolHDW)
    set(_INSTALL_TARGETS ${_INSTALL_TARGETS} AexolHDW)
endif()

if(TARGET AexolHdwStatic)
    set_target_properties(AexolHdwStatic
        PROPERTIES
        OUTPUT_NAME AexolHDW)

    set(_INSTALL_TARGETS ${_INSTALL_TARGETS} AexolHdwStatic)

    # Handle interface frameworks if static target defined.
    if(APPLE)
        foreach(fw ${FRAMEWORKS_${apple_os_name}})
            target_link_libraries(AexolHdwStatic INTERFACE "-framework ${fw}")
        endforeach()
    endif()
endif()

# Install AexolHDW
install(TARGETS ${_INSTALL_TARGETS} EXPORT AEXOLHDW_TARGETS
    LIBRARY DESTINATION ${libdir}
    ARCHIVE DESTINATION ${libdir}
    RUNTIME DESTINATION ${libdir}
    INCLUDES DESTINATION ${includedir}
 )

 export(EXPORT AEXOLHDW_TARGETS
     FILE "${CMAKE_CURRENT_BINARY_DIR}/AexolHDW/AexolHDWTargets.cmake"
     NAMESPACE AexolGL::
     )

 install(EXPORT AEXOLHDW_TARGETS
     FILE AexolHDWTargets.cmake
    NAMESPACE AexolGL::
    DESTINATION ${configdir}
    )

install(DIRECTORY "include/aex"
    DESTINATION ${includedir}
    COMPONENT Devel 
   )

install(
    FILES
    ${CMAKE_SOURCE_DIR}/cmake/AexolHDWConfig.cmake
    DESTINATION ${configdir}/AexolHDW
    COMPONENT Devel
    )
SET( AEXOL_HDW_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include" PARENT_SCOPE )

if(ANDROID)
    install_aar_name("${CMAKE_CURRENT_BINARY_DIR}/hdwmodule/build/outputs/aar/hdwmodule-" hdwmodule_aar)
    install(FILES
        ${hdwmodule_aar}
        DESTINATION ${libdir}/cmake/android_gradle/support
        RENAME hdwmodule.aar
    )
    if(TARGET AexolHdwStatic)
        install_aar_name("${CMAKE_CURRENT_BINARY_DIR}/hdwmodule_static/build/outputs/aar/hdwmodule_static-" hdwmodule_static_aar)
        install(FILES
            ${hdwmodule_static_aar}
            DESTINATION ${libdir}/cmake/android_gradle/support
            RENAME hdwmodule_static.aar
        )
    endif()
endif()
