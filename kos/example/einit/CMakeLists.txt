# © 2025 AO Kaspersky Lab
# Licensed under the MIT License

project(einit)

# Include the CMake library named image
# containing the solution image build scripts.
include(platform/image)
include(${KL_SDK_ROOT_PATH}/common/build-sd-image.cmake)

# Set the linker and compiler flags.
project_header_default("STANDARD_GNU_17:YES" "STRICT_WARNINGS:NO")

# Define an ENTITIES variable with a list of programs.
set(ENTITIES
    ${precompiled_vfsVfsRamFs}
    ${precompiled_vfsVfsNet}
    ${rump_DHCPCD_ENTITY}
    ${tls_ENTITY}
    WebServer
    TokenService)

set(INIT_YAML_FILE "src/init.yaml.in")
set(SECURITY_PSL_FILE "psl/security.psl.in")
set(RAMDISK_FS "${CMAKE_BINARY_DIR}/ramdisk")
set(LIBS_PATH "${RAMDISK_FS}/lib")
set(IMAGE_FS "${CMAKE_BINARY_DIR}/hdd")

if(PLATFORM_SUPPORTS_DYNAMIC_LINKING)
  set_target_properties(${blob_container_ENTITY} PROPERTIES
    ${vfs_ENTITY}_REPLACEMENT ""
    DEPENDS_ON_ENTITY ${precompiled_vfsVfsRamFs}
    EXTRA_ENV "    VFS_FILESYSTEM_BACKEND: client:kl.VfsRamFs")
endif()

set_target_properties(${tls_ENTITY} PROPERTIES
  ${vfs_ENTITY}_REPLACEMENT ""
  EXTRA_CONNECTIONS "
  - target: kl.VfsNet
    id: kl.VfsNet
  - target: kl.VfsRamFs
    id: kl.VfsRamFs"
  EXTRA_ENV "
    VFS_FILESYSTEM_BACKEND: client:kl.VfsRamFs
    VFS_NETWORK_BACKEND: client:kl.VfsNet"
  EXTRA_ARGS "
    - --cacert
    - /certs/rootCA.crt
    - --cert
    - /certs/server.crt
    - --key
    - /certs/server.key")

set_target_properties(${rump_DHCPCD_ENTITY} PROPERTIES
  ${vfs_ENTITY}_REPLACEMENT ""
  DEPENDS_ON_ENTITY "${precompiled_vfsVfsRamFs};${precompiled_vfsVfsNet}"
  EXTRA_ENV "\
    VFS_FILESYSTEM_BACKEND: client{fs->net}:kl.VfsRamFs
    VFS_NETWORK_BACKEND: client:kl.VfsNet"
  EXTRA_ARGS "\
    - '-4'
    - -f
    - /etc/dhcpcd.conf")

set_target_properties(${precompiled_vfsVfsNet} PROPERTIES
  EXTRA_ARGS "\
    - -l
    - devfs /dev devfs 0
    - -l
    - romfs /etc romfs ro")

# Add hosts file to romfs image, to be used by application.
set(FILES ${CMAKE_SOURCE_DIR}/resources/hdd/etc/hosts
          ${CMAKE_BINARY_DIR}/einit/ramdisk0.img)

# Add a target with making and copying the disk image to the example build
# directory.
add_custom_target(ramdisk0.img
                  DEPENDS copylibs_qemu
                  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources/hdd ${RAMDISK_FS}
                  COMMAND ${KL_SDK_ROOT_PATH}/common/prepare_hdd_img.sh
                          -d ${RAMDISK_FS} -s 64 -f ext4 -img ramdisk0.img)

# Create a top-level CMake target named kos-image which can be used
# when building a solution image to be started on the hardware platform.
build_kos_hw_image(kos-image
                   EINIT_ENTITY EinitHw
                   CONNECTIONS_CFG ${INIT_YAML_FILE}
                   SECURITY_PSL ${SECURITY_PSL_FILE}
                   PACK_DEPS_COPY_ONLY ON
                   PACK_DEPS_LIBS_PATH ${LIBS_PATH}
                   PACK_DEPS_COPY_TARGET copylibs_hw
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
                     CONNECTIONS_CFG ${INIT_YAML_FILE}
                     SECURITY_PSL ${SECURITY_PSL_FILE}
                     QEMU_FLAGS "${QEMU_FLAGS}"
                     QEMU_USER_NETWORK_FLAGS "hostfwd=tcp::1106-:1106"
                     PACK_DEPS_COPY_ONLY ON
                     PACK_DEPS_LIBS_PATH ${LIBS_PATH}
                     PACK_DEPS_COPY_TARGET copylibs_qemu
                     IMAGE_FILES ${ENTITIES} ${FILES})

add_dependencies(kos-qemu-image ramdisk0.img)
