# Add sources for ios platform.
# Checks if source is added or not is checked inside file.
#
if(IOS)
    set(FILESYSTEM_IMPLEMENTATION "ios")
    set(ACCELEROMETER_IMPLEMENTATION "ios")
    set(ORIENTATION_IMPLEMENTATION "ios")
    set(CAMERA_IMPLEMENTATION "ios")
    set(LOCAL_INCLUDES
        include/FileManagerIOS.hh
        include/FileManagerIOS.hpp
        include/CameraController.hh
        include/iOSHWSensor.hh
        include/IOSUtils.hh
        include/IOSAssets.hh
        )
    ADD_CURRENT_LIST_DIR(SRC_FRAMEWORK_PLATFORM_SPECIFIC_IOS_INCLUDE)
    set(SRC_FRAMEWORK_PLATFORM_SPECIFIC_IOS_INCLUDE
        ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_IOS_INCLUDE}
        )

    set(_FRAMEWORK_PLATFORM_SPECIFIC_IOS ${PLATFORM_SOURCES}
        src/AccelerometerIosImpl.mm
        src/iOSHWSensor.mm
        src/FileManagerFactoryIMPL.cpp
        src/FileManagerIOS.mm
        src/OSInit.mm
        src/OrientationImpl.cpp
        src/CameraController.mm
        src/CameraIOS.mm
        src/CameraImplIOS_Y_UV.cpp
        src/IOSUtils.mm
        src/IOSAssets.mm
        src/OSUtilsIOS.mm
        CMakeLists.txt
        )
    ADD_CURRENT_LIST_DIR(SRC_FRAMEWORK_PLATFORM_SPECIFIC_IOS ${_FRAMEWORK_PLATFORM_SPECIFIC_IOS})

    source_group("src\\framework\\platform_specific\\ios\\include" FILES ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_IOS_INCLUDE} ${INCLUDE_FILES_IOS_SUPPORT})
    source_group("src\\framework\\platform_specific\\ios\\src" FILES ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_IOS} )
    #set_source_files_properties(CMakeLists.txt PROPERTIES HEADER_FILE_ONLY TRUE)
else()
    set(SRC_FRAMEWORK_PLATFORM_SPECIFIC_IOS_INCLUDE )
    set(SRC_FRAMEWORK_PLATFORM_SPECIFIC_IOS )
endif()
