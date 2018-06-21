# Add sources for ios platform.
#
set(LOCAL_INCLUDES 
    )
ADD_CURRENT_LIST_DIR(SRC_FRAMEWORK_PLATFORM_SPECIFIC_BOOST_INCLUDE)
set(SRC_FRAMEWORK_PLATFORM_SPECIFIC_BOOST_INCLUDE
    ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_BOOST_INCLUDE}
    )

# Available dummy implementations.
# For each implementation there must exist a list
# of files needed to build it. The name must match
# {IMPLEMENTATION_NAME}_files.
set(_boost_implementations
    FILESYSTEM_IMPLEMENTATION
    )
set(FILESYSTEM_IMPLEMENTATION_files  src/FileManagerBoost.cpp)
ADD_IMPLEMENTATIONS(_FRAMEWORK_PLATFORM_SPECIFIC_BOOST "boost" _boost_implementations)
ADD_CURRENT_LIST_DIR(SRC_FRAMEWORK_PLATFORM_SPECIFIC_BOOST ${_FRAMEWORK_PLATFORM_SPECIFIC_BOOST})
set(SRC_FRAMEWORK_PLATFORM_SPECIFIC_BOOST ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_BOOST} CMakeLists.txt)
source_group("src\\framework\\platform_specific\\boost\\include" FILES ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_BOOST_INCLUDE})
source_group("src\\framework\\platform_specific\\boost\\src" FILES ${SRC_FRAMEWORK_PLATFORM_SPECIFIC_BOOST})
set_source_files_properties(CMakeLists.txt PROPERTIES HEADER_FILE_ONLY TRUE)
