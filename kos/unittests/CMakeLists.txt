# © 2024 AO Kaspersky Lab
# Licensed under the MIT License

cmake_minimum_required(VERSION 3.25)
project(unittests)

# Initialize CMake library for the KasperskyOS SDK.
include(platform)
# Statically link executable files.
initialize_platform(FORCE_STATIC)

# Include the CMake library named nk
# for working with the NK compiler (nk-gen-c).
include(platform/nk)

set(JWT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

include_directories(${JWT_ROOT_DIR}/include)

# Attach the vfs package with the VFS component libraries.
find_package(vfs REQUIRED)
include_directories(${vfs_INCLUDE})

# Attach the vfs package with the VFS component executable files.
find_package(precompiled_vfs REQUIRED)
include_directories(${precompiled_vfs_INCLUDE})

# Attach the directory with the generated config header files.
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Attach the directory with header files.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/resources/include)

# Recommended flags against vulnerabilities.
add_compile_options(-Wall -Wextra -Wconversion
                    -fPIE -pie -D_FORTIFY_SOURCE=2 -O2
                    -fstack-protector-strong -fstack-clash-protection
                    -mbranch-protection=standard
                    -Wsign-conversion -Wformat=2 -Wformat-security -Werror=format-security
                    -fsanitize=undefined -fsanitize-undefined-trap-on-error)

add_link_options(LINKER:-z,noexecstack
                 LINKER:-z,separate-code
                 LINKER:-z,now)

# Attach the directory with tests.
add_subdirectory(tests)

# Attach the einit directory containing the KasperskyOS image
# to the project build.
add_subdirectory(einit)
