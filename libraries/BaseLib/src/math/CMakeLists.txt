set(LOCAL_INCLUDES )
ADD_CURRENT_LIST_DIR(SRC_MATH_INCLUDE ${LOCAL_INCLUDES})
set(SRC_MATH_INCLUDE
    ${SRC_MATH_INCLUDE}
    #include/aex/aex::math.hpp
    include/aex/Math.hpp
    include/aex/Box.hpp
    include/aex/BBox2d.hpp
    include/aex/Line2D.hpp
    include/aex/Easing.hpp
    include/aex/Matrix.hpp      
    include/aex/Matrix3.hpp
    include/aex/Point.hpp
    include/aex/Quaternion.hpp
    include/aex/Vector2.hpp
    include/aex/Vector3.hpp
    include/aex/PODTypes.hpp
    include/aex/Point.hpp
)
set(_MATH
    src/aMath.cpp
    src/BBox2d.cpp
    src/Easing.cpp
    src/EasingStatic.cpp
    src/Line2D.cpp
    src/Matrix.cpp
    src/Matrix3.cpp
    src/Quaternion.cpp
    src/Vector2.cpp
    src/Vector3.cpp
    CMakeLists.txt
    )
ADD_CURRENT_LIST_DIR(SRC_MATH ${_MATH})

source_group("src\\math\\include" FILES ${SRC_MATH_INCLUDE})
source_group("src\\math\\src" FILES ${SRC_MATH}) 
set_source_files_properties(CMakeLists.txt PROPERTIES HEADER_FILE_ONLY TRUE)
