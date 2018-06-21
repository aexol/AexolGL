set(LOCAL_INCLUDES )
ADD_CURRENT_LIST_DIR(SRC_AUDIO_INCLUDE ${LOCAL_INCLUDES})
set(SRC_AUDIO_INCLUDE
    ${SRC_AUDIO_INCLUDE}
    include/aex/audio/AudioPlayer.hpp
    include/aex/audio/SoundSource.hpp
    include/aex/audio/SoundSourceHandler.hpp
)
set(_AUDIO
    AudioPlayer.cpp
    SoundSource.cpp
    SoundSourceHandler.cpp
    CMakeLists.txt
)
ADD_CURRENT_LIST_DIR(SRC_AUDIO ${_AUDIO})
source_group("src\\audio\\include" FILES ${SRC_AUDIO_INCLUDE})
source_group("src\\audio\\src" FILES ${SRC_AUDIO})
set_source_files_properties(CMakeLists.txt PROPERTIES HEADER_FILE_ONLY TRUE)
