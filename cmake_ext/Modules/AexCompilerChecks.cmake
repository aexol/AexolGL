include(CMakeParseArguments)
function(iphone_check_source_compile src lang var)
    if(NOT ${var})
        # Set flags depending on language.
        if("${lang}" STREQUAL "C")
            set(src_ext "c")
            set(extra_c_flags "${CMAKE_REQUIRED_FLAGS}")
            set(extra_cxx_flags "")
        elseif("${lang}" STREQUAL "CXX")
            set(src_ext "cxx")
            set(extra_c_flags "")
            set(extra_cxx_flags "${CMAKE_REQUIRED_FLAGS}")
        endif()

        # Prepare cmake temporary directory.
        set(_cmake_work_dir ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles)
        set(_cmake_tmp_dir ${_cmake_work_dir}/CMakeTmp)
        file(GLOB tmp_files RELATIVE ${_cmake_tmp_dir} ${_cmake_tmp_dir}/*)
        if(tmp_files)
            foreach(f ${tmp_files})
                file(REMOVE_RECURSE ${_cmake_tmp_dir}/${f})
            endforeach()
        endif()
        set(src_fname "src.${src_ext}")
        set(_cmake_lists_content "cmake_minimum_required(VERSION ${CMAKE_VERSION})
project(IOS_TRY_COMPILE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH})
set(CMAKE_C_FLAGS \"${extra_c_flags}\")
set(CMAKE_CXX_FLAGS \"${extra_cxx_flags}\")
add_library(tcmp SHARED ${src_fname})
")
        file(WRITE ${_cmake_tmp_dir}/CMakeLists.txt "${_cmake_lists_content}")
        file(WRITE ${_cmake_tmp_dir}/${src_fname} "${src}")
        execute_process(COMMAND cmake -G "${CMAKE_GENERATOR}" -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} .
                OUTPUT_VARIABLE itc_out1
                ERROR_VARIABLE itc_err1
                RESULT_VARIABLE rt
                WORKING_DIRECTORY ${_cmake_tmp_dir}
            )
        if("${rt}" STREQUAL "0")
            set(_build_cmd cmake --build .)
            execute_process(COMMAND ${_build_cmd}
                    OUTPUT_VARIABLE itc_out2
                    ERROR_VARIABLE itc_err2
                    RESULT_VARIABLE rt
                    WORKING_DIRECTORY ${_cmake_tmp_dir}
                )
        endif()
        if("${rt}" STREQUAL "0")
            set(${var} TRUE)
        else()
            set(${var} FALSE)
        endif()
        set(${var} ${${var}} CACHE BOOL "Test result for ${var}.")
        set(info_msg "Performing Test ${var} - ")
        if(${var})
            set(info_msg "${info_msg}Success")
        else()
            set(info_msg "${info_msg}Failed")
            file(APPEND ${_cmake_work_dir}/CMakeError.log ${itc_out1})
            file(APPEND ${_cmake_work_dir}/CMakeError.log ${itc_err1})
            file(APPEND ${_cmake_work_dir}/CMakeError.log ${itc_out2})
            file(APPEND ${_cmake_work_dir}/CMakeError.log ${itc_err2})
        endif()
        message(STATUS "${info_msg}")
    endif()
endfunction()

function(iphone_check_c_source_compile src var)
    iphone_check_source_compile("${src}" "C" ${var})
endfunction()

function(iphone_check_cxx_source_compile src var)
    iphone_check_source_compile("${src}" "CXX" ${var})
endfunction()

function(CheckCXX11SourceCompiles SOURCE VAR)
    set(old_CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS}")
    if(NOT MSVC)
        set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} -std=c++11")
    endif()
    include(CheckCXXSourceCompiles)
    if(NOT IOS)
        check_cxx_source_compiles("${SOURCE}" ${VAR})
    else()
        iphone_check_cxx_source_compile("${SOURCE}" ${VAR})
    endif()
    set(CMAKE_REQUIRED_FLAGS "${old_CMAKE_REQUIRED_FLAGS}")
endfunction()

function(CHECK_HAS_CONSTEXPR VAR)
    CheckCXX11SourceCompiles("int main(void)
    {
        constexpr int _a = 0;
        return _a;
    }" ${VAR})
endfunction()

function(CHECK_HAS_NOEXCEPT VAR)
    CheckCXX11SourceCompiles("int f() noexcept
    {
        return 0;
    }
    int main(void)
    {
        int _a = f();
        return _a;
    }" ${VAR})
endfunction()

function(CHECK_HAS_CODECVT VAR)
    CheckCXX11SourceCompiles("#include <locale>
    int main(void)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return 0;
    }" ${VAR})
endfunction()

function(CHECK_HAS_THREAD_LOCAL VAR)
    CheckCXX11SourceCompiles("int main(void)
    {
        thread_local int _a = 0;
        return _a;
    }" ${VAR})
endfunction()
