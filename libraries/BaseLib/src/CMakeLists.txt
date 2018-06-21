# Add sources that should be classified but someone is to lazy to do so.

set(LOCAL_INCLUDES )
ADD_CURRENT_LIST_DIR(SRC_FRAMEWORK_INCLUDE ${LOCAL_INCLUDES})
set(SRC_GROUP_INCLUDE
    include/aex/glLoadGen/gl_core_3_0.hpp
    include/aex/ax_Macros.hpp
    include/aex/ALoop.hpp
    include/aex/GameObject.hpp
    include/aex/GameObjectNode.hpp
    include/aex/Icosahedron.hpp
    include/aex/ThreadedLoop.hpp
    include/aex/QuaternionCamera.hpp
    #include/aex/MultiCamera.hpp
    include/aex/ObjectRenderNodeRaw.hpp
    include/aex/VRCamera.hpp

    #TODO: Files below need to be moved to correct CMakeLists.txt

    include/aex/Accelerometer.hpp
    include/aex/AexTypes.hpp
    include/aex/AexolGL.hpp
    include/aex/BasicEvents.hpp
    include/aex/BoundingSphere.hpp
    include/aex/BuildInEvents.hpp
    include/aex/ConvertVector.hpp
    include/aex/Common.hpp
    include/aex/EventRelayNode.hpp
    include/aex/FrameBuffer.hpp
    include/aex/FrameBufferOld.hpp
    include/aex/GameEvent.hpp
    include/aex/GameObjectInternalEvent.hpp
    include/aex/GetRawData.hpp
    include/aex/GlobalEvent.hpp
    include/aex/GraphicAPI.hpp
    include/aex/HandlerFunctionBase.hpp
#    include/aex/Hud.hpp
#    include/aex/HudElement.hpp
    include/aex/LinesDrw.hpp       
    include/aex/MemberFunctionHandler.hpp
    include/aex/Mesh.hpp
    include/aex/MeshExporter.hpp
    include/aex/MeshImporter.hpp
    include/aex/MockClock.hpp
    include/aex/MouseEvent.hpp
    include/aex/OverhauserSpline.hpp
    include/aex/PostEffect.hpp
    include/aex/ReadFromAexFile.hpp
    include/aex/Sprite.hpp
    include/aex/SpriteAnimated.hpp
    include/aex/SpriteFont.hpp
    include/aex/SubsystemGuard.hpp
    include/aex/TerrainHeightMap.hpp
    include/aex/TextDrw.hpp    
    include/aex/Timer.hpp
    include/aex/TreeNode.hpp
    include/aex/TriangulateShape.hpp
    include/aex/TypeInfo.hpp
    include/aex/api_definitions.hpp
    include/aex/ios_api_definitions.hpp
    include/aex/platform_definitions.hpp
    include/aex/Alloc.hpp
    include/aex/Resource.hpp
    include/aex/ResourceDirector.hpp
    include/aex/ReadWriteSemaphore.hpp
)

set(SRC_GROUP
    src/ALoop.cpp
    src/ThreadedLoop.cpp
    src/ax_Macros.cpp
    src/SaveToAexFile.cpp
    src/MeshExporter.cpp
    src/MeshImporter.cpp
    src/ReadFromAexFile.cpp
    src/TransformationImporter.cpp
    src/VRCamera.cpp
    src/ax_Macros.cpp
    src/Icosahedron.cpp
    src/QuaternionCamera.cpp
    src/CMakeLists.txt
)

include(${CMAKE_CURRENT_LIST_DIR}/algorithms/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/containers/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/framework/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/geometry/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/math/CMakeLists.txt)

source_group("include" FILES ${SRC_GROUP_INCLUDE})
source_group("src" FILES ${SRC_GROUP})
set_source_files_properties(src/CMakeLists.txt PROPERTIES HEADER_FILE_ONLY TRUE)
