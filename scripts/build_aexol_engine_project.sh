#!/bin/sh
# Simple build script for cmake projects with multiple build types.
# Use config file to specify behavoiur.
# Config file search order:
# - Config file specified on command line
# - Config file in current directory (make_projects.conf).
# - Config file in script directory (make_projects.conf).
# Should be run in project build directory!
# Supported options are:
# source_root - Project source root. Defaults to ${this_script_dir}/..
# operating_systems - Space separated operating systems list to build 
#                          (for ex. Android, iOS, macOS, Linux)
# build_types - Space sperated list of configurations to build.
#               Defaults to "Debug Release".
# cmake_generator - CMake generator for operating systems when not specified.
#                   Defaults to Ninja if available otherwise Unix Makefiles.
# cmake_gen_extra_args - String with extra cmake generation arguments.
#                       (for ex: "-DFOO=foo -DBAR=bar")
# cmake_build_extra_args - String with extra cmake build arguments.
#                       (for ex: "-j8")
# cmake_build_target - Global build target.
#                       (for ex: "install")
# OS specific options:
# {lower_case_os_name}_arch - List of space separated architectures to build for os.
#                              Defaults to host arch.
# {lower_case_os_name}_cmake_generator = OS specific cmake generator.
#                             Defaults to ${cmake_generator}.
# {lower_case_os_name}_cmake_gen_extra_args = OS specific cmake generation args.
#                             Defaults to nothing.
# {lower_case_os_name}_cmake_build_extra_args = OS specific cmake build args.
#                             Defaults to nothing.
# Generator specific options:
# {lower_case_underscore_gen_name}_cmake_gen_extra_args = 
#           Generator specific generation args. If generator name contains space,
#           replace them with underscore. Defaults to nothing.
# {lower_case_underscore_gen_name}_cmake_build_extra_args = 
#           Generator specific build args. If generator name contains space,
#           replace them with underscore. Defaults to nothing.
# Config file var values can contain @var_name@ templates. All strings matching this
# pattern will be replaced with an actual value of var_name.
# Currently available variables:
#   - os
#   - arch
#   - build_type

#
# 
#
# Example config:
# operating_systems="Android iOS"
# build_types="Debug Release"
# ninja_cmake_build_extra_args="-j8"
# android_arch="armeabi-v7a x86"
# android_cmake_gen_extra_args="-DANDROID_ABI=@arch@ "\
# "-DANDROID_TOOLCHAIN=clang "

usage() {
    echo "Usage: \${CMAKE_SOURCE_DIR}/make_projects.sh [OPTIONS] (gen|build|install|uninstall|clean)"
    echo "Options:"
    echo "    -c|--config      Build config path. Defaults to.\n"
    echo "    -f|--filter      Regular expresion matching gen/build path.\n"
    echo "                     For example Android/.*/Debug to generate/build only"
    echo "                     android debug for all architectures.\n"
    echo "    -v|--verbose     More verbose output.\n"
    echo "    -h|--help        This help message\n"
}

report_error() {
    (>&2 echo "$1")
}

report_verbose() {
    if [ "${verbose}" = "YES" ]; then
        report_error "${1}"
    fi
}

while test $# -gt 1; do
    key=$1
    case $key in
        -c|--config)
            config_file="$2"
            shift
            ;;
        -h|--help)
            usage
            exit
            ;;
        -f|--filter)
            filter="$2"
            shift
            ;;
        -v|--verbose)
            verbose="YES"
            ;;
        *)
            ;;
    esac
    shift
done
if [ "${verbose}" = "" ]; then
    verbose="NO"
fi
if [ "${filter}" = "" ]; then
    filter=".*"
fi

filter=$(echo "${filter}" | sed -e 's/[\/$]/\\&/g')

SOURCE="${0}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

