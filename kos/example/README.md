# Example of using the JWT library

This example demonstrates how to use of the adapted JWT library in a KasperskyOS-based solution.

## Table of contents
- [Example of using the JWT library](#example-of-using-the-jwt-library)
  - [Table of contents](#table-of-contents)
  - [Solution overview](#solution-overview)
    - [List of programs](#list-of-programs)
    - [Initialization description](#initialization-description)
    - [Security policy description](#security-policy-description)
  - [Getting started](#getting-started)
    - [Prerequisites](#prerequisites)
    - [Structure of example directories](#structure-of-example-directories)
    - [Building and running the example](#building-and-running-the-example)
      - [QEMU](#qemu)
      - [Raspberry Pi 4 B](#raspberry-pi-4-b)
      - [CMake input files](#cmake-input-files)
  - [Usage](#usage)

## Solution overview

### List of programs

* `WebServer`—`Civetweb` web server
* `TokenService`—User authentication subsystem
* `VfsRamFs`—Program that supports RamFS file system
* `VfsNet`—Networking program
* `Dhcpcd`—DHCP client implementation program that gets network interface parameters from an external
DHCP server in the background and passes them to the virtual file system
* `TlsEntity`—TLS terminator
* `BlobContainer`—Program that loads dynamic libraries used by other programs into shared memory
* `RAMDisk`—Block device driver of a virtual drive in RAM
* `EntropyEntity`—Random number generator
* `DNetSrv`—Driver for working with network cards
* `Bcm2711MboxArmToVc`—Mailbox driver for Raspberry Pi 4 B

[⬆ Back to Top](#Table-of-contents)

### Initialization description

<details><summary>Statically created IPC channels for a basic solution</summary>

* `jwt_example.WebServer` → `kl.VfsNet`
* `jwt_example.WebServer` → `kl.VfsRamFs`
* `jwt_example.WebServer` → `jwt_example.TokenService`
* `jwt_example.WebServer` → `kl.bc.BlobContainer`
* `jwt_example.WebServer` → `kl.TlsEntity`
* `wt_example.TokenService` → `kl.VfsRamFs`
* `wt_example.TokenService` → `kl.bc.BlobContainer`
* `kl.VfsRamFs` → `kl.drivers.RAMDisk`
* `kl.VfsRamFs` → `kl.EntropyEntity`
* `kl.VfsRamFs` → `kl.bc.BlobContainer`
* `kl.VfsNet` → `kl.EntropyEntity`
* `kl.VfsNet` → `kl.drivers.DNetSrv`
* `kl.VfsNet` → `kl.bc.BlobContainer`
* `kl.rump.Dhcpcd` → `kl.VfsRamFs`
* `kl.rump.Dhcpcd` → `kl.VfsNet`
* `kl.rump.Dhcpcd` → `kl.bc.BlobContainer`
* `kl.TlsEntity` → `kl.EntropyEntity`
* `kl.TlsEntity` → `kl.bc.BlobContainer`
* `kl.TlsEntity` → `kl.VfsNet`
* `kl.TlsEntity` → `kl.VfsRamFs`
* `kl.drivers.RAMDisk` → `kl.bc.BlobContainer`
* `kl.EntropyEntity` → `kl.bc.BlobContainer`
* `kl.drivers.DNetSrv` → `kl.drivers.Bcm2711MboxArmToVc`
* `kl.drivers.DNetSrv` → `kl.bc.BlobContainer`
* `kl.drivers.Bcm2711MboxArmToVc` → `kl.bc.BlobContainer`
</details>

The [`./einit/src/init.yaml.in`](einit/src/init.yaml.in) template is used to automatically generate
part of the solution initialization description file `init.yaml`. For more information about the
`init.yaml.in` template file, see the
[KasperskyOS Community Edition Online Help](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.2&customization=KCE_cmake_yaml_templates).

### Security policy description

The [`./einit/psl/security.psl.in`](einit/psl/security.psl.in) template is used to automatically
generate part of the `security.psl` file using CMake tools. The `security.psl` file contains part
of a solution security policy description. For more information about the `security.psl` file, see
[Describing a security policy for a KasperskyOS-based solution](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.2&customization=KCE_ssp_descr).

[⬆ Back to Top](#Table-of-contents)

## Getting started

### Prerequisites

Make sure that you have installed the latest version of the [KasperskyOS Community Edition SDK](https://os.kaspersky.com/development/).

### Structure of example directories

When you develop a KasperskyOS-based solution, use the
[recommended structure of project directories](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.2&customization=KCE_cmake_using_sdk_cmake)
to simplify usage of CMake scripts.

<pre>
.
├── kos
│   ├── example
│   │   ├── build            —Generated directory with KasperskyOS build artifacts
│   │   ├── einit            —Source files and CMakeList.txt for the Einit program
│   │   ├── resources        —Files of EDL and IDL descriptions and other configuration files of the solution
│   │   ├── token_service    —Sources files and CMakeList.txt for the TokenService program
│   │   ├── utils            —Source files for work with the IPC message arena
│   │   ├── web_server       —Sources files and CMakeList.txt for the WebServer program
│   │   ├── CMakeLists.txt   —CMake file containing the build instructions
│   │   ├── cross-build.sh   —Script for building a solution with the example
│   ├── unittests
│   │   ├── einit            —Source files and CMakeList.txt for the Einit program
│   │   ├── tests            —Source files and CMakeList.txt for the TestEntity program
│   │   ├── CMakeLists.txt   —CMake file containing the build instructions
│   │   ├── cross-build.sh   —Script for building a solution with the tests
</pre>

### Building and running the example

The example is built using the CMake build system, which is provided in the KasperskyOS Community
Edition SDK.

The `SDK_PREFIX` environment variables affects the build of the example. It specifies the path to
the installed version of the KasperskyOS Community Edition SDK.

Run the following command `./cross-build.sh <platform> [-s SDK_PATH]`, where:

* `platform` can take one of the following values: `qemu` for QEMU or `rpi` for Raspberry Pi 4 B.
* `SDK_PATH` specifies the path to the installed version of the KasperskyOS Community Edition SDK.
If not specified, the path defined in the `SDK_PREFIX` environment variable is used. The value
specified in `-s` option takes precedence over the value of the `SDK_PREFIX` environment variable.

For example, review the following command:
```
$ ./cross-build.sh qemu -s /opt/KasperskyOS-Community-Edition-<version>
```
The command builds the example with the built-in JWT library and runs the
KasperskyOS-based solution image on QEMU. The solution image is based on the SDK found in
the `/opt/KasperskyOS-Community-Edition-<version>` path, where `version` is the latest version
number of the KasperskyOS Community Edition SDK.

#### QEMU

Running `cross-build.sh` creates a KasperskyOS-based solution image that includes the example.
The `kos-qemu-image` solution image is located in the `./build/einit` directory.

The `cross-build.sh` script both builds the example on QEMU and runs it.

#### Raspberry Pi 4 B

Running `cross-build.sh` creates a KasperskyOS-based solution image that includes the example
and a bootable SD card image for Raspberry Pi 4 B. The `kos-image` solution image is located in
the `./build/einit` directory. The `rpi4kos.img` bootable SD card image is located in the `./build`
directory.

1. To copy the bootable SD card image to the SD card, connect the SD card to the computer and run
the following command:

   `$ sudo dd bs=64k if=build/rpi4kos.img of=/dev/sd[X] conv=fsync`,

   where `[X]` is the final character in the name of the SD card block device.

1. Connect the bootable SD card to the Raspberry Pi 4 B.
1. Supply power to the Raspberry Pi 4 B and wait for the example to run.

You can also use an alternative option to prepare and run the example:

1. Prepare Raspberry Pi 4 B and a bootable SD card to run the example by following the instructions
in the [KasperskyOS Community Edition Online Help](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.2&customization=KCE_preparing_sd_card_rpi).
1. Run the example by following the instructions in the
[KasperskyOS Community Edition Online Help](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.2&customization=KCE_running_sample_programs_rpi)

[⬆ Back to Top](#Table-of-contents)

#### CMake input files

[./einit/CMakeLists.txt](einit/CMakeLists.txt)—CMake commands for building the `Einit` program
and the solution image.

[./token_service/CMakeLists.txt](token_service/CMakeLists.txt)—CMake commands for building the
`TokenService` program.

[./web_server/CMakeLists.txt](web_server/CMakeLists.txt)—CMake commands for building the
`WebServer` program.

[./CMakeLists.txt](CMakeLists.txt)—CMake commands for building the solution.

## Usage

After building and running the example, follow these steps:

1. Wait until the following message appears in the standard output:
   ```
   [WebServer] WebServer started (port: 1106)
   ```
1. Open the page <https://localhost:1106> (when running the example on QEMU) or
[https://\<Raspberry Pi IP address\>:1106]() (when running the example on Raspberry Pi 4 B) in your
browser. The browser will display the `index` page containing an authentication prompt.
1. Click the `Get JWT` button. The `TokenService` program uses the values in the variables `payloadKey`
and `payloadValue` to create the JWT token. The browser will display the generated token in the `JWT`
field. The standard output will show information about the generated JWT token:

   ```
   [WebServer]  Local URL: /gettoken
   [WebServer]  Request: GET
   [WebServer]  RequestURI: /gettoken
   [TokenService] *** [GetJWTToken] ***
   ```
1. Click the `Get data` button. The browser will display the `Data access successed` message or the
`Wrong token or token service unavailable` message depending on the verification result. If the
verification is successful, the following information will be displayed in the standard output:

   ```
   [WebServer]  Local URL: /getdata
   [WebServer]  Request: GET
   [WebServer]  RequestURI: /getdata
   [TokenService][HEADER  ]{"alg":"HS256","typ":"JWT"}
   [TokenService][PAYLOAD ]{"secret":"TOKEN"}
   ```

   If verification fails, the following information will be displayed in the standard output:

   ```
   [WebServer]  Local URL: /getdata
   [WebServer]  Request: GET
   [WebServer]  RequestURI: /getdata
   [TokenService] Token parse error: signature format is incorrect
   ```

[⬆ Back to Top](#Table-of-contents)

© 2024 AO Kaspersky Lab
