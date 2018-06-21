set(LOCAL_INCLUDES )
ADD_CURRENT_LIST_DIR(SRC_CONTAINERS_INCLUDE ${LOCAL_INCLUDES})
set(SRC_CONTAINERS_INCLUDE
    include/aex/HashEntry.hpp
    include/aex/HashTable.hpp
    include/aex/TextureManager.hpp
)
set(_CONTAINERS
    src/HashEntry.cpp
    src/HashTable.cpp
    src/TextureManager.cpp
    CMakeLists.txt
    )
ADD_CURRENT_LIST_DIR(SRC_CONTAINERS ${_CONTAINERS})

source_group("src\\containers\\include" FILES ${SRC_CONTAINERS_INCLUDE})
source_group("src\\containers\\src" FILES ${SRC_CONTAINERS})
set_source_files_properties(CMakeLists.txt PROPERTIES HEADER_FILE_ONLY TRUE)
