set(LOCAL_INCLUDES )
ADD_CURRENT_LIST_DIR(SRC_UTILS_INCLUDE ${LOCAL_INCLUDES})
set(SRC_UTILS_INCLUDE
    ${SRC_UTILS_INCLUDE}
    include/aex/utils/Connection.hpp
)
set(_UTILS
    Connection.cpp
)
ADD_CURRENT_LIST_DIR(SRC_UTILS ${_UTILS})
source_group("src\\utils\\include" FILES ${SRC_UTILS_INCLUDE})
source_group("src\\utils\\src" FILES ${SRC_UTILS})
set_source_files_properties(CMakeLists.txt PROPERTIES HEADER_FILE_ONLY TRUE)
