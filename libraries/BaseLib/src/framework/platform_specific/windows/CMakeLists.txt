# Add sources for ios platform.
#
set(LOCAL_INCLUDES 
    )
ADD_CURRENT_LIST_DIR(SRC_FRAMEWORK_PLATFORM_SPECIFIC_WINDOWS_INCLUDE)
set(SRC_FRAMEWORK_PLATFORM_SPECIFIC_WINDOWS_INCLUDE
    ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_WINDOWS_INCLUDE}
    )

# Available dummy implementations.
# For each implementation there must exist a list
# of files needed to build it. The name must match
# {IMPLEMENTATION_NAME}_files.
if(WIN32)
    set(DYNLOAD_IMPLEMENTATION "windows")
    set(_windows_implementations
        DYNLOAD_IMPLEMENTATION
        )
    set(DYNLOAD_IMPLEMENTATION_files  src/Dynload.cpp)
    ADD_IMPLEMENTATIONS(_FRAMEWORK_PLATFORM_SPECIFIC_WINDOWS "windows" _windows_implementations)
    if(HAVE_GL_LOAD_GEN)
        set(_FRAMEWORK_PLATFORM_SPECIFIC_WINDOWS ${_FRAMEWORK_PLATFORM_SPECIFIC_WINDOWS} src/gl_core_3_0.cpp)
    endif(HAVE_GL_LOAD_GEN)
    ADD_CURRENT_LIST_DIR(SRC_FRAMEWORK_PLATFORM_SPECIFIC_WINDOWS ${_FRAMEWORK_PLATFORM_SPECIFIC_WINDOWS})

else()
    set(SRC_FRAMEWORK_PLATFORM_SPECIFIC_WINDOWS "")
endif()
set(SRC_FRAMEWORK_PLATFORM_SPECIFIC_WINDOWS ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_WINDOWS} CMakeLists.txt)
source_group("src\\framework\\platform_specific\\windows\\include" FILES ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_WINDOWS_INCLUDE})
source_group("src\\framework\\platform_specific\\windows\\src" FILES ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_WINDOWS})
set_source_files_properties(CMakeLists.txt PROPERTIES HEADER_FILE_ONLY TRUE)