# If exists, use realpath command, otherwise define function
# emulating it.
command -v realpath >/dev/null 2>&1 || realpath() {
  OURPWD=$PWD
  is_dir=$(cd ${1}; echo $?) >/dev/null 2>&1
  PP="${1}"
  if [ "${is_dir}" = 0 ]; then
      PP="$(cd ${1}; echo $(pwd))"
  fi
  cd "$(dirname "$PP")"
  LINK=$(readlink "$(basename "$PP")")
  while [ "$LINK" ]; do
    cd "$(dirname "$LINK")"
    LINK=$(readlink "$(basename "$PP")")
  done
  REALPATH="$PWD/$(basename "$PP")"
  cd "$OURPWD"
  echo "$REALPATH"
}
# Source project build configuration
if [ "${config_file}" = "" ] || [ ! -f "${config_file}" ]; then
    if [ -f "${PWD}/make_projects.conf" ]; then
        config_file="${PWD}/make_projects.conf"
    else
        config_file="${DIR}/make_projects.conf"
    fi
fi
. "${config_file}"

get_lower_case() {
    echo $1 | tr '[:upper:]' '[:lower:]'
}

get_lower_case_os() {
    echo $(get_lower_case ${os})
}

get_lower_case_action() {
    echo $(get_lower_case ${action})
}

get_lower_case_built_type() {
    echo $(get_lower_case ${build_type})
}

get_lower_case_arch() {
    echo $(get_lower_case ${arch})
}

get_lower_case_gen() {
    echo $(get_lower_case $(get_cmake_generator))
}

report_os_skip() {
    report_verbose "Skipping ${os}."
}

filter_build_path() {
    path="${os}/${arch}/${build_type}"
    match=$(echo "${path}" | grep -o "${filter}")
    if [ "${match}" = "${path}" ]; then
        exit
    else
        exit 1
    fi
}

# Replace all @var_name@ with in string
# with var value.
parse_opt() {
    s="${1}"
    opt="$(echo "$s" | sed -e 's/^\([^@]*\)\(@\([^@]*\)@\)\{0,1\}\(.*\)$/\3/g')"
    while [ "${opt}" != "" ]; do
        opt_val=$(eval echo "\${${opt}}")
        s=$(echo "${s}" | sed -e "s/@${opt}@/${opt_val}/g")
        opt="$(echo "$s" | sed -e 's/^\([^@]*\)\(@\([^@]*\)@\)\{0,1\}\(.*\)$/\3/g')"
    done
    echo ${s}
}

# If variable operating_systems is not set
# defaults to current host. Option operating_systems
# is delimited by space.
get_build_os() {
    if [ "${operating_systems}" != "" ]; then
        _operating_systems="${operating_systems}"
    else
        _operating_systems="$(uname)"
    fi
    _parsed_operating_systems=""
    for os in $(echo ${_operating_systems}); do
        case "${os}" in
            Android)
                # Detect android ndk/sdk environment variables.
                has_ndk=1
                has_sdk=1
                if [ ! -z ${ANDROID_NDK+x} ] && [ -d ${ANDROID_NDK} ] ; then
                    has_ndk=0
                fi
                if [ ! -z ${ANDROID_SDK+x} ] && [ -d ${ANDROID_SDK} ]; then
                    has_sdk=0
                fi
                if [ ${has_sdk} = "0" ] && [ "${has_ndk}" = "0" ]; then
                    _parsed_operating_systems="${_parsed_operating_systems} ${os}"
                else
                    [ ${has_sdk} != "0" ] && report_verbose "Missing ANDROID_SDK"
                    [ ${has_ndk} != "0" ] && report_verbose "Missing ANDROID_NDK"
                    report_os_skip
                fi
                ;;
            iOS)
                # For now iOS only on macOS
                if [ "$(uname)" = "Darwin" ]; then
                    _parsed_operating_systems="${_parsed_operating_systems} ${os}"
                else
                    report_verbose "iOS build supported only on macOS."
                    report_os_skip
                fi
                ;;
            macOS)
                # For now macOS only on macOS (...).
                if [ "Darwin" = "$(uname)" ]; then
                    _parsed_operating_systems="${_parsed_operating_systems} ${os}"
                else
                    report_verbose "Cross compilation for ${os} not supported."
                    report_os_skip
                fi
                ;;
            *)
                # For now this script will not support cross compilation.
                if [ "${os}" = "$(uname)" ]; then
                    _parsed_operating_systems="${_parsed_operating_systems} ${os}"
                else
                    report_verbose "Cross compilation for ${os} not supported."
                    report_os_skip
                fi
                ;;
        esac
    done
    echo "${_parsed_operating_systems}"
}

