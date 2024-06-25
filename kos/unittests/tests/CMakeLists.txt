# © 2024 AO Kaspersky Lab
# Licensed under the MIT License

include(platform/nk)

find_package(OpenSSL REQUIRED)

project_header_default("STANDARD_GNU_11:YES" "STRICT_WARNINGS:NO")

generate_edl_file(TestEntity PREFIX unittests)
nk_build_edl_files(unit_tests NK_MODULE unittests EDL ${EDL_FILE})

set(TEST_DIR ${JWT_ROOT_DIR}/tests)
set(JWT_TEST_CC_SRC
    src/main.cpp
    ${TEST_DIR}/test_jwt_es.cc
    ${TEST_DIR}/test_jwt_object.cc
    ${TEST_DIR}/test_jwt_decode_verifiy_with_exception.cc
    ${TEST_DIR}/test_jwt_encode.cc
    ${TEST_DIR}/test_jwt_decode_verifiy.cc
    ${TEST_DIR}/test_jwt_rsa.cc
    ${TEST_DIR}/test_jwt_decode.cc)

add_executable(TestEntity ${JWT_TEST_CC_SRC})
target_compile_definitions(TestEntity PUBLIC CERT_ROOT_DIR="")
target_compile_features(TestEntity PRIVATE cxx_std_17)
target_compile_options(TestEntity PRIVATE -fexceptions)
target_link_libraries(TestEntity PRIVATE gtest
                                         gmock
                                         ${vfs_CLIENT_LIB}
                                         ${OPENSSL_LIBRARIES})

add_dependencies(TestEntity unit_tests)
set_target_properties(TestEntity PROPERTIES ${vfs_ENTITY}_REPLACEMENT "")
