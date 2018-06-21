# gtest.

include(ExternalProject)

set(GTEST_SOURCE_DIR "${CMAKE_BINARY_DIR}/cmake/googletest-src")
set(GTEST_BUILD_DIR "${CMAKE_BINARY_DIR}/cmake/googletest-build")

macro(pi_external_project_add name)
    if(NOT ANDROID)
        ExternalProject_Add(${name} "${ARGN}")
    else(NOT ANDROID)
        find_host_external_project(${name} "${ARGN}")
    endif(NOT ANDROID)
endmacro(pi_external_project_add name)

macro(pi_gtest_add_test name)
    if(NOT ANDROID)
        add_test(NAME ${name} 
            COMMAND ${name} ${ARGN}
        )
    else(NOT ANDROID)
        add_test(NAME ${name} 
            COMMAND ${CMAKE_COMMAND}
            -DTEST=${name}
            -DPUSH_DIR=${EXECUTABLE_OUTPUT_PATH}
            -DTESTED_PROJECT_NAME=${CMAKE_PROJECT_NAME}
            -DTEST_ARGS="${ARGN}" 
            -P ${CMAKE_SOURCE_DIR}/cmake/Modules/RunTestsAndroid.cmake
            )
    endif(NOT ANDROID)
endmacro(pi_gtest_add_test name)

set(gtest_args "")

if(ANDROID)
    set(gtest_cmake_args "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}" "-DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}" "-DANDROID_NATIVE_API_LEVEL=${ANDROID_NATIVE_API_LEVEL}" "-DANDROID_ABI=${ANDROID_ABI}" "-DANDROID_TOOLCHAIN_NAME=${ANDROID_TOOLCHAIN_NAME}" "-DANDROID_STL=${ANDROID_STL}")
    set(test_exec_command)
else()
    if(MSVC OR MINGW)
	if("${gtest_cmake_args}" STREQUAL "")
		set(gtest_cmake_args "")
	endif()
        set(gtest_cmake_args ${gtest_cmake_args} "-Dgtest_force_shared_crt=ON")
    endif()
    if(MINGW)
        set(gtest_cmake_args ${gtest_cmake_args} "-Dgtest_disable_pthreads=ON")
    endif()
    set(gtest_args)
endif()
set(google_link_directories 
    "${GTEST_BUILD_DIR}/googlemock"
    "${GTEST_BUILD_DIR}/googlemock/gtest"
    )

pi_external_project_add(googletest
        GIT_REPOSITORY "https://github.com/google/googletest"
        GIT_TAG "master"
        BINARY_DIR "${GTEST_BUILD_DIR}"
        SOURCE_DIR "${GTEST_SOURCE_DIR}"
        INSTALL_COMMAND ""
        CMAKE_ARGS ${gtest_cmake_args}
        "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
        )

set(google_project_name googletest)
set(google_libraries gtest gmock)
set(google_main_library gmock_main)

set(GTEST_INCLUDE_DIRS
    "${GTEST_SOURCE_DIR}/googletest/include"
    "${GTEST_SOURCE_DIR}/googlemock/include"
)

link_directories(
    ${google_link_directories}
)

enable_testing()

find_package(Threads)
function(gtest_add_test name)
    set(noValOptions)
    set(singleValOptions MAIN_SOURCE)
    set(multiValOptions)
    cmake_parse_arguments(${name}_opts "${noValOptions}" "${singleValOptions}" "${multiValOptions}" ${ARGN})
    if("${${name}_opts_MAIN_SOURCE}" STREQUAL "")
        set(google_libraries ${google_libraries} ${google_main_library})
    endif("${${name}_opts_MAIN_SOURCE}" STREQUAL "")
    add_executable(${name} ${${name}_opts_UNPARSED_ARGUMENTS} ${${name}_opts_MAIN_SOURCE})
    target_link_libraries(${name} ${google_libraries} ${CMAKE_THREAD_LIBS_INIT})
    set_property(TARGET ${name} APPEND PROPERTY INCLUDE_DIRECTORIES
        ${GTEST_INCLUDE_DIRS}
    )
    add_dependencies(${name} ${google_project_name})
    # Working directory: where the dlls are installed.
    pi_gtest_add_test(${name} ${gtest_args})
endfunction()
