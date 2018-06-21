# Add sources for each platform.
#

# Default implementations.
set(FILESYSTEM_IMPLEMENTATION "boost")
set(ACCELEROMETER_IMPLEMENTATION "dummy")
set(ORIENTATION_IMPLEMENTATION "dummy")
set(CAMERA_IMPLEMENTATION "dummy")
set(DYNLOAD_IMPLEMENTATION "dummy")

include(${CMAKE_CURRENT_LIST_DIR}/android/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/ios/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/boost/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/osx/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/windows/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/unix/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/dummy/CMakeLists.txt)

# Shared code between platform implementations.

# Shared include files.
set(LOCAL_INCLUDES 
        include/CameraImpl.hpp
        include/CamPrevView.hpp
        include/CamPrevViewShaders.hpp
        include/DynloadFuncs.hpp
        include/FileManagerIMPL.hpp
        include/OrientationImpl.hpp
        include/SDLAccelEventGenerator.hpp
    )
ADD_CURRENT_LIST_DIR(SRC_FRAMEWORK_PLATFORM_SPECIFIC_INCLUDE)
set(SRC_FRAMEWORK_PLATFORM_SPECIFIC_INCLUDE
    ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_INCLUDE}
    include/aex/FileManagerBase.hpp
    include/aex/FileManager.hpp
    include/aex/Camera.hpp
    include/aex/DeviceOrientationObservator.hpp
    include/aex/os/OSUtils.hpp
)

set(PLATFORM_SOURCES ${PLATFORM_SOURCES} 
    src/FileManager.cpp
    src/SDLAccelEventGenerator.cpp
    src/Camera.cpp
    src/DeviceOrientationObservator.cpp
    src/CamPrevView.cpp
    CMakeLists.txt
   )
ADD_CURRENT_LIST_DIR(SRC_FRAMEWORK_PLATFORM_SPECIFIC ${PLATFORM_SOURCES})

source_group("src\\framework\\platform_specific\\include" FILES ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_INCLUDE})
source_group("src\\framework\\platform_specific\\src" FILES ${SRC_FRAMEWORK_PLATFORM_SPECIFIC})
set_source_files_properties(CMakeLists.txt PROPERTIES HEADER_FILE_ONLY TRUE)
