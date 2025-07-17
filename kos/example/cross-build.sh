#!/bin/bash

# © 2025 AO Kaspersky Lab
# Licensed under the MIT License

Usage() {
    echo "Usage: $0 [TARGET] [OPTIONS]"
    echo "KasperskyOS SDK location will be obtained from the SDK_PREFIX environment variable."
    echo ""
    echo "Available targets":
    echo "  qemu - build for QEMU."
    echo "  hw   - build for hardware."
    echo ""
    echo "Options:"
    echo "  -h | --help     - print this message."
    echo "  -s | --sdk PATH - directory where KasperskyOS installed,"
    echo "                    this option overrides the SDK_PREFIX environment variable."
    echo ""
    echo "Examples:"
    echo "  ./cross-build.sh qemu "
    echo "  ./cross-build.sh hw -s \"/opt/KasperskyOS-Community-Edition-<version>\""
    echo ""
}

[ $# -eq 0 ] && Usage && exit 0

while [ -n "${1}" ]; do
    case "${1}" in
    -h | --help) Usage
        exit 0;;
    -s | --sdk) SDK_PREFIX="${2}"
        shift;;
    qemu) TARGET=sim;;
    hw) TARGET=sd-image;;
    -*) echo "Unknown option -'${1}'."
        Usage
        exit 1;;
    *) echo "Unknown TARGET -'${1}'."
        Usage
        exit 1;;
    esac
    shift
done

if [ -z "${SDK_PREFIX}" ]; then
    echo "Can't get path to the installed KasperskyOS SDK."
    Usage
    exit 1
fi

SCRIPT_DIR="$(dirname "$(realpath "$0")")"
BUILD="${SCRIPT_DIR}/build"

export LANG=C
export TARGET_PLATFORM="aarch64-kos"
export PKG_CONFIG=""
export INSTALL_PREFIX="$BUILD/../install"
export PATH="$SDK_PREFIX/toolchain/bin:$PATH"

export BUILD_WITH_CLANG=
export BUILD_WITH_GCC=

TOOLCHAIN_SUFFIX=""

if [ "$BUILD_WITH_CLANG" == "y" ];then
    TOOLCHAIN_SUFFIX="-clang"
fi

if [ "$BUILD_WITH_GCC" == "y" ];then
    TOOLCHAIN_SUFFIX="-gcc"
fi

(cd $SCRIPT_DIR/../../ && git submodule update --init --depth=1 third_party/openssl && cd third_party/openssl && ./config && make build_generated)

"$SDK_PREFIX/toolchain/bin/cmake" -G "Unix Makefiles" -B "$BUILD" \
      -D CMAKE_BUILD_TYPE:STRING=Debug \
      -D CMAKE_INSTALL_PREFIX:STRING="$INSTALL_PREFIX" \
      -D CMAKE_FIND_ROOT_PATH="$PREFIX_DIR/sysroot-$TARGET_PLATFORM" \
      -D CMAKE_TOOLCHAIN_FILE="$SDK_PREFIX/toolchain/share/toolchain-$TARGET_PLATFORM$TOOLCHAIN_SUFFIX.cmake" \
      -D OPENSSL_INCLUDE_DIR="$SCRIPT_DIR/../../third_party/openssl/include/" \
      "$SCRIPT_DIR/" && "$SDK_PREFIX/toolchain/bin/cmake" --build "$BUILD" --target $TARGET
