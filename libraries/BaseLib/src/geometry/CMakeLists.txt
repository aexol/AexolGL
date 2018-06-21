set(LOCAL_INCLUDES )
ADD_CURRENT_LIST_DIR(SRC_GEOMETRY_INCLUDE ${LOCAL_INCLUDES})
set(SRC_GEOMETRY_INCLUDE
    ${SRC_GEOMETRY_INCLUDE}
    include/aex/BoundingSphereObject.hpp
    include/aex/Indexer.hpp
    include/aex/Mesh.hpp
    include/aex/ObjectRenderNode.hpp
)
set(_GEOMETRY
    src/BoundingSphereObject.cpp
    src/CombinedMesh.cpp
    src/Indexer.cpp
    src/Mesh.cpp
    src/ObjectRenderNode.cpp
    src/ObjectRenderNodeRaw.cpp
    CMakeLists.txt
    )
ADD_CURRENT_LIST_DIR(SRC_GEOMETRY ${_GEOMETRY})

source_group("src\\geometry\\include" FILES ${SRC_GEOMETRY_INCLUDE})
source_group("src\\geometry\\src" FILES ${SRC_GEOMETRY}) 
source_group("src\\geometry" FILES CMakeLists.txt)
set_source_files_properties(CMakeLists.txt PROPERTIES HEADER_FILE_ONLY TRUE)
