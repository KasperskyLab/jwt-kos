# © 2024 AO Kaspersky Lab
# Licensed under the MIT License

project(einit)

# Include the CMake library named image
# containing the solution image build scripts.
include(platform/image)
include(${KL_SDK_ROOT_PATH}/common/build-sd-image.cmake)

# Set the linker and compiler flags.
project_header_default("STANDARD_GNU_11:YES" "STRICT_WARNINGS:NO")

# Define an ENTITIES variable with a list of programs
set(ENTITIES
    ${precompiled_vfsVfsNet}
    ${precompiled_vfsVfsRamFs}
    TestEntity)

set(YAML_FILE "src/init.yaml.in")
set(SECURITY_PSL_FILE "psl/security.psl.in")
set(IMAGE_FS "${CMAKE_BINARY_DIR}/hdd")
set(FILES ${CMAKE_BINARY_DIR}/einit/ramdisk0.img)

# Add a target with making and copying the disk image to the example build
# directory.
add_custom_target(ramdisk0.img
                  COMMAND ${CMAKE_COMMAND} -E make_directory ${IMAGE_FS}
                  COMMAND ${CMAKE_COMMAND} -E copy_directory ${JWT_ROOT_DIR}/tests/certs ${IMAGE_FS}
                  COMMAND ${KL_SDK_ROOT_PATH}/common/prepare_hdd_img.sh
                          -d ${IMAGE_FS} -img ramdisk0.img -f ext4 -s 32)

# Create a top-level CMake target named kos-image which can be used
# when building a solution image to be started on the hardware platform.
build_kos_hw_image(kos-image
                   EINIT_ENTITY EinitHw
                   CONNECTIONS_CFG ${YAML_FILE}
                   SECURITY_PSL ${SECURITY_PSL_FILE}
                   IMAGE_FILES ${ENTITIES} ${FILES})

add_dependencies(kos-image ramdisk0.img)

# Create a top-level CMake target named sd-image which can be used
# when building an SD card image to be started on the hardware platform.
build_sd_image(sd-image
               KOS_IMAGE_TARGET kos-image
               IMAGE_FS ${IMAGE_FS})

# Create a top-level CMake target named kos-qemu-image which
# can be used when building a solution image to be started in QEMU.
build_kos_qemu_image(kos-qemu-image
                     EINIT_ENTITY EinitQemu
                     CONNECTIONS_CFG ${YAML_FILE}
                     SECURITY_PSL ${SECURITY_PSL_FILE}
                     QEMU_FLAGS "${QEMU_FLAGS}"
                     QEMU_USER_NETWORK_FLAGS "hostfwd=tcp::1106-:1106"
                     IMAGE_FILES ${ENTITIES} ${FILES})

add_dependencies(kos-qemu-image ramdisk0.img)