# Get build types.
# Defaults to Debug and Release
# Space delimited.
get_build_types() {
    if [ "${build_types}" != "" ]; then
        echo "${build_types}"
    else
        echo "Debug Release"
    fi
}

# Get architecutre for which
# project is being built.
# Set by ${lcase_os}_arch option. Space delimited.
# Defaults to host arch.

get_build_archs() {
    arch=$(eval echo \${$(get_lower_case_os)_arch})
    if [ "${arch}" != "" ]; then
        echo "${arch}"
    else
        echo "$(uname -m)"
    fi
}

# If source_root is set by user in config, use it.
# Attempt to detect project root. For now it's gonna
# be parent dir to script directory.
get_source_root() {
    s="${source_root}"
    if [ "${s}" = "" ]; then
        s="$(realpath ${DIR}/..)"
    fi
    if [ ! -e "${s}/CMakeLists.txt" ]; then
        exit 1
    fi
    echo "${s}"
}

# Get default cmake generator.
# Unless overriden it's gonna be Ninja.
get_default_generator() {
    default_gen="${cmake_generator}"
    if [ "${default_gen}" = "" ]; then
        command -v ninja >/dev/null 2>&1 \
            && default_gen="Ninja" \
            || default_gen="Unix zMakefiles"
    fi
    echo "${default_gen}"
}

# Get OS specific generator.
# If OS does not override generator, use default global one.
get_cmake_generator() {
    cmake_gen=$(eval echo \${$(get_lower_case_os)_cmake_generator})
    if [ "${cmake_gen}" != "" ]; then
        echo "${cmake_gen}"
    else
        # Defaults to ninja.
        echo "$(get_default_generator)"
    fi
}

# CMAKE_BUILD_TYPE option.
get_cmake_gen_build_type_arg() {
    echo "-DCMAKE_BUILD_TYPE=${build_type}"
}

# Concat common cmake args with os and generator specific ones.
get_cmake_gen_extra_args() {
    os_specific="$(eval echo \${$(get_lower_case_os)_cmake_gen_extra_args})"
    gen_specific="$(eval echo \${$(get_lower_case_gen)_cmake_gen_extra_args})"
    echo "${cmake_gen_extra_args} ${gen_specific} ${os_specific}"
}

# Create cmake gen command.
cmake_gen_cmd() {
    source_root="$(get_source_root)"
    if [ "$?" != "0" ]; then
        echo "Invalid source root. Set source_root in config."
        exit 1
    fi
    cmd="\"-G$(get_cmake_generator)\" "\
"$(get_cmake_gen_build_type_arg) "\
"$(get_cmake_gen_extra_args) "\
"$source_root"
    cmd=$(parse_opt "${cmd}")
    echo "${cmd}"
}

# Build specific target only.
# First attempts to see if there's target set specifically for
# os being built. If not check if user set global target.
# If all fails just built cmake default one.
get_cmake_build_target() {
    cmake_target="$(eval echo \${$(get_lower_case_os)_cmake_build_target})"
    if [ "${cmake_target}" != "" ]; then
        echo "--target ${cmake_target}"
        exit
    fi
    cmake_target="${cmake_build_target}"
    if [ "${cmake_target}" != "" ]; then
        echo "--target ${cmake_target}"
        exit
    fi
    echo ""
}

