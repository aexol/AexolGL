find_package(AexolGL)
include("${CMAKE_CURRENT_LIST_DIR}/AexolFBTargets.cmake")
if(IOS)
    find_package(FacebookSDK REQUIRED)
    get_target_property(_linkinterface AexolGL::AexolFB INTERFACE_LINK_LIBRARIES)
    set_target_properties(AexolGL::AexolFB PROPERTIES
        INTERFACE_LINK_LIBRARIES "-F${FacebookSDK_INCLUDE_DIR};${_linkinterface}")
    unset(_linkinterface)
endif()
