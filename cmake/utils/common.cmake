# Appends CMAKE_CURRENT_LIST_DIR to a list of sources.
# Returns new list specified by OUT.
macro(ADD_CURRENT_LIST_DIR OUT)
    if(NOT ${OUT})
        set(${OUT})
    endif()
    foreach(f ${ARGN})
        set(${OUT} ${${OUT}} "${CMAKE_CURRENT_LIST_DIR}/${f}")
    endforeach()
endmacro()

# This little macro lets you set any XCode specific property
macro (set_xcode_property TARGET XCODE_PROPERTY XCODE_VALUE)
    set_property (TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY} ${XCODE_VALUE})
endmacro (set_xcode_property)

#
# @brief Build file list for implementations.
#
# Compares if IDENTIFIER is equal to specific implementation identifier, if yes
# adds implementation files to list specified by OUT_VAR.
#
# @param OUT_VAR Output variable name.
# @param IDENTIFIER Implementation identifier (dummy, android, etc).
# @param IMPLEMENTATIONS_LIST List of implementations to check.
#
function(ADD_IMPLEMENTATIONS OUT_VAR IDENTIFIER IMPLEMENTATIONS_LIST)
    foreach(_dummy_impl ${${IMPLEMENTATIONS_LIST}})
        if("${${_dummy_impl}}" STREQUAL "${IDENTIFIER}")
            set(${OUT_VAR}
                ${${OUT_VAR}}
                ${${_dummy_impl}_files}
                )
        endif()
    endforeach()
    set(${OUT_VAR} ${${OUT_VAR}} PARENT_SCOPE)
endfunction()

function(_add_relacatable_interface TARGET FILENAME)
    get_filename_component(lib_name ${fn} NAME)
    if("${fn}" MATCHES "^/.*$")
        # Do not touch Xcode SDK paths and paths
        # starting with /usr/lib as those are usually system libs.
        if((APPLE AND "${fn}" MATCHES "^/Applications/Xcode\\.app.*$") 
            OR "${fn}" MATCHES "^/usr/lib.*")
            target_link_libraries(${TARGET} INTERFACE ${ARGN} ${fn}) 
        # Make NDK paths relative to user NDK path.
        elseif(ANDROID AND "${fn}" MATCHES "^${ANDROID_NDK}.*")
            get_filename_component(lib_name ${fn} NAME_WE)
            string(REGEX REPLACE "^lib(.*)$" "\\1" lib_name ${lib_name})
            target_link_libraries(${TARGET} INTERFACE ${ARGN} $<INSTALL_INTERFACE:${lib_name}>)
            target_link_libraries(${TARGET} INTERFACE ${ARGN} $<BUILD_INTERFACE:${fn}>)
        # Everything else is ASSUMED to be relative to install dir of target.
        else()
            target_link_libraries(${TARGET} INTERFACE ${ARGN} $<INSTALL_INTERFACE:\${_IMPORT_PREFIX}/lib/${lib_name}>)
            target_link_libraries(${TARGET} INTERFACE ${ARGN} $<BUILD_INTERFACE:${fn}>)
        endif()
    else()
        target_link_libraries(${TARGET} INTERFACE ${ARGN} "${lib_name}")
    endif()
endfunction()

function(add_relacatable_interface_libraries TARGET)
    foreach(fn ${ARGN})
        if("${fn}" MATCHES "^(optimized|debug)$")
            set(sw ${fn})
        else()
            _add_relacatable_interface(${TARGET} ${fn} ${sw})
            unset(sw)
        endif()
    endforeach()
endfunction()

if(ANDROID)
    function(install_aar_name prefix out)
        set(${out} ${prefix})
        set(${out} "${${out}}$<$<CONFIG:Debug>:debug>")
        set(${out} "${${out}}$<$<CONFIG:Release>:release>")
        set(${out} "${${out}}$<$<CONFIG:MinSizeRel>:minSizeRel>")
        set(${out} "${${out}}$<$<CONFIG:RelWithDebInfo>:relWithDebInfo>")
        set(${out} "${${out}}.aar" PARENT_SCOPE)
    endfunction()
endif()
