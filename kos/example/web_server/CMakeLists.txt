# © 2025 AO Kaspersky Lab
# Licensed under the MIT License

include(platform/nk)

project_header_default("STANDARD_GNU_17:YES" "STRICT_WARNINGS:NO")

set(INCLUDE_DIR include)
set(SRC_DIR src)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
set(WWW_DIR www)

if(OLD_VERSION_CIVETWEB)
  find_library(CIVETWEB civetweb)
  if(CIVETWEB STREQUAL "CIVETWEB-NOTFOUND")
    message(FATAL_ERROR "civetweb library not found")
  endif()

  find_library(CIVETWEB_CXX civetweb-cpp)
  if(CIVETWEB_CXX STREQUAL "CIVETWEB_CXX-NOTFOUND")
    message(FATAL_ERROR "civetweb-cpp not found")
  endif()
endif()

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(civetweb REQUIRED)

add_executable(WebServer
               ${SRC_DIR}/get_data_handler.cpp
               ${SRC_DIR}/get_token_handler.cpp
               ${SRC_DIR}/kos_connector.cpp
               ${SRC_DIR}/logger.cpp
               ${SRC_DIR}/main.cpp
               ${CMAKE_SOURCE_DIR}/utils/utils.cpp)

target_compile_features(TokenService PRIVATE cxx_std_17)
target_compile_options(TokenService PRIVATE -fexceptions -Wunused-parameter)

target_include_directories(WebServer PRIVATE ${INCLUDE_DIR})

target_link_libraries(WebServer
                      civetweb::civetweb-cpp
                      ${ConnectionManager_CLIENT_LIB}
                      ${tls_CLIENT_LIB}
                      ${vfs_CLIENT_LIB}
                      ${CMAKE_THREAD_LIBS_INIT}
                      ${OPENSSL_LIBRARIES})

add_dependencies(WebServer jwt_example_web_server)

# We do not need default VFS entity here, which comes from ${vfs_CLIENT_LIB}.
set_target_properties(WebServer PROPERTIES ${vfs_ENTITY}_REPLACEMENT "")
