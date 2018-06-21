set(LOCAL_INCLUDES
    include/GLUniforms.hpp
    include/GLShaderProgram.hpp 
    include/GLVertexAttribArrays.hpp
    include/GLBuffers.hpp
    include/GLFramebuffer.hpp
    include/GLRenderbuffer.hpp
    include/GLContext.hpp
    include/GLTexture.hpp
   )
ADD_CURRENT_LIST_DIR(SRC_FRAMEWORK_GRAPHICS_API_INCLUDE ${LOCAL_INCLUDES})
set(SRC_FRAMEWORK_GRAPHICS_API_INCLUDE 
    ${SRC_FRAMEWORK_GRAPHICS_API_INCLUDE}
)

set(_FRAMEWORK_GRAPHICS_API
    src/GLUniforms.cpp
    src/GLShaderProgram.cpp
    src/GLVertexAttribArrays.cpp
    src/GLBuffers.cpp
    src/GLFramebuffer.cpp
    src/GLRenderbuffer.cpp
    src/GLContext.cpp
    src/GLTexture.cpp
    CMakeLists.txt
   )
ADD_CURRENT_LIST_DIR(SRC_FRAMEWORK_GRAPHICS_API ${_FRAMEWORK_GRAPHICS_API})

source_group("src\\framework\\graphics\\api\\include" FILES ${SRC_FRAMEWORK_GRAPHICS_API_INCLUDE})
source_group("src\\framework\\graphics\\api\\src" FILES ${SRC_FRAMEWORK_GRAPHICS_API})
set_source_files_properties(CMakeLists.txt PROPERTIES HEADER_FILE_ONLY TRUE)
