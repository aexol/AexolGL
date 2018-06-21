set(LOCAL_INCLUDES )
ADD_CURRENT_LIST_DIR(SRC_ALGORITHMS_INCLUDE ${LOCAL_INCLUDES})
set(SRC_ALGORITHMS_INCLUDE
    ${SRC_ALGORITHMS_INCLUDE}
    include/aex/algorithms/PoissonDiscSampling.hpp
)
set(_ALGORITHMS 
    src/PoissonDiscSampling.cpp
    CMakeLists.txt
)
ADD_CURRENT_LIST_DIR(SRC_ALGORITHMS ${_ALGORITHMS})
source_group("src\\algorithms\\include" FILES ${SRC_ALGORITHMS_INCLUDE})
source_group("src\\algorithms\\src" FILES ${SRC_ALGORITHMS})
set_source_files_properties(CMakeLists.txt PROPERTIES HEADER_FILE_ONLY TRUE)
