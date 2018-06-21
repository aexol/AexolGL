# Add sources for android platform.
#
if(ANDROID)
    set(FILESYSTEM_IMPLEMENTATION "android")
    set(ACCELEROMETER_IMPLEMENTATION "android")
    set(ORIENTATION_IMPLEMENTATION "android")
    set(CAMERA_IMPLEMENTATION "android")

    set(LOCAL_INCLUDES 
        include/GeneralAndroid.hpp
        include/FBButtonAndroid.h
        include/CameraMediator.hpp
        include/CameraImplAndroid.hpp
        )
    ADD_CURRENT_LIST_DIR(SRC_FRAMEWORK_PLATFORM_SPECIFIC_ANDROID_INCLUDE)
    set(SRC_FRAMEWORK_PLATFORM_SPECIFIC_ANDROID_INCLUDE
        ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_ANDROID_INCLUDE}
        include/aex/JniObject.hpp
        )

    set(_FRAMEWORK_PLATFORM_SPECIFIC_ANDROID ${PLATFORM_SOURCES} 
            src/GeneralAndroid.cpp
            src/FileManagerIMPL.cpp
            src/ALooperWrapper.cpp
            src/AndroidAPI.cpp
            src/AndroidInternal.cpp
            src/AccelerometerAndroidIMPL.cpp
            src/CameraImplAndroid.cpp
            src/CameraMediator.cpp
            src/CameraAndroid.cpp
            src/JniObject.cpp
            src/OrientationImpl.cpp
            src/OSUtilsAndroid.cpp
            CMakeLists.txt
        )
    ADD_CURRENT_LIST_DIR(SRC_FRAMEWORK_PLATFORM_SPECIFIC_ANDROID ${_FRAMEWORK_PLATFORM_SPECIFIC_ANDROID})

    

    source_group("src\\framework\\platform_specific\\android\\include" FILES ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_ANDROID_INCLUDE})
    source_group("src\\framework\\platform_specific\\android\\src" FILES ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_ANDROID})
    set_source_files_properties(CMakeLists.txt PROPERTIES HEADER_FILE_ONLY TRUE)
else()
    set(SRC_FRAMEWORK_PLATFORM_SPECIFIC_ANDROID_INCLUDE )
    set(SRC_FRAMEWORK_PLATFORM_SPECIFIC_ANDROID )
endif()