# For multiconfiguration generators, like VS and Xcode.
get_cmake_build_configuration() {
    gen=$(get_cmake_generator)
    if [ "${gen}" = "Xcode" ]; then
        configuration="$(eval echo \${$(get_lower_case_os)_cmake_build_configuration})"
        if [ "${configuration}" != "" ]; then
            echo "--config ${configuration}"
            exit
        fi
        # Fallback to current build type.
        echo "--config ${build_type}"
        exit
    fi
    echo ""
}

# Additonal arguments when building cmake project.
# Passed directly to build system (for ex. -j8 passed to Makefile/Ninja
get_cmake_build_extra_args() {
    gen="$(echo "$(get_lower_case_gen)" | sed -e 's/\s/_/g')"
    os_specific="$(eval echo \${$(get_lower_case_os)_cmake_build_extra_args})"
    gen_specific="$(eval echo \${${gen}_cmake_build_extra_args})"
    echo "${cmake_build_extra_args} ${gen_specific} ${os_specific}"
}

cmake_build_cmd() {
    cmd="--build . "\
"$(get_cmake_build_target) "\
"$(get_cmake_build_configuration) "\
"-- $(get_cmake_build_extra_args)"
    cmd=$(parse_opt "${cmd}")
    echo "${cmd}"
}

cmake_install_cmd() {
    cmd="--build . --target install "\
"$(get_cmake_build_configuration) "\
"-- $(get_cmake_build_extra_args)"
    cmd=$(parse_opt "${cmd}")
    echo "${cmd}"
}

cmake_uninstall_cmd() {
    cmd="--build . --target uninstall "\
"$(get_cmake_build_configuration) "\
"-- $(get_cmake_build_extra_args)"
    cmd=$(parse_opt "${cmd}")
    echo "${cmd}"
}

gen() {
    cmd=$(cmake_gen_cmd)
    ret=$?
    echo "${cmd}"
    if [ "${ret}" != "0" ]; then
        exit 1
    fi
}

build() {
    echo $(cmake_build_cmd)
}

install() {
    echo $(cmake_install_cmd)
}

uninstall() {
    echo $(cmake_uninstall_cmd)
}

do_action() {
    (filter_build_path)
    if [ "$?" = "0" ]; then
        echo "Running ${action} on ${os}/${arch}/${build_type}"
        cmd="$($(get_lower_case_action))"
        if [ "$?" != "0" ]; then
            report_error "${cmd}"
            exit 1
        fi
        if [ "${verbose}" = "YES" ]; then
            report_verbose "CWD: $(pwd)"
            report_verbose "CMAKE command is: cmake ${cmd}"
        fi
        cmake ${cmd}
        if [ "$?" != "0" ]; then
            exit 1
        fi
    else
        report_verbose "Skipping ${action} on ${os}/${arch}/${build_type}"
    fi
}

on_build_types(){
    for build_type in $(echo $(get_build_types)); do
        mkdir -p ${build_types}
        cd ${build_type}
            do_action
        cd ..
    done
}

on_arch() {
    for arch in $(echo $(get_build_archs)); do
        mkdir -p ${arch}
        cd ${arch}
            on_build_types
        cd ..
    done
}

run_action() {
    for os in $(echo $(get_build_os)); do
        if [ "${action}" != "gen" ]; then
            if [ ! -d "${os}" ]; then
                real_action="${action}"
                action=gen
                run_action
                action="${real_action}"
            fi
        fi
        mkdir -p ${os}
        cd ${os}
            on_arch
        cd .. 
    done
}

if [ "$1" = "gen" ]; then
    action=gen
    run_action
elif [ "$1" = "build" ]; then
    action=build
    run_action
elif [ "$1" = "install" ]; then
    action=build
    run_action
    action=install
    run_action
elif [ "$1" = "uninstall" ]; then
    action=uninstall
    run_action
elif [ "$1" = "clean" ]; then
    for os in $(echo ${operating_systems}); do
        rm -rf ${os}
    done
else
    usage
fi
