include(CMakeParseArguments)
set(android_macros_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})
set(__default_gradle_version "3.3")
macro( find_host_package )
    set( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER )
    set( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER )
    set( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER )
    if( CMAKE_HOST_WIN32 )
        SET( WIN32 1 )
        SET( UNIX )
    elseif( CMAKE_HOST_APPLE )
        SET( APPLE 1 )
        SET( UNIX )
    endif()
    find_package( ${ARGN} )
    SET( WIN32 )
    SET( APPLE )
    SET( UNIX 1 )
    set( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY )
    set( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY )
    set( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY )
endmacro()

macro (find_host_external_project)
    set (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
    set (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
    if( CMAKE_HOST_WIN32 )
        SET( WIN32 1 )
        SET( UNIX )
    elseif( CMAKE_HOST_APPLE )
        SET( APPLE 1 )
        SET( UNIX )
    endif()
    ExternalProject_Add(${ARGN})
    SET( WIN32 )
    SET( APPLE )
    SET( UNIX 1 )
    set (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY)
    set (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endmacro (find_host_external_project)


# macro to find programs on the host OS
macro( find_host_program )
    set( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER )
    set( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER )
    set( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER )
    if( CMAKE_HOST_WIN32 )
        SET( WIN32 1 )
        SET( UNIX )
    elseif( CMAKE_HOST_APPLE )
        SET( APPLE 1 )
        SET( UNIX )
    endif()
    find_program( ${ARGN} )
    SET( WIN32 )
    SET( APPLE )
    SET( UNIX 1 )
    set( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY )
    set( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY )
    set( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY )
endmacro()
find_host_package(PythonInterp REQUIRED)
#
# Variables modifing script behaviour.
# ANDROID_ROOT_PACKAGE - Root package name.
# ANDROID_ROOT_BUILD_GRADLE - Optional. Replace default root build.gradle with 
#                     file pointed by this path. Relative to CMAKE_SOURCE_DIR.
# ANDROID_ROOT_SETTINGS_GRADLE - Optional. Replace default root settings.gradle with
#                     file pointed by this path. Relative to CMAKE_SOURCE_DIR.
# ANDROID_ROOT_GRADLE_PROPERTIES - Optional. Replace default root gradle.properties with
#                     file pointed by this path. Relative to CMAKE_SOURCE_DIR.
# ANDROID_ROOT_MANIFEST - Optional. Replace default root AndroidManifest.xml with
#                     file pointed by this path. Relative to CMAKE_SOURCE_DIR.
# ANDROID_ROOT_CMAKE_PROPERTIES - Optional. Replace default root
#                     cmake-project.properties.xml with file pointed 
#                     by this path. Relative to CMAKE_SOURCE_DIR.
# ANDROID_SDK_API_VERSION - compileSdkVersion. Default 15.
# ANDROID_MIN_SDK_API_VERSION - compileSdkVersion. Default ANDROID_SDK_API_VERSION
# ANDROID_TARGET_SDK_API_VERSION - compileSdkVersion. Default ANDROID_SDK_API_VERSION
# ANDROID_BUILD_TOOLS_VERSION - buildToolsVersion. Default try to autodetect.
#
if(NOT ANDROID_ROOT_PACKAGE)
    set(ANDROID_ROOT_PACKAGE com.example)
endif()
if(NOT ANDROID_SDK_API_VERSION)
    set(ANDROID_SDK_API_VERSION 15)
endif()
if(NOT ANDROID_MIN_SDK_API_VERSION)
    set(ANDROID_MIN_SDK_API_VERSION ${ANDROID_SDK_API_VERSION})
endif()
if(NOT ANDROID_TARGET_SDK_API_VERSION)
    set(ANDROID_TARGET_SDK_API_VERSION ${ANDROID_SDK_API_VERSION})
endif()

macro(get_gradle_exe var)
    set(${var} "${CMAKE_BINARY_DIR}/gradlew")
endmacro()

function( find_android_target_sdk _ANDROID_SDK_PATH )
    if( NOT ANDROID_TARGET_SDK )
        file( GLOB __ANDROID_PLATFORMS RELATIVE ${_ANDROID_SDK_PATH}/platforms/*
            ${_ANDROID_SDK_PATH}/platforms/*
            )
        foreach( f ${__ANDROID_PLATFORMS})
            get_filename_component( fName ${f} NAME )
            string( REGEX REPLACE "android-([0-9]+)" "\\1" found_platform "${fName}"  )
            set( _platforms ${_platforms} ${found_platform} )
        endforeach( f ${__ANDROID_PLATFORMS})
        if( NOT _platforms)
            MESSAGE( FATAL_ERROR "No valid Android SDK target found, check your ANDROID_SDK path or provide one manually by defining ANDROID_TARGET_SDK")
        endif( NOT _platforms)
        set( platform 1 )
        foreach( _platform ${_platforms} )
            if( ${_platform} GREATER ${platform} )
                set( platform ${_platform} )
            endif( ${_platform} GREATER ${platform} )
        endforeach( _platform ${_platforms} )
        SET( ANDROID_TARGET_SDK ${platform} CACHE STRING "Android SDK version.")
    endif( NOT ANDROID_TARGET_SDK )
endfunction( find_android_target_sdk _ANDROID_SDK_PATH )

macro(build_tools_paths __PATHS)
    file(GLOB __paths_temp  "${ANDROID_SDK}/build-tools/*")
    foreach(dn ${__paths_temp})
        if(IS_DIRECTORY "${dn}")
            SET(${__PATHS} ${${__PATHS}} "${dn}")
        endif(IS_DIRECTORY "${dn}")
    endforeach(dn ${__paths_temp})
    list(REVERSE ${__PATHS})
endmacro(build_tools_paths __PATHS)

macro(_parse_args)
    # Parse args
    set(noValOptions
        SHARED
        STATIC
        SKIP_RELEASE_SIGN)

    set(singleValOptions 
        JAVA_SOURCES_ROOT
        BUILD_TOOLS_VERSION
        GRADLE_VERSION
        GRADLE_PLUGIN_VERSION
        ASSETS_ROOT
        RESOURCES_ROOT
        ANDROID_MANIFEST_IN
        SETTINGS_GRADLE_IN
        ANDROID_DIR_SUFFIX
        BUILD_GRADLE_IN
        ANDROID_SDK_API_VERSION
        ANDROID_MIN_SDK_API_VERSION
        ANDROID_TARGET_SDK_API_VERSION
        PROGUARD_FILE
        ARTIFACT_ID
        GROUP_ID
        CMAKE_LISTS_FOR_NDK
        BUILD_GRADLE_EXTRA
        REPOSITORIES
        BUILD_SCRIPT
        PACKAGE_NAME
        ORIENTATION
        MANIFEST_EXTRA
        MANIFEST_APPLICATION_EXTRA
        MANIFEST_APPLICATION_ACTIVITY_EXTRA
        )

    set(multiValOptions
        ASSETS
        GRADLE_PLUGINS
        RESOURCES
        JAVA_SOURCES
        DEPENDENCIES
        EXTRA_PROPERTIES
        PERMISSIONS
        )
    cmake_parse_arguments(OPTS "${noValOptions}" "${singleValOptions}" "${multiValOptions}" ${ARGN})
    if(NOT OPTS_REPOSITORIES)
        set(OPTS_REPOSITORIES "jcenter()\n")
        if(NOT EXISTS ${CMAKE_SOURCE_DIR}/cmake_ext)
            set(OPTS_REPOSITORIES "${OPTS_REPOSITORIES}
    def cmakeProps = new Properties()
    cmakeProps.load(new FileInputStream(project.file(\"cmake-project.properties\")))
    def f = new File(cmakeProps.aexolSDK, \"lib/cmake/android_gradle/support\")
    if(f.exists()) {
        flatDir {
            dirs f
        }
    }
    ")
        endif()
    endif()
    if(NOT OPTS_BUILD_SCRIPT)
        set(OPTS_BUILD_SCRIPT "def f = null
    def cmakeProps = new Properties()
    cmakeProps.load(new FileInputStream(project.file(\"cmake-project.properties\")))
    ")
    if(EXISTS ${CMAKE_SOURCE_DIR}/cmake_ext)
        set(OPTS_BUILD_SCRIPT "${OPTS_BUILD_SCRIPT}if(cmakeProps?.projectSourceDir) {
        f = new File(cmakeProps.projectSourceDir, \"cmake_ext\")
    }
    else ")
    endif()
    set(OPTS_BUILD_SCRIPT "${OPTS_BUILD_SCRIPT}if(cmakeProps?.aexolSDK) {
        f = new File(cmakeProps.aexolSDK, \"lib/cmake\")
    }
    if(f) {
        f = new File(f, \"android_gradle/support\")
    }
    repositories {
        jcenter()
        if(f.exists()) {
            flatDir {
                dirs f
            }
        }
    }

    dependencies {
        classpath 'com.aexol.android:gradle:1.0'
    }")
    endif()
    if(NOT OPTS_ORIENTATION)
        set(OPTS_ORIENTATION "landscape")
    endif()
    set(_p "\n")
    foreach(p ${OPTS_PERMISSIONS})
        set(_p "${_p}\t<uses-permission android:name=\"${p}\" />\n")
    endforeach()
    set(OPTS_PERMISSIONS "${_p}")
endmacro()

function(GRADLE_INIT)
    get_gradle_exe(gradlew_exe)
    set(gradle_version ${__default_gradle_version})
    if(NOT "${ARG0}" STREQUAL "")
        set(gradle_version ${ARG0})
    endif(NOT "${ARG0}" STREQUAL "")

    if(NOT EXISTS ${gradlew_exe})
        set(gradle_exe gradle)
        if(WIN32)
            set(gradle_exe gradle.bat)
        endif()
        set(gradle_root ${CMAKE_BINARY_DIR}/.gradle)
        set(gradle_path ${gradle_root}/gradle-${gradle_version}/)
        set(gradle_exe ${gradle_path}/bin/${gradle_exe})
        if(NOT EXISTS ${gradle_exe})
            if(NOT EXISTS ${gradle_root})
                file(MAKE_DIRECTORY ${gradle_root})
            endif()
            file(DOWNLOAD 
                https://services.gradle.org/distributions/gradle-${gradle_version}-all.zip
                ${gradle_root}/gradle-${gradle_version}-all.zip
                STATUS status
                SHOW_PROGRESS
                )
            list(GET status 0 _status_code)
            list(GET status 1 _status_message)
            if(NOT "${_status_code}" STREQUAL "0")
                message(FATAL_ERROR ${_status_message})
            endif()
            execute_process(COMMAND cmake -E tar x gradle-${gradle_version}-all.zip
                WORKING_DIRECTORY ${gradle_root}
                RESULT_VARIABLE _status_code
                ERROR_VARIABLE _error_message
                )
            if(NOT "${_status_code}" EQUAL 0)
                message(FATAL_ERROR ${_error_message})
            endif()
        endif()
        execute_process(COMMAND ${gradle_exe} init
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            OUTPUT_VARIABLE __CREATE_OUTPUT
            ERROR_VARIABLE __CREATE_ERROR
            RESULT_VARIABLE _res
            )
        file(REMOVE ${CMAKE_BINARY_DIR}/build.gradle)
        file(REMOVE ${CMAKE_BINARY_DIR}/settings.gradle)
        if(NOT "${_res}" EQUAL "0")
            message(FATAL_ERROR ${__CREATE_ERROR})
        endif()
    endif()
    _gen_root_project_files()
endfunction()

function(_gen_root_project_file FILE_NAME SUFFIX DEFAULT_CONTETNT GENERATE)
    set_target_properties(_internal_gradle_tgt
        PROPERTIES generate_${SUFFIX} "${GENERATE}"
            content_${SUFFIX} "${DEFAULT_CONTETNT}"
        )
    file(GENERATE OUTPUT ${FILE_NAME}
        CONTENT "$<TARGET_PROPERTY:_internal_gradle_tgt,content_${SUFFIX}>"
        CONDITION "$<TARGET_PROPERTY:_internal_gradle_tgt,generate_${SUFFIX}>"
        )
endfunction()

function(_gen_root_project_files)
    add_custom_target(_internal_gradle_tgt)
    set(root_build_gradle ${CMAKE_BINARY_DIR}/build.gradle)
    set(root_settings_gradle ${CMAKE_BINARY_DIR}/settings.gradle)
    set(root_gradle_properties ${CMAKE_BINARY_DIR}/gradle.properties)
    set(root_local_properties ${CMAKE_BINARY_DIR}/local.properties)
    set(root_cmake_properties ${CMAKE_BINARY_DIR}/cmake-project.properties)
    set(root_manifest_path ${CMAKE_BINARY_DIR}/src/main)
    set(root_manifest ${root_manifest_path}/AndroidManifest.xml)
    set(ANDROID_GRADLE_BASE_DIR "${android_macros_LIST_DIR}/../android_gradle")
    if(ANDROID_ROOT_BUILD_GRADLE)
        file(READ 
            ${CMAKE_SOURCE_DIR}/${ANDROID_ROOT_BUILD_GRADLE}
            build_gradle_content)
    else()
        set(target_type library)
        set(build_gradle_content "// Top-level build file where you can add configuration options common to all sub-projects/modules.

buildscript {
    repositories {
        jcenter()
    }
    dependencies {
        classpath 'com.android.tools.build:gradle:2.3.3'

        // NOTE: Do not place your application dependencies here; they belong
        // in the individual module build.gradle files
    }
}

allprojects {
    repositories {
        jcenter()
    }
}

task clean(type: Delete) {
    delete rootProject.buildDir
}
"
        )
    endif()
    if(NOT EXISTS "${root_build_gradle}")
        _gen_root_project_file(${root_build_gradle}
            BUILD_GRADLE
            "${build_gradle_content}"
            1
            )
    endif()

    set(SETTINGS_GENERATE 1)
    if(ANDROID_ROOT_SETTINGS_GRADLE)
        execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/${ANDROID_ROOT_SETTINGS_GRADLE} ${root_settings_gradle}
            )
        set(SETTINGS_GENERATE 0)
    endif()
    if(NOT EXISTS "${root_build_gradle}")
        _gen_root_project_file(${root_settings_gradle}
            SETTINGS_GRADLE
            ""
            ${SETTINGS_GENERATE}
        )
    endif()
    if(ANDROID_ROOT_GRADLE_PROPERTIES)
        execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/${ANDROID_ROOT_GRADLE_PROPERTIES} ${root_gradle_properties}
            )
    endif()
    if(NOT EXISTS ${root_gradle_properties})
        # Silence annoying android plugin ndk/sdk detection.
        _check_android_xdk(NDK)
        _check_android_xdk(SDK)
        file(APPEND ${root_local_properties} "ndk.dir=${ANDROID_NDK}\n")
        file(APPEND ${root_local_properties} "sdk.dir=${ANDROID_SDK}\n")
    endif()
    if(NOT EXISTS ${root_manifest_path})
        file(MAKE_DIRECTORY ${root_manifest_path})
    endif()
    if(ANDROID_ROOT_MANIFEST)
        execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/${ANDROID_ROOT_MANIFEST} ${root_manifest}
            )
    endif()
    if(NOT EXISTS ${root_manifest})
        file(WRITE ${root_manifest} "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n")
        file(APPEND ${root_manifest} "<manifest xmlns:android=\"http://schemas.android.com/apk/res/android\"\n")
        file(APPEND ${root_manifest} "        package=\"${ANDROID_ROOT_PACKAGE}\"\n")
        file(APPEND ${root_manifest} "        android:versionCode=\"1\"\n")
        file(APPEND ${root_manifest} "        android:versionName=\"1.0\"\n")
        file(APPEND ${root_manifest} "        android:installLocation=\"auto\">\n")
        file(APPEND ${root_manifest} "</manifest>\n")
    endif()
    if(ANDROID_ROOT_CMAKE_PROPERTIES)
        execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/${ANDROID_ROOT_CMAKE_PROPERTIES} ${root_cmake_properties}
            )
    endif()
    if(NOT EXISTS ${root_cmake_properties})
        set(DIR ${CMAKE_BINARY_DIR})
        # Minimal props for root project
        _write_line_to_props("# This file was auto generated by CMake. DO NOT EDIT.\n# For customizing behaviour use gradle.properties or build.gradle.")
        _check_set_append(projectCmake "${CMAKE_COMMAND}")
        if("${CMAKE_GENERATOR}" STREQUAL "Unix Makefiles" OR
                "${CMAKE_GENERATOR}" STREQUAL "Ninja")
            # 8 Jobs for make/Ninja
            set(makeCmd ${CMAKE_COMMAND} --build . -- -j8 )
        else()
            set(makeCmd ${CMAKE_COMMAND} --build . )
        endif()
        _check_set_append(projectMake ${makeCmd})
        _check_set_append(projectDir ${CMAKE_BINARY_DIR})
        _check_set_append(projectSourceDir ${CMAKE_SOURCE_DIR})
        if(NOT "${CMAKE_PROJECT_NAME}" STREQUAL "AexolGL")
            _check_set_append(aexolSDK ${AEXOL_SDK})
            _check_set_append(aexlibMavenRepoDir ${AEXLIB_ANDROID_DIR})
        endif()
        _check_set_append(compileSdkVersion ${ANDROID_SDK_API_VERSION})
        _get_build_tools("" bt_used)
        _check_set_append(buildToolsVersion ${bt_used})
        _check_set_append(minSdkVersion ${ANDROID_MIN_SDK_API_VERSION})
        _check_set_append(targetSdkVersion ${ANDROID_TARGET_SDK_API_VERSION})
        # Append extra properties to cmake-project.properties.
    endif()
endfunction()

function(_add_subproject subproject)
    file(RELATIVE_PATH subproject ${CMAKE_BINARY_DIR} ${subproject})
    # If subproject is empty, root project
    # has contents. Do not generate default configuration
    # in that case.
    if("${subproject}" STREQUAL "")
        set_target_properties(_internal_gradle_tgt
            PROPERTIES generate_SETTINGS_GRADLE 0)
        if(NOT ANDROID_ROOT_BUILD_GRADLE)
            set_target_properties(_internal_gradle_tgt
                PROPERTIES generate_BUILD_GRADLE 0)
        endif()
    endif()

    if(NOT "${subproject}" STREQUAL "")
        string(REPLACE "/" ":" subproject ${subproject})
        get_target_property(gradle_subprojects 
            _internal_gradle_tgt 
            content_SETTINGS_GRADLE)
        set_target_properties(_internal_gradle_tgt
            PROPERTIES
            content_SETTINGS_GRADLE "${gradle_subprojects}include \"${subproject}\"\n")
    endif()
endfunction()

function(_get_build_tools PREFERED_BT OUTPUT)
    # Find build_tools paths and version of used bt.

    if(NOT "${PREFERED_BT}" STREQUAL "")
        set(bt_used ${PREFERED_BT})
    elseif(ANDROID_BUILD_TOOLS_VERSION)
        set(bt_used ${ANDROID_BUILD_TOOLS_VERSION})
    else()
        build_tools_paths(bt_paths)
        list(LENGTH bt_paths build_tools_count)
        if(build_tools_count EQUAL 0)
            FATAL_ERROR("No build tools.")
        endif(build_tools_count EQUAL 0)
        list(GET bt_paths 0 bt_used)
        get_filename_component(bt_used "${bt_used}" NAME)
    endif()

    set(${OUTPUT} ${bt_used} PARENT_SCOPE)
endfunction()

function(_prepare_gradle ANDROID_PROJECT_DIR)
    _parse_args(${ARGN})
    file(MAKE_DIRECTORY ${ANDROID_PROJECT_DIR})
    _get_build_tools("${OPTS_BUILD_TOOLS_VERSION}" bt_used)
    set(bt_used ${bt_used} PARENT_SCOPE)
endfunction()

function(_get_build_gradle_in BUILD_GRADLE_VAR TEMPLATE_ROOT)
    _parse_args(${ARGN})
    set(${BUILD_GRADLE_VAR} ${TEMPLATE_ROOT}/build.gradle.in)
    if(OPTS_BUILD_GRADLE_IN)
        set(${BUILD_GRADLE_VAR} ${CMAKE_CURRENT_SOURCE_DIR}/${OPTS_BUILD_GRADLE_IN})
    endif()
    set(${BUILD_GRADLE_VAR} ${${BUILD_GRADLE_VAR}} PARENT_SCOPE)
endfunction()

function(_get_manifest_in manifest_var template_root)
    _parse_args(${ARGN})
    set(${manifest_var} ${template_root}/AndroidManifest.xml.in)
    if(OPTS_ANDROID_MANIFEST_IN)
        set(${manifest_var} ${CMAKE_CURRENT_SOURCE_DIR}/${OPTS_ANDROID_MANIFEST_IN})
    endif()
    set(${manifest_var} ${${manifest_var}} PARENT_SCOPE)
endfunction()

function(_get_activity_in activity_var template_root)
    _parse_args(${argn})
    set(${activity_var} ${template_root}/ANDROID_ACTIVITY/ANDROID_ACTIVITY.java.in)
    if(OPTS_ANDROID_ACTIVITY_IN)
        set(${activity_var} ${cmake_current_source_dir}/${OPTS_ANDROID_ACTIVITY_IN})
    endif()
    set(${activity_var} ${${activity_var}} PARENT_SCOPE)
endfunction()

# Usage:
# TEMPLATE_DIR - template root
# TARGET_DIR - result root
# followed by pairs [<src> <dst>] of template file and result file
# src is relative to template root and dst to result root
function(_gen_with_template TEMPLATE_DIR TARGET_DIR)
    set(args ${ARGN})
    list(LENGTH args count)
    math(EXPR mod "${count} % 2")
    if(NOT "${mod}" EQUAL "0")
        message(FATAL_ERROR "Invalid usage.")
    endif()
    foreach(idx RANGE 1 ${count} 2)
        math(EXPR prev_idx "${idx} - 1")
        list(GET args ${prev_idx} src)
        list(GET args ${idx} dst)
        configure_file( "${src}" "${TARGET_DIR}/${dst}" @ONLY)
    endforeach()
endfunction()

function(_to_props_file ACTION line)
    set(cmake_props ${DIR}/cmake-project.properties)
    file(${ACTION} ${cmake_props} "${line}\n")
endfunction()

function(_write_line_to_props line)
    _to_props_file(WRITE "${line}")
endfunction()

function(_append_line_to_props line)
    _to_props_file(APPEND "${line}")
endfunction()

function(_check_set_append propName)
    if(NOT "${ARGN}" STREQUAL "")
        _append_line_to_props("${propName}=${ARGN}")
    endif()
endfunction()

function(_make_cmake_project_properties DIR)
    _write_line_to_props("# This file was auto generated by CMake. DO NOT EDIT.\n# For customizing behaviour use gradle.properties or build.gradle.")
    _check_set_append(projectCmake "${CMAKE_COMMAND}")
    if("${CMAKE_GENERATOR}" STREQUAL "Unix Makefiles" OR
            "${CMAKE_GENERATOR}" STREQUAL "Ninja")
        # 8 Jobs for make/Ninja
        set(makeCmd ${CMAKE_COMMAND} --build . -- -j8 )
    else()
        set(makeCmd ${CMAKE_COMMAND} --build . )
    endif()
    set(ANDROID_DEBUG "false")
    string(TOLOWER "${CMAKE_BUILD_TYPE}" lcase_build_type)
    if("${lcase_build_type}" STREQUAL "debug")
        seT(ANDROID_DEBUG "true")
    endif()
    _check_set_append(projectMake ${makeCmd})
    _check_set_append(projectDir ${CMAKE_BINARY_DIR})
    _check_set_append(projectSourceDir ${CMAKE_SOURCE_DIR})
    file(RELATIVE_PATH rel ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR})
    _check_set_append(projectCurrentBinaryDir ${rel})
    file(RELATIVE_PATH rel ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
    _check_set_append(projectCurrentSourceDir ${rel})
    _check_set_append(projectLibraryDir ${JNI_LIBS_DIR})
    _check_set_append(projectAssetsRoot ${OPTS_ASSETS_ROOT})
    _check_set_append(projectAssets ${OPTS_ASSETS})
    _check_set_append(projectResourcesRoot ${OPTS_RESOURCES_ROOT})
    _check_set_append(projectResources ${OPTS_RESOURCES})
    _check_set_append(projectJavaSourcesRoot ${OPTS_JAVA_SOURCES_ROOT})
    _check_set_append(projectJavaSources ${OPTS_JAVA_SOURCES})
    # Skipping AEXOL_SDK and Maven repo for project build.
    if(NOT "${CMAKE_PROJECT_NAME}" STREQUAL "AexolGL")
        _check_set_append(aexolSDK ${AEXOL_SDK})
        _check_set_append(aexlibMavenRepoDir ${AEXLIB_ANDROID_DIR})
    endif()
    _check_set_append(projectSkipReleaseSign ${ANDROID_SKIP_RELEASE_SIGN})
    _check_set_append(compileSdkVersion ${ANDROID_SDK_API_VERSION})
    _check_set_append(buildToolsVersion ${bt_used})
    _check_set_append(minSdkVersion ${ANDROID_MIN_SDK_API_VERSION})
    _check_set_append(targetSdkVersion ${ANDROID_TARGET_SDK_API_VERSION})
    _check_set_append(proguardFile ${OPTS_PROGUARD_FILE})
    _check_set_append(artifactId ${OPTS_ARTIFACT_ID})
    _check_set_append(groupId ${OPTS_GROUP_ID})
    _check_set_append(cmakeListsForNdk ${OPTS_CMAKE_LISTS_FOR_NDK})
    # Append extra properties to cmake-project.properties.
    if(OPTS_EXTRA_PROPERTIES)
        list(LENGTH OPTS_EXTRA_PROPERTIES _extra_properties_len)
        math(EXPR _extra_properties_len "${_extra_properties_len}-1")
        foreach(idx RANGE 0 ${_extra_properties_len} 2)
            math(EXPR idx_next "${idx}+1")
            list(GET OPTS_EXTRA_PROPERTIES ${idx} _property_name)
            list(GET OPTS_EXTRA_PROPERTIES ${idx_next} _property_value)
            _check_set_append(${_property_name} ${_property_value})
        endforeach()
    endif()
endfunction()

macro(_check_android_xdk SUFFIX)
    string(TOLOWER ${SUFFIX} suffix)
    if(NOT ANDOIRD_${SUFFIX})
        if(NOT "$ENV{ANDROID_${SUFFIX}}" STREQUAL "")
            set(ANDROID_${SUFFIX} $ENV{ANDROID_${SUFFIX}}
                CACHE STRING "Android ${suffix} path.")
        else()
            message(FATAL_ERROR "Android ${SUFFIX} path not set.\n Either set ANDROID_${SUFFIX} variable or ANDROID_${SUFFIX} environment variable.")
        endif()
    endif()
endmacro()

macro(_check_sdk_levels)
    # Options passed as arguments always have priority.
    if(OPTS_ANDROID_SDK_API_VERSION)
        set(ANDROID_SDK_API_VERSION ${OPTS_ANDROID_SDK_API_VERSION})
    endif()

    if(OPTS_ANDROID_MIN_SDK_API_VERSION)
        set(ANDROID_MIN_SDK_API_VERSION ${OPTS_ANDROID_MIN_SDK_API_VERSION})
    endif()

    if(OPTS_ANDROID_TARGET_SDK_API_VERSION)
        set(ANDROID_TARGET_SDK_API_VERSION ${OPTS_ANDROID_TARGET_SDK_API_VERSION})
    elseif(NOT OPTS_ANDROID_TARGET_SDK_API_VERSION)
        set(ANDROID_TARGET_SDK_API_VERSION ${ANDROID_SDK_API_VERSION})
    endif()
endmacro()

function(_make_android_project TYPE ANAME)
    # Parse args
    _parse_args(${ARGN})
    _check_sdk_levels()
    SET(__android_project_dir "${CMAKE_CURRENT_BINARY_DIR}/${OPTS_ANDROID_DIR_SUFFIX}")
    _add_subproject(${__android_project_dir})

    get_target_property(gradle_subprojects _internal_gradle_tgt content)
    set(_gradle_subprojects ${_gradle_subprojects} ${__android_project_dir} CACHE INTERNAL "")
    #set(_gradle_subprojects ${_gradle_subprojects} CACHE INTERNAL "gradle subprojects")
    # Extra opts for cmake properties to skip
    # signing on release build.
    # Useful for profiling.
    if(OPTS_SKIP_RELEASE_SIGN EQUAL TRUE)
        set(ANDROID_SKIP_RELEASE_SIGN true)
    else()
        set(ANDROID_SKIP_RELEASE_SIGN false)
    endif()

    # Deduce project type.
    if("${TYPE}" STREQUAL "APP")
        set(_app TRUE)
        set(target_type app)
    elseif("${TYPE}" STREQUAL "LIBRARY")
        set(_app FALSE)
        set(target_type library)
    else()
        message(FATAL_ERROR "Unsupported type.")
    endif()
    if(OPTS_STATIC AND OPTS_SHARED)
        message(FATAL_ERROR "STATIC and SHARED library types are mutually exclusive.")
    endif()
    if(OPTS_STATIC)
        set(LIBRARY_TYPE STATIC)
    elseif(OPTS_SHARED)
        set(LIBRARY_TYPE SHARED)
    endif()

    if(${_app})
        set(_template_suffix "user_project_template")
    else()
        set(_template_suffix "library_project_template")
    endif()
    set(ANDROID_GRADLE_BASE_DIR "${android_macros_LIST_DIR}/../android_gradle")
    set(ANDROID_TEMPLATE_DIR "${ANDROID_GRADLE_BASE_DIR}/${_template_suffix}")

    set(JNI_LIBS_DIR "${__android_project_dir}/src/main/jniLibs/${ANDROID_NDK_ABI_NAME}")

    # If new executable set up basic project.
    if(OPTS_DEPENDENCIES)
        foreach(dependency ${OPTS_DEPENDENCIES})
            if("${GRADLE_DEPENDENCIES}" STREQUAL "")
                set(GRADLE_DEPENDENCIES "${dependency}")
            else("${GRADLE_DEPENDENCIES}" STREQUAL "")
                set(GRADLE_DEPENDENCIES "${GRADLE_DEPENDENCIES}\n\t${dependency}")
            endif("${GRADLE_DEPENDENCIES}" STREQUAL "")
        endforeach()
    endif()
    if(OPTS_GRADLE_PLUGINS)
        foreach(plugin ${OPTS_GRADLE_PLUGINS})
            set(GRADLE_PLUGINS "${GRADLE_PLUGINS}apply plugin: '${plugin}'\n")
        endforeach()
    endif()

    # Bootstrap gradle project.
    _prepare_gradle(${__android_project_dir} ${ARGN})
    if(NOT EXISTS "${__android_project_dir}/build.gradle")
        set(BASE_ACTIVITY_NAME AexActivity)
        if(PYTHON_PROJECT)
            set(BASE_ACTIVITY_NAME AexPythonActivity)
        endif()
        string(REPLACE "." "/" PACKAGE_DIR ${OPTS_PACKAGE_NAME})
        file( MAKE_DIRECTORY  ${JNI_LIBS_DIR})
        find_android_target_sdk(${ANDROID_SDK})
        _get_build_gradle_in(_build_gradle_in ${ANDROID_GRADLE_BASE_DIR} ${ARGN})
        _get_manifest_in(manifestFile ${ANDROID_TEMPLATE_DIR} ${ARGN})
        set(_pairs 
            ${_build_gradle_in} "build.gradle"
            ${manifestFile} "src/main/AndroidManifest.xml")
        if(${_app})
            # Genearete 'basic' entry activity if user
            # didn't define any custom java sources.
            if(NOT OPTS_JAVA_SOURCES_ROOT)
                _get_activity_in(activityFile ${ANDROID_TEMPLATE_DIR} ${ARGN})
                set(_pairs ${_pairs}
                ${activityFile} "src/main/java/${PACKAGE_DIR}/${ANAME}.java")
            endif()
        endif()
        _gen_with_template(${ANDROID_TEMPLATE_DIR}
            ${__android_project_dir}
            ${_pairs}
        )
        if(${_app})
            # Add stl libs to jni dir on native app - if any.
            foreach(lib ${ANDROID_CXX_STANDARD_LIBRARIES})
                if(lib MATCHES "^/.*\\.so$")
                    execute_process( COMMAND "${CMAKE_COMMAND}" -E copy_if_different ${lib} ${JNI_LIBS_DIR})
                endif()
            endforeach()
        endif()
    endif(NOT EXISTS "${__android_project_dir}/build.gradle")


    # Create appropriate targets for app/library.
    if(${_app})
        add_library(${ANAME} SHARED ${OPTS_UNPARSED_ARGUMENTS})
        set_target_properties(${ANAME} PROPERTIES 
            LIBRARY_OUTPUT_DIRECTORY ${__android_project_dir}/native_libs/${ANDROID_NDK_ABI_NAME}
            LIBRARY_OUTPUT_NAME "main"
            RUNTIME_OUTPUT_DIRECTORY ${__android_project_dir}/native_libs/${ANDROID_NDK_ABI_NAME}
            RUNTIME_OUTPUT_NAME "main"
            ANDROID_PROJECT_PATH "${__android_project_dir}")
    else()
        if(OPTS_UNPARSED_ARGUMENTS)
            add_library(${ANAME} ${LIBRARY_TYPE} ${OPTS_UNPARSED_ARGUMENTS})
        else()
            add_custom_target(${ANAME} ALL)
        endif()
        set_target_properties(${ANAME} PROPERTIES 
            LIBRARY_OUTPUT_DIRECTORY ${__android_project_dir}/native_libs/${ANDROID_NDK_ABI_NAME}
            RUNTIME_OUTPUT_DIRECTORY ${__android_project_dir}/native_libs/${ANDROID_NDK_ABI_NAME}
            ANDROID_PROJECT_PATH "${__android_project_dir}")
    endif()

    get_target_property(TARGET_TYPE ${ANAME} TYPE)
    if("${TARGET_TYPE}" STREQUAL "EXECUTABLE" OR "${TARGET_TYPE}" STREQUAL "SHARED_LIBRARY")
        add_custom_command(TARGET ${ANAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:${ANAME}>
                ${__android_project_dir}/src/main/jniLibs/${ANDROID_NDK_ABI_NAME}/$<TARGET_FILE_NAME:${ANAME}>
                )
        file(GLOB STRIP_FILE
            ${ANDROID_TOOLCHAIN_ROOT}/bin/*-strip${TOOLS_OS_SUFFIX})
        add_custom_command( TARGET ${ANAME} POST_BUILD 
            COMMAND ${STRIP_FILE} -s ${JNI_LIBS_DIR}/$<TARGET_FILE_NAME:${ANAME}> )
        file(GLOB READELF_FILE
            ${ANDROID_TOOLCHAIN_ROOT}/bin/*-readelf${TOOLS_OS_SUFFIX})
        add_custom_command( TARGET ${ANAME} POST_BUILD 
            COMMAND ${PYTHON_EXECUTABLE}
                ${android_macros_LIST_DIR}/../tools/py_tools/gen_dep_tree.py
                ${READELF_FILE}
                ${JNI_LIBS_DIR}
                $<TARGET_FILE_NAME:${ANAME}>)

    endif()

    get_gradle_exe(GRADLE_EXECUTABLE)
    add_custom_command(TARGET ${ANAME}
        POST_BUILD
        COMMAND ${GRADLE_EXECUTABLE} assemble$<CONFIG>
        WORKING_DIRECTORY ${__android_project_dir})

    _make_cmake_project_properties(${__android_project_dir})
endfunction()

# SYNTAX: ADD_ANDROID_EXECUTABLE(ACTIVITY_NAME <sourceFile1> [<sourceFile2>...]
#       [PACKAGE_NAME <package_name>]
#       [ASSETS_ROOT <assets_root>]
#       [ASSETS <asset1> [asset2...]]
#       [RESOURCES_ROOT <resources_root>]
#       [RESOURCES <resource1> [resource2...]] 
#       [BUILD_TOOLS_VERSION <build_tools_version>]
#       [GRADLE_BUILD_TASK <gradle_build_task>]
#       [ANDROID_MANIFEST_IN <file>]
#       [SKIP_RELEASE_SIGN])
# ASSETS_ROOT - Assets source root, by default ${CMAKE_CURRENT_SOURCE_DIR}, relative to current source dir.
# ASSETS - Files to be copied to src/main/assets. Path must be relative to ASSETS_ROOT.
# RESOURCES_ROOT - Resource source root, by default ${CMAKE_CURRENT_SOURCE_DIR}, relative to current source dir.
# RESOURCES - Files to be copied to src/main/res. Path must be relative to RESOURCES_ROOT.
# BUILD_TOOLS_VERSION - Version of build tools (default: latest in sdk/build_tools/)
# GRADLE_VERSION - Android gradle plugin version (default: 1.2.0)
# GRADLE_BUILD_TASK - Task to be executed to build APK. Will be concated with Release/RelWithDebInfo/MinSizeRel/Debug variant depending on CMAKE_BUILD_CONFIG_TYPE (default assemble${CMAKE_BUILD_CONFIG_TYPE}. 
# ANDROID_MANIFEST_IN - If set with path to file, will be used in place of vanilla android generated by AndroidSDK. File will be configured.
# SKIP_RELEASE_SIGN - Do not sign release version. Useful for quick debug builds.
# 
# Build library and create target to build each configuration.
function(ADD_ANDROID_EXECUTABLE ACTIVITY_NAME)
    _make_android_project(APP ${ACTIVITY_NAME} ${ARGN})
endfunction()

# SYNTAX: ADD_ANDROID_LIBRARY_GRADLE(LIBRARY_NAME <sourceFile1> [<sourceFile2>...]
#       [PACKAGE_NAME <package_name>]
#       [JAVA_SOURCE_ROOT <java_src_root>]
#       [ASSETS_ROOT <assets_root>]
#       [ASSETS <asset1> [asset2...]]
#       [RESOURCES_ROOT <resources_root>]
#       [RESOURCES <resource1> [resource2...]] 
#       [BUILD_TOOLS_VERSION <build_tools_version>]
#       [GRADLE_BUILD_TASK <gradle_build_task>]
#       [BUILD_GRADLE_IN <file>]
#       [ANDROID_MANIFEST_IN <file>]
#       [SETTINGS_GRADLE_IN <file>]
#       [DEPENDENCIES <dependecy1> [dependency2...]]
#       [ANDROID_LIBRARY_DIR_SUFFIX suffix]
#       [SHARED]
#       [STATIC]
#       [EXTRA_PROPERTIES] <PROPERTY_NAME1> <PROPERTY_VALUE2> [<PROPERTY_NAME2> <PROPERTY_VALUE2> ...]
#)
# JAVA_SOURCE_ROOT - Root for android java sources, if not set nothing will be copied otherwise sources will be synced with it on each refresh. Relative to ${CMAKE_CURRENT_SOURCE_DIR}
# ASSETS_ROOT - Assets source root, by default ${CMAKE_CURRENT_SOURCE_DIR}. Relative to ${CMAKE_CURRENT_SOURCE_DIR}
# ASSETS - Files to be copied to src/main/assets. Path must be relative to ASSETS_ROOT.
# RESOURCES_ROOT - Resource source root, by default ${CMAKE_CURRENT_SOURCE_DIR}. Relative to ${CMAKE_CURRENT_SOURCE_DIR}
# RESOURCES - Files to be copied to src/main/res. Path must be relative to RESOURCES_ROOT.
# BUILD_TOOLS_VERSION - Version of build tools (default: latest in sdk/build_tools/)
# GRADLE_VERSION - Android gradle plugin version (default: 1.2.0)
# GRADLE_BUILD_TASK - Task to be executed to build APK. Will be concated with Release/RelWithDebInfo/MinSizeRel/Debug variant depending on CMAKE_BUILD_CONFIG_TYPE (default assemble${CMAKE_BUILD_CONFIG_TYPE}. 
# BUILD_GRADLE_IN - If set with path to file, will be used in place of default one.
# ANDROID_MANIFEST_IN - If set with path to file, will be used in place of vanilla file generated by AndroidSDK. File will be configured.
# SETTINGS_GRADLE_IN - If set with path to file, will be used in place of vanilla file generated by AndroidSDK. File will be configured.
# STATIC/SHARED - Resulting library type.
# DEPENDENCIES - List of project default dependencies. Updated only first time project is created.
# ANDROID_LIBRARY_DIR_SUFFIX - Create android gradle project in ${CMAKE_CURRENT_BINARY_DIR}/suffix dir.
# EXTRA_PROPERTIES - Name-Value pairs of extra properties to be appended in cmake-project.properties
# 
# Create android library gradle project.


function(ADD_ANDROID_LIBRARY LIBRARY_NAME)
    _make_android_project(LIBRARY ${LIBRARY_NAME} ${ARGN})
endfunction()

# Link libraries with android exec.
function( TARGET_LINK_HOST_LIBRARIES_ANDROID TARGET )
    foreach(lib ${ARGN})
        get_filename_component( fName ${lib} NAME )
        get_filename_component( lib_ext_component ${lib} EXT)
        string(FIND "${lib_ext_component}" ".so" EXT_POS REVERSE)

        # Check if cmake library type description.
        if("${lib}" STREQUAL "optimized" OR "${lib}" STREQUAL "debug" OR "${lib}" STREQUAL "general")
            set(OPTIM_PREFIX ${lib})
        else()
            # Is library *.so?
            if(NOT ${EXT_POS} EQUAL -1)
                # Strip all versioning from library name and copy
                # liibrary to project dir.
                get_target_property(__android_project_dir ${TARGET} ANDROID_PROJECT_PATH)
                get_filename_component( RealLib ${lib} REALPATH )
                get_filename_component( libFileName ${RealLib} NAME_WE )
                get_filename_component( so_ext ${RealLib} EXT )
                if(NOT "${so_ext}" MATCHES "^\\.so[.0-9]*$")
                    # Find .so extension in case of library having . in
                    # file name. For example libSDL2-2.0.so
                    if("${so_ext}" MATCHES "^.*\\.so[.0-9]*$")
                        string(REGEX REPLACE "^(\\..*)\\.so[.0-9]*$"
                            "\\1" pre_so "${so_ext}")
                        string(REGEX REPLACE "^\\..*(\\.so[.0-9]*)$"
                            "\\1" so_ext "${so_ext}")
                        set(libFileName "${libFileName}${pre_so}")
                    endif()
                endif()
                if(NOT "${so_ext}" STREQUAL ".so")
                    if("${so_ext}" MATCHES "^\\.so[.0-9]*$")
                        message(WARNING "Stripping version from ${libFileName}${so_ext} file name.\n\tMake sure that SONAME of library matches.")
                    endif()
                endif()
                string(SUBSTRING ${lib_ext_component} 0 ${EXT_POS}-1 NAME_PART)
                set(TargetLib "${__android_project_dir}/native_libs/${ANDROID_NDK_ABI_NAME}/${libFileName}.so")
                set(StrippedLib "${__android_project_dir}/src/main/jniLibs/${ANDROID_NDK_ABI_NAME}/${libFileName}.so")
                execute_process(
                    COMMAND "${CMAKE_COMMAND}" -E compare_files ${RealLib} ${TargetLib}
                    RESULT_VARIABLE compare_libs
                    OUTPUT_QUIET
                    ERROR_QUIET
                    )
                if(NOT "${compare_libs}" STREQUAL 0)
                    execute_process( COMMAND "${CMAKE_COMMAND}" -E copy_if_different ${RealLib} ${TargetLib})
                    execute_process( COMMAND "${CMAKE_COMMAND}" -E copy_if_different ${TargetLib} ${StrippedLib})

                    # Fix permission issues on linux.
                    if(UNIX)
                        find_host_program(CHMOD_EXECUTABLE chmod)
                        execute_process(COMMAND ${CHMOD_EXECUTABLE} +w ${StrippedLib})
                    endif(UNIX)

                    # Strip all symbols from library after copy.
                    file(GLOB STRIP_FILE
                        ${ANDROID_TOOLCHAIN_ROOT}/bin/*-strip${TOOLS_OS_SUFFIX})
                    execute_process(
                        COMMAND ${STRIP_FILE} -s ${StrippedLib}
                        )
                    # Link against copied library.
                    set(RealLib ${TargetLib})
                endif()
            else()
                # If it's an archive just link it.
                set( RealLib ${lib})
            endif()
            get_target_property(TARGET_TYPE ${TARGET} TYPE)
            if("${TARGET_TYPE}" STREQUAL "STATIC_LIBRARY")
                target_link_libraries(${TARGET} INTERFACE ${OPTIM_PREFIX} ${lib})
            elseif("${TARGET_TYPE}" STREQUAL "UTILITY")
            else("${TARGET_TYPE}" STREQUAL "STATIC_LIBRARY")
                target_link_libraries(${TARGET} PRIVATE ${OPTIM_PREFIX} ${lib})
            endif("${TARGET_TYPE}" STREQUAL "STATIC_LIBRARY")
            set(OPTIM_PREFIX "")
        endif()
    endforeach(lib ${ARGN})
endfunction()

function(CLEAN_ANDROID_LIBRARY TARGET)
    get_target_property(${TARGET}_DEPENDECIES ${TARGET} _INTERFACE_LINK_LIBRARIES_DIRTY)
    list(LENGTH ${TARGET}_DEPENDECIES dependencies_count)
    if(NOT dependencies_count EQUAL 0)
        math(EXPR last_index ${dependencies_count}-1)
        foreach(idx RANGE ${last_index})
            list(GET ${TARGET}_DEPENDECIES ${idx} lib)
            if("${lib}" STREQUAL "optimized" OR "${lib}" STREQUAL "debug")
                math(EXPR next_idx ${idx}+1)
                list(GET ${TARGET}_DEPENDECIES ${next_idx} lib_file)
                set(${lib}_libs ${${lib}_libs} ${lib_file})
            endif("${lib}" STREQUAL "optimized" OR "${lib}" STREQUAL "debug")
        endforeach(idx RANGE ${last_index})
    endif(NOT dependencies_count EQUAL 0)
    list(REMOVE_ITEM ${TARGET}_DEPENDECIES "debug" "optimized" "general")
    if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        list(REMOVE_ITEM ${TARGET}_DEPENDECIES ${optimized_libs})
    else("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        list(REMOVE_ITEM ${TARGET}_DEPENDECIES ${debug_libs})
    endif("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set_target_properties(${TARGET} PROPERTIES INTERFACE_LINK_LIBRARIES "${${TARGET}_DEPENDECIES}")
endfunction()

if(ANDROID)
    gradle_init()
endif()
