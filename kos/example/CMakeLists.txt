# © 2024 AO Kaspersky Lab
# Licensed under the MIT License

cmake_minimum_required(VERSION 3.25)
project(jwt_example)

# Initialize CMake library for the KasperskyOS SDK.
include(platform)
initialize_platform()

# Include the CMake library named nk
# for working with the NK compiler (nk-gen-c).
include(platform/nk)

set(JWT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
include_directories(${JWT_ROOT_DIR}/include)

# Build idl/cdl/edl files.
nk_build_idl_files(jwt_example_token_service_idl
                    NK_MODULE "jwt_example"
                    IDL "resources/idl/IToken.idl")

nk_build_edl_files(jwt_example_token_service
                    IDL_TARGET jwt_example_token_service_idl
                    NK_MODULE "jwt_example"
                    EDL "resources/edl/TokenService.edl")

nk_build_edl_files(jwt_example_web_server
                    DEPENDS jwt_example_token_service_idl
                    NK_MODULE "jwt_example"
                    EDL "resources/edl/WebServer.edl")

# Attach the vfs package with the VFS component libraries.
find_package(vfs REQUIRED)
include_directories(${vfs_INCLUDE})

# Attach a package with the Dhcpcd program implementation.
find_package(rump REQUIRED COMPONENTS DHCPCD_ENTITY)
include_directories(${rump_INCLUDE})

# Attach a package with TLS program and client lib implementations.
find_package(tls REQUIRED)
include_directories(${tls_INCLUDE})

# Attach the vfs package with the VFS component executable files.
find_package(precompiled_vfs REQUIRED)
include_directories(${precompiled_vfs_INCLUDE})

# Attach the directory with the generated config header files.
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Attach the directory with header files.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/resources/include)

# Include a directory with a header containing file name definitions.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/utils)

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

# Attach the directories with programs.
add_subdirectory(token_service)
add_subdirectory(web_server)

# Attach the einit directory containing the KasperskyOS image
# to the project build.
add_subdirectory(einit)
