# Unit testing using the GoogleTest framework

These tests were copied from https://github.com/arun11299/cpp-jwt/tree/master/tests and converted
to gtests.

## Table of contents
- [Unit testing using the GoogleTest framework](#unit-testing-using-the-googletest-framework)
  - [Table of contents](#table-of-contents)
  - [Solution overview](#solution-overview)
    - [List of programs](#list-of-programs)
    - [Initialization description](#initialization-description)
    - [Security policy description](#security-policy-description)
  - [Getting started](#getting-started)
    - [Structure of unittests directory](#structure-of-unittests-directory)
    - [Building and running the tests](#building-and-running-the-tests)
      - [QEMU](#qemu)
      - [Hardware](#hardware)
      - [CMake input files](#cmake-input-files)

## Solution overview

### List of programs

* `TestEntity`—Tests
* `VfsRamFs`—Program that supports RamFS file system
* `EntropyEntity`—Random number generator
* `RAMDisk`—Block device driver of a virtual drive in RAM

### Initialization description

<details><summary>Statically created IPC channels</summary>

* `unittests.TestEntity` → `kl.VfsRamFs`
* `kl.VfsRamFs` → `kl.drivers.RAMDisk`
* `kl.VfsRamFs` → `kl.EntropyEntity`
</details>

The [`./einit/src/init.yaml.in`](einit/src/init.yaml.in) template is used to automatically generate
part of the solution initialization description file `init.yaml`. For more information about the
`init.yaml.in` template file, see the
[KasperskyOS Community Edition Online Help](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.3&customization=KCE&helpid=cmake_yaml_templates).

[⬆ Back to Top](#Table-of-contents)

### Security policy description

The [`./einit/psl/security.psl.in`](einit/psl/security.psl.in) template is used to automatically
generate part of the `security.psl` file using CMake tools. The `security.psl` file contains part of
a solution security policy description. For more information about the `security.psl` file, see
[Describing a security policy for a KasperskyOS-based solution](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.3&customization=KCE&helpid=ssp_descr).

[⬆ Back to Top](#Table-of-contents)

## Getting started

### Structure of unittests directory

When you develop a KasperskyOS-based solution, use the
[recommended structure of project directories](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.3&customization=KCE&helpid=cmake_using_sdk_cmake)
to simplify usage of CMake scripts.

<pre>
.
├── kos
│   ├── unittests
│   │   ├── einit            —Source files and CMakeList.txt for the Einit program
│   │   ├── tests            —Source files and CMakeList.txt for the TestEntity program
│   │   ├── CMakeLists.txt   —CMake file containing the build instructions
│   │   ├── cross-build.sh   —Script for building a solution with the tests
</pre>

### Building and running the tests

The tests for KasperskyOS are built using the CMake build system, which is provided in the KasperskyOS
Community Edition SDK.

The `SDK_PREFIX` environment variables affects the build of the example. It specifies the path to
the installed version of the KasperskyOS Community Edition SDK.

Run the following command `./cross-build.sh <TARGET> [-s SDK_PATH]`, where:

* `TARGET` can take one of the following values: `qemu` for QEMU or `hw` for Raspberry Pi 4 B or Radxa ROCK 3A.
* `SDK_PATH` specifies the path to the installed version of the KasperskyOS Community Edition SDK.
If not specified, the path defined in the `SDK_PREFIX` environment variable is used. The value
specified in `-s` option takes precedence over the value of the `SDK_PREFIX` environment variable.

#### QEMU

Running `cross-build.sh` creates a KasperskyOS-based solution image that includes the tests.
The `kos-qemu-image` solution image is located in the `./build/einit` directory.

The `cross-build.sh` script both builds the tests on QEMU and runs them.

#### Hardware

Running `cross-build.sh` creates a KasperskyOS-based solution image that includes the tests and
a bootable SD card image for Raspberry Pi 4 B or Radxa ROCK 3A. The `kos-image` solution image is located in the
`./build/einit` directory. The `hdd.img` bootable SD card image is located in the `./build`
directory.

1. To copy the bootable SD card image to the SD card, connect the SD card to the computer and run
the following command:

   `$ sudo dd bs=64k if=build/hdd.img of=/dev/sd[X] conv=fsync`,

   where `[X]` is the final character in the name of the SD card block device.

1. Connect the bootable SD card to the Raspberry Pi 4 B or Radxa ROCK 3A.
1. Supply power to the Raspberry Pi 4 B or Radxa ROCK 3A and wait for the tests to run.

You can also use an alternative option to prepare and run the tests:

1. Prepare the required hardware platform and a bootable SD card to run the tests by following the instructions:
   - [Raspberry Pi 4 B](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.3&customization=KCE&helpid=preparing_sd_card_rpi).
   - [Radxa ROCK 3A](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.3&customization=KCE&helpid=preparing_sd_card_radxa).
1. Run the tests by following the instructions in the
[KasperskyOS Community Edition Online Help](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.3&customization=KCE&helpid=running_sample_programs_rpi).

#### CMake input files

[./einit/CMakeLists.txt](einit/CMakeLists.txt)—CMake commands for building the solution.

[./tests/CMakeLists.txt](tests/CMakeLists.txt)—CMake commands for building the tests.

[./CMakeLists.txt](CMakeLists.txt)—CMake commands for building the solution.

[⬆ Back to Top](#Table-of-contents)

© 2025 AO Kaspersky Lab
