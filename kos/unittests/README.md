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
    - [Building and running the tests](#building-and-running-the-tests)
      - [QEMU](#qemu)
      - [Raspberry Pi 4 B](#raspberry-pi-4-b)
      - [CMake input files](#cmake-input-files)
  - [Usage](#usage)

## Solution overview

### List of programs

* `TestEntity`—Tests
* `VfsNet`—Program that is used for working with the network
* `VfsRamFs`—Program that supports RamFS file system
* `EntropyEntity`—Random number generator
* `DNetSrv`—Driver for working with network cards
* `RAMDisk`—Block device driver of a virtual drive in RAM
* `Bcm2711MboxArmToVc`—Mailbox driver for Raspberry Pi 4 B

### Initialization description

<details><summary>Statically created IPC channels</summary>

* `unittests.TestEntity` → `kl.VfsRamFs`
* `kl.VfsNet` → `kl.EntropyEntity`
* `kl.VfsNet` → `kl.drivers.DNetSrv`
* `kl.VfsRamFs` → `kl.drivers.RAMDisk`
* `kl.VfsRamFs` → `kl.EntropyEntity`
* `kl.drivers.DNetSrv` → `kl.drivers.Bcm2711MboxArmToVc`
</details>

The [`./einit/src/init.yaml.in`](einit/src/init.yaml.in) template is used to automatically generate
part of the solution initialization description file `init.yaml`. For more information about the
`init.yaml.in` template file, see the
[KasperskyOS Community Edition Online Help](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.2&customization=KCE_cmake_yaml_templates).

[⬆ Back to Top](#Table-of-contents)

### Security policy description

The [`./einit/psl/security.psl.in`](einit/psl/security.psl.in) template is used to automatically
generate part of the `security.psl` file using CMake tools. The `security.psl` file contains part of
a solution security policy description. For more information about the `security.psl` file, see
[Describing a security policy for a KasperskyOS-based solution](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.2&customization=KCE_ssp_descr).

[⬆ Back to Top](#Table-of-contents)

## Getting started

### Building and running the tests

The tests for KasperskyOS are built using the CMake build system, which is provided in the KasperskyOS
Community Edition SDK.

The `SDK_PREFIX` environment variables affects the build of the example. It specifies the path to
the installed version of the KasperskyOS Community Edition SDK.

Run the following command `./cross-build.sh <TARGET> [-s SDK_PATH]`, where:

* `TARGET` can take one of the following values: `qemu` for QEMU or `rpi` for Raspberry Pi 4 B.
* `SDK_PATH` specifies the path to the installed version of the KasperskyOS Community Edition SDK.
If not specified, the path defined in the `SDK_PREFIX` environment variable is used. The value
specified in `-s` option takes precedence over the value of the `SDK_PREFIX` environment variable.

#### QEMU

Running `cross-build.sh` creates a KasperskyOS-based solution image that includes the tests.
The `kos-qemu-image` solution image is located in the `./build/einit` directory.

The `cross-build.sh` script both builds the tests on QEMU and runs them.

#### Raspberry Pi 4 B

Running `cross-build.sh` creates a KasperskyOS-based solution image that includes the tests and
a bootable SD card image for Raspberry Pi 4 B. The `kos-image` solution image is located in the
`./build/einit` directory. The `rpi4kos.img` bootable SD card image is located in the `./build`
directory.

1. To copy the bootable SD card image to the SD card, connect the SD card to the computer and run
the following command:

   `$ sudo dd bs=64k if=build/rpi4kos.img of=/dev/sd[X] conv=fsync`,

   where `[X]` is the final character in the name of the SD card block device.

1. Connect the bootable SD card to the Raspberry Pi 4 B.
1. Supply power to the Raspberry Pi 4 B and wait for the tests to run.

You can also use an alternative option to prepare and run the tests:

1. Prepare Raspberry Pi 4 B and a bootable SD card to run the tests by following the instructions in the
[KasperskyOS Community Edition Online Help](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.2&customization=KCE_preparing_sd_card_rpi).
1. Run the tests by following the instructions in the
[KasperskyOS Community Edition Online Help](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.2&customization=KCE_running_sample_programs_rpi)

#### CMake input files

[./einit/CMakeLists.txt](einit/CMakeLists.txt)—CMake commands for building the solution.

[./tests/CMakeLists.txt](tests/CMakeLists.txt)—CMake commands for building the tests.

[./CMakeLists.txt](CMakeLists.txt)—CMake commands for building the solution.

[⬆ Back to Top](#Table-of-contents)

## Usage

Once testing is complete, the results are displayed:

```
[==========] Running 58 tests from 7 test suites.
[----------] Global test environment set-up.
[----------] 4 tests from ESAlgo
[ RUN      ] ESAlgo.ES256EncodingDecodingTest
[       OK ] ESAlgo.ES256EncodingDecodingTest (128 ms)
[ RUN      ] ESAlgo.ES384EncodingDecodingTest
[       OK ] ESAlgo.ES384EncodingDecodingTest (45 ms)
[ RUN      ] ESAlgo.ES512EncodingDecodingTest
[       OK ] ESAlgo.ES512EncodingDecodingTest (44 ms)
[ RUN      ] ESAlgo.ES384EncodingDecodingValidTest
[       OK ] ESAlgo.ES384EncodingDecodingValidTest (107 ms)
[----------] 4 tests from ESAlgo (342 ms total)

[----------] 1 test from ObjectTest
[ RUN      ] ObjectTest.MoveConstructor
[       OK ] ObjectTest.MoveConstructor (2 ms)
[----------] 1 test from ObjectTest (7 ms total)

[----------] 13 tests from DecodeVerifyExp
[ RUN      ] DecodeVerifyExp.BeforeExpiryTest
[       OK ] DecodeVerifyExp.BeforeExpiryTest (6 ms)
[ RUN      ] DecodeVerifyExp.AfterExpiryTest
[       OK ] DecodeVerifyExp.AfterExpiryTest (56 ms)
[ RUN      ] DecodeVerifyExp.AfterExpiryWithLeeway
[       OK ] DecodeVerifyExp.AfterExpiryWithLeeway (3 ms)
[ RUN      ] DecodeVerifyExp.ValidIssuerTest
[       OK ] DecodeVerifyExp.ValidIssuerTest (3 ms)
[ RUN      ] DecodeVerifyExp.InvalidIssuerTest_1
[       OK ] DecodeVerifyExp.InvalidIssuerTest_1 (13 ms)
[ RUN      ] DecodeVerifyExp.InvalidIssuerTest_2
[       OK ] DecodeVerifyExp.InvalidIssuerTest_2 (2 ms)
[ RUN      ] DecodeVerifyExp.NotImmatureSignatureTest
[       OK ] DecodeVerifyExp.NotImmatureSignatureTest (3 ms)
[ RUN      ] DecodeVerifyExp.ImmatureSignatureTest
[       OK ] DecodeVerifyExp.ImmatureSignatureTest (2 ms)
[ RUN      ] DecodeVerifyExp.ImmatureSignatureTestWithLeeway
[       OK ] DecodeVerifyExp.ImmatureSignatureTestWithLeeway (3 ms)
[ RUN      ] DecodeVerifyExp.InvalidAudienceTest
[       OK ] DecodeVerifyExp.InvalidAudienceTest (4 ms)
[ RUN      ] DecodeVerifyExp.InvalidSignatureTest
[       OK ] DecodeVerifyExp.InvalidSignatureTest (2 ms)
[ RUN      ] DecodeVerifyExp.KeyNotPresentTest
[       OK ] DecodeVerifyExp.KeyNotPresentTest (2 ms)
[ RUN      ] DecodeVerifyExp.InvalidSubjectTest
[       OK ] DecodeVerifyExp.InvalidSubjectTest (4 ms)
[----------] 13 tests from DecodeVerifyExp (162 ms total)

[----------] 13 tests from EncodeTest
[ RUN      ] EncodeTest.TestRemoveClaim
[       OK ] EncodeTest.TestRemoveClaim (4 ms)
[ RUN      ] EncodeTest.TestRemoveTypHeader
Header: {"alg":"HS256"}
Signature: eyJhbGciOiJIUzI1NiJ9.eyJleHAiOjE2OTg2NDc2MjAsImlhdCI6MTUxMzg2MjM3MSwiaWQiOiJhLWItYy1kLWUtZi0xLTItMyIsImlzcyI6ImFydW4ubXVyYWxpZGhhcmFuIiwic3ViIjoiYWRtaW4ifQ.s-XB_TB1w5e6As4o4yWuc9V_ZXfcROT3JS2GSSElPf4
[       OK ] EncodeTest.TestRemoveTypHeader (6 ms)
[ RUN      ] EncodeTest.StrEncodeHS256_1
Header: {"alg":"HS256","typ":"JWT"}
Payload: {"iat":1513862371,"id":"a-b-c-d-e-f-1-2-3","iss":"arun.muralidharan","sub":"admin"}
Signature: eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1MTM4NjIzNzEsImlkIjoiYS1iLWMtZC1lLWYtMS0yLTMiLCJpc3MiOiJhcnVuLm11cmFsaWRoYXJhbiIsInN1YiI6ImFkbWluIn0.jk7bRQKTLvs1RcuvMc2B_rt6WBYPoVPirYi_QRBPiuk
[       OK ] EncodeTest.StrEncodeHS256_1 (4 ms)
[ RUN      ] EncodeTest.StrEncodeHS256_2
[       OK ] EncodeTest.StrEncodeHS256_2 (1 ms)
[ RUN      ] EncodeTest.StrEncodeNONE
Header: {"alg":"NONE","typ":"JWT"}
Payload: {"aud":"rift.io","exp":1513863371,"sub":"nothing much"}
[       OK ] EncodeTest.StrEncodeNONE (5 ms)
[ RUN      ] EncodeTest.StrEncodeHS256WithKey
[       OK ] EncodeTest.StrEncodeHS256WithKey (1 ms)
[ RUN      ] EncodeTest.StrEncodeHS384WithKey
[       OK ] EncodeTest.StrEncodeHS384WithKey (1 ms)
[ RUN      ] EncodeTest.StrEncodeHS512WithKey
[       OK ] EncodeTest.StrEncodeHS512WithKey (3 ms)
[ RUN      ] EncodeTest.StrEncodeChangeAlg
[       OK ] EncodeTest.StrEncodeChangeAlg (3 ms)
[ RUN      ] EncodeTest.StrEncodeNoKey
[       OK ] EncodeTest.StrEncodeNoKey (1 ms)
[ RUN      ] EncodeTest.StrEncodeNoneAlgWithKey
[       OK ] EncodeTest.StrEncodeNoneAlgWithKey (2 ms)
[ RUN      ] EncodeTest.OverwriteClaimsTest
[       OK ] EncodeTest.OverwriteClaimsTest (1 ms)
[ RUN      ] EncodeTest.HeaderParamTest
{"alg":"none","crit":["exp"],"kid":1234567,"typ":"jwt"}
{"alg":"none","crit":["exp"],"kid":1234567,"typ":"jwt"}
[       OK ] EncodeTest.HeaderParamTest (11 ms)
[----------] 13 tests from EncodeTest (98 ms total)

[----------] 12 tests from DecodeVerify
[ RUN      ] DecodeVerify.BeforeExpiryTest
[       OK ] DecodeVerify.BeforeExpiryTest (2 ms)
[ RUN      ] DecodeVerify.AfterExpiryTest
[       OK ] DecodeVerify.AfterExpiryTest (1 ms)
[ RUN      ] DecodeVerify.AfterExpiryWithLeeway
[       OK ] DecodeVerify.AfterExpiryWithLeeway (1 ms)
[ RUN      ] DecodeVerify.ValidIssuerTest
[       OK ] DecodeVerify.ValidIssuerTest (4 ms)
[ RUN      ] DecodeVerify.InvalidIssuerTest_1
[       OK ] DecodeVerify.InvalidIssuerTest_1 (1 ms)
[ RUN      ] DecodeVerify.InvalidIssuerTest_2
[       OK ] DecodeVerify.InvalidIssuerTest_2 (6 ms)
[ RUN      ] DecodeVerify.NotImmatureSignatureTest
[       OK ] DecodeVerify.NotImmatureSignatureTest (2 ms)
[ RUN      ] DecodeVerify.ImmatureSignatureTest
[       OK ] DecodeVerify.ImmatureSignatureTest (1 ms)
[ RUN      ] DecodeVerify.ImmatureSignatureTestWithLeeway
[       OK ] DecodeVerify.ImmatureSignatureTestWithLeeway (2 ms)
[ RUN      ] DecodeVerify.InvalidAudienceTest
[       OK ] DecodeVerify.InvalidAudienceTest (1 ms)
[ RUN      ] DecodeVerify.InvalidIATTest
[       OK ] DecodeVerify.InvalidIATTest (4 ms)
[ RUN      ] DecodeVerify.InvalidSignatureTest
[       OK ] DecodeVerify.InvalidSignatureTest (1 ms)
[----------] 12 tests from DecodeVerify (81 ms total)

[----------] 4 tests from RSAAlgo
[ RUN      ] RSAAlgo.RSA256EncodingDecodingTest
[       OK ] RSAAlgo.RSA256EncodingDecodingTest (73 ms)
[ RUN      ] RSAAlgo.RSA384EncodingDecodingTest
[       OK ] RSAAlgo.RSA384EncodingDecodingTest (204 ms)
[ RUN      ] RSAAlgo.RSA512EncodingDecodingTest
[       OK ] RSAAlgo.RSA512EncodingDecodingTest (1077 ms)
[ RUN      ] RSAAlgo.NoSpecificAlgo
[       OK ] RSAAlgo.NoSpecificAlgo (1080 ms)
[----------] 4 tests from RSAAlgo (2451 ms total)

[----------] 11 tests from DecodeTest
[ RUN      ] DecodeTest.InvalidFinalDotForNoneAlg
[       OK ] DecodeTest.InvalidFinalDotForNoneAlg (0 ms)
[ RUN      ] DecodeTest.DecodeNoneAlgSign
{"aud":"rift.io","exp":4513863371,"sub":"nothing much"}
[       OK ] DecodeTest.DecodeNoneAlgSign (3 ms)
[ RUN      ] DecodeTest.DecodeWrongAlgo
[       OK ] DecodeTest.DecodeWrongAlgo (2 ms)
[ RUN      ] DecodeTest.DecodeInvalidHeader
[       OK ] DecodeTest.DecodeInvalidHeader (5 ms)
[ RUN      ] DecodeTest.DecodeEmptyHeader
[       OK ] DecodeTest.DecodeEmptyHeader (1 ms)
[ RUN      ] DecodeTest.DecodeInvalidPayload
[       OK ] DecodeTest.DecodeInvalidPayload (1 ms)
[ RUN      ] DecodeTest.DecodeHS256
[       OK ] DecodeTest.DecodeHS256 (1 ms)
[ RUN      ] DecodeTest.SecretKeyNotPassed
[       OK ] DecodeTest.SecretKeyNotPassed (1 ms)
[ RUN      ] DecodeTest.DecodeHS384
[       OK ] DecodeTest.DecodeHS384 (1 ms)
[ RUN      ] DecodeTest.DecodeHS512
[       OK ] DecodeTest.DecodeHS512 (1 ms)
[ RUN      ] DecodeTest.TypHeaderMiss
Decode header: {"alg":"HS256"}
[       OK ] DecodeTest.TypHeaderMiss (1 ms)
[----------] 11 tests from DecodeTest (66 ms total)

[----------] Global test environment tear-down
[==========] 58 tests from 7 test suites ran. (3251 ms total)
[  PASSED  ] 58 tests.

```

[⬆ Back to Top](#Table-of-contents)

© 2024 AO Kaspersky Lab
