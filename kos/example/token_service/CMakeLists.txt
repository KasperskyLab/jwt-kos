# © 2024 AO Kaspersky Lab
# Licensed under the MIT License

project(token_service)

# Include the CMake library named nk
# for working with the NK compiler (nk-gen-c).
include(platform/nk)

# Set the linker and compiler flags.
project_header_default("STANDARD_GNU_11:YES" "STRICT_WARNINGS:NO")

find_package(OpenSSL REQUIRED)

add_executable(TokenService
               src/main.cpp
               src/server.cpp
               src/token_handler.cpp
               ${CMAKE_SOURCE_DIR}/utils/utils.cpp)

target_compile_features(TokenService PRIVATE cxx_std_17)
target_compile_options(TokenService PRIVATE -fexceptions)

target_include_directories(TokenService PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
                                        PUBLIC ${PROJECT_INC}
                                               ${JWT_INC_DIR})

target_link_libraries(TokenService ${vfs_CLIENT_LIB}
                                   ${OPENSSL_LIBRARIES}
                                   ${TLS_CLIENT_LIB})

add_dependencies(TokenService jwt_example_token_service)

# We do not need default VFS entity here, which comes from ${vfs_CLIENT_LIB}.
set_target_properties(TokenService PROPERTIES ${vfs_ENTITY}_REPLACEMENT "")
