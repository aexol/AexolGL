macro( ORIENTATIONS )
    foreach( orientation ${ARGN} )
        if(orientation STREQUAL "LANDSCAPE_LEFT")
            SET(LANDSCAPE_LEFT_ORIENTATION "UIInterfaceOrientationLandscapeLeft")
        endif(orientation STREQUAL "LANDSCAPE_LEFT")
        if(orientation STREQUAL "LANDSCAPE_RIGHT")
            SET(LANDSCAPE_RIGHT_ORIENTATION "UIInterfaceOrientationLandscapeRight")
        endif(orientation STREQUAL "LANDSCAPE_RIGHT")
        if(orientation STREQUAL "PORTRAIT")
            SET(PORTRAIT "UIInterfaceOrientationPortrait")
        endif(orientation STREQUAL "PORTRAIT")
        if(orientation STREQUAL "UPSIDE_DOWN")
            SET(UPSIDE_DOWN "UIInterfaceOrientationPortrait")
        endif(orientation STREQUAL "UPSIDE_DOWN")
    endforeach( orientation ${ARGN} )
endmacro( ORIENTATIONS )

macro(COMPILE_PYTHON_SOURCE_WITH_TARGET TARGET_NAME SDK_PATH SOURCE_PATH OUT_SUFFIX)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${SDK_PATH}/iOS/iPhoneOS/bin/host_python 
            -OO
            ${SDK_PATH}/cmake/tools/py_tools/compile_ios.py
            --sdk ${SDK_PATH}/iOS/iPhoneOS
            --src ${SOURCE_PATH}
            --out $<TARGET_FILE_DIR:${TARGET_NAME}>/${OUT_SUFFIX}
            )
endmacro()

include("${_helper_macros_dir}/dict.cmake")
# SYNTAX: APPEND_IPHONE_PLIST_STRING_VAR(key value
#       [DICTIONARY dict]
#       [PLIST_VAR var_name]
# By default function uses parent scoped dictionary by name IPHONE_PLIST_DICT to create
# new plist content for generated Info.plist.
# Function will update dictionary with ${key}:${value} pair and then for each pair in dict:
#       <key>${key}</key>
#       <string>${value}</string>
# will be added to the Info.plist. By default variable named IPHONE_INFO_PLIST_USER_VARS will
# be replaced in Info.plist.in template.
# DICTIONARY - Alternative name for parent scoped dictionary used in Info.plist generation.
# PLIST_VAR - Alternative variable name in Info.plist template.

macro(APPEND_IPHONE_PLIST_STRING_VAR key value)
    append_iphone_plist_var(${key} "<string>${value}</string>" ${ARGN})
endmacro()

# SYNTAX: APPEND_IPHONE_PLIST_VAR(key value
#       [DICTIONARY dict]
#       [PLIST_VAR var_name]
# By default function uses parent scoped dictionary by name IPHONE_PLIST_DICT to create
# new plist content for generated Info.plist.
# Function will update dictionary with ${key}:${value} pair and then for each pair in dict:
#       <key>${key}</key>
#       ${value}
# will be added to the Info.plist. By default variable named IPHONE_INFO_PLIST_USER_VARS will
# be replaced in Info.plist.in template.
# DICTIONARY - Alternative name for parent scoped dictionary used in Info.plist generation.
# PLIST_VAR - Alternative variable name in Info.plist template.
function(APPEND_IPHONE_PLIST_VAR key value)
    set(oneValueArgs DICTIONARY PLIST_VAR)
    cmake_parse_arguments(MY_INSTALL "" "${oneValueArgs}" "" ${ARGN} )
    if(NOT APPEND_IPHONE_PLIST_VAR_DICTIONARY)
        set(dict_name IPHONE_PLIST_DICT)
    else()
        set(dict_name ${APPEND_IPHONE_PLIST_VAR_DICTIONARY})
    endif()
    if(NOT APPEND_IPHONE_PLIST_VAR_PLIST_VAR)
        set(plist_var IPHONE_INFO_PLIST_USER_VARS)
    else()
        set(plist_var ${APPEND_IPHONE_PLIST_VAR_PLIST_VAR})
    endif()
    dict(SET ${dict_name} ${key} ${value})
    set(${dict_name} ${${dict_name}} PARENT_SCOPE)
    dict(GET_KEYS ${dict_name} keys)
    set(${plist_var} "")
    foreach(key ${keys})
        set(${plist_var} "${${plist_var}}<key>${key}</key>
        ${value}
        ")
    endforeach()
    set(${plist_var} ${${plist_var}} PARENT_SCOPE)
endfunction()
