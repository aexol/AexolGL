set(test_sources 
    "test_src/positionnormaluv.cpp"
    "test_src/Traits.cpp"
    "test_src/Simulation.cpp"
    "test_src/Animation.cpp"
    "test_src/BasicAnimation.cpp"
    "test_src/AnimationPlayers.cpp"
    "test_src/Subsystem.cpp"
    "test_src/Proxy.cpp"
    "test_src/FrameData.cpp"
    "test_src/KeyFrameData.cpp"
    "test_src/SaveSimpleKeyFrame.cpp"
    "test_src/ReadSimpleKeyFrame.cpp"
    "test_src/SimpleKeyFrameAnimation.cpp"
    )

enable_testing()

include_directories("${CMAKE_CURRENT_LIST_DIR}/include")
include_directories(${BASELIB_INCLUDE_DIR})
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2IMAGE_INCLUDE_DIR})
include_directories(${GL_INCLUDE})
include_directories(${TBB_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIR})
include_directories(${JSONCPP_INCLUDE_DIR})

include(GTest)

if(APPLE)
    set(FRAMEWORKS
        Foundation
        AudioToolbox
        CoreGraphics
        QuartzCore
        ImageIO
        Cocoa
        )
    foreach(FW ${FRAMEWORKS})
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework ${FW}")
    endforeach()
endif()

# TODO Make library target from support files.

set(support_files "test_src/MockSimulationDataTraits.cpp"
    "test_src/SimpleKeyFrameSample.cpp"
    "test_src/MockWindow.cpp"
    )

if(NOT ANDROID)
    set(main_source_file "")
    set(aexol_library_target "AexolGL")
else(NOT ANDROID)
    set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
    file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
    set(main_source_file "test_src/main_android.cc")
    set(support_files ${support_files} "test_src/sdl_main_android.cc")
    set(aexol_library_target "AexolGLStatic")
    get_target_property(_dependencies ${aexol_library_target} INTERFACE_LINK_LIBRARIES)
    foreach(dep IN LISTS _dependencies)
        get_filename_component(_dep_ext ${dep} EXT)
        if("${_dep_ext}" MATCHES ".*\.so")
            execute_process( COMMAND "${CMAKE_COMMAND}" -E copy_if_different ${dep} "${EXECUTABLE_OUTPUT_PATH}/")
        endif("${_dep_ext}" MATCHES ".*\.so")
    endforeach()
    file(GLOB GLOBAL_LIBS "${LIBRARY_OUTPUT_PATH}/*.so")
    foreach(f ${GLOBAL_LIBS})
      execute_process( COMMAND "${CMAKE_COMMAND}" -E copy_if_different ${f} "${EXECUTABLE_OUTPUT_PATH}/")
    endforeach(f ${GLOBAL_LIBS})
endif(NOT ANDROID)

foreach(test_src ${test_sources})
    get_filename_component(moduleName ${test_src} NAME_WE)
    set(moduleName "${moduleName}_TEST")
    gtest_add_test(${moduleName} ${support_files} ${test_src} MAIN_SOURCE ${main_source_file})
    target_link_libraries(${moduleName} ${aexol_library_target} ${NEEDED_SYSTEM_LIBRARIES})
    if(ANDROID)
        set_target_properties(${moduleName} PROPERTIES LOCATION ${EXECUTABLE_OUTPUT_PATH})
    endif(ANDROID)
endforeach(test_src ${TEST_SRC_FILES})
