# JWT library adaptation for KasperskyOS

This version of the [CPP-JWT library](https://github.com/arun11299/cpp-jwt) is adapted for KasperskyOS.

JSON Web Token (JWT) library creates access tokens that allow users to access resources. A server
provides these tokens to clients for authentication. Typically, a user enters a user name and a password
to receive the token.

For more information about the original CPP-JWT library, please refer to [README.md](https://github.com/arun11299/cpp-jwt/blob/master/README.md).

Please refer to the following helpful resources for more information about:

* the original CPP-JWT library—[README.md](https://github.com/arun11299/cpp-jwt/blob/master/README.md)
* using JWT—[Anatomy of JWT](https://scotch.io/tutorials/the-anatomy-of-a-json-web-token),
[Learn JWT](https://auth0.com/learn/json-web-tokens/) and [RFC 7519](https://tools.ietf.org/html/rfc7519)
* KasperskyOS—[KasperskyOS Community Edition Online Help](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.2&customization=KCE_community_edition).

Limitations and known issues are described in the
[KasperskyOS Community Edition Online Help](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.2&customization=KCE_limitations_and_known_problems).

## Table of contents
- [JWT library adaptation for KasperskyOS](#jwt-library-adaptation-for-kasperskyos)
  - [Table of contents](#table-of-contents)
  - [Getting started](#getting-started)
    - [Prerequisites](#prerequisites)
  - [Usage](#usage)
    - [Example](#example)
    - [Tests](#tests)
  - [Trademark notices](#trademark-notices)
  - [Contributing](#contributing)
  - [Licensing](#licensing)

## Getting started

### Prerequisites

1. [Install](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.2&customization=KCE_sdk_install_and_remove)
KasperskyOS Community Edition SDK. You can download the latest version of the KasperskyOS Community
Edition for free from [os.kaspersky.com](https://os.kaspersky.com/development/). The minimum required
version of the KasperskyOS Community Edition SDK is 1.2. For more information, see
[System requirements](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.2&customization=KCE_system_requirements).
1. Copy source files to your project directory. The example of KasperskyOS-based solution and tests
are located in the [`./kos`](kos) directory.

## Usage

When you develop a KasperskyOS-based solution, use the
[recommended structure of project directories](https://click.kaspersky.com/?hl=en-us&link=online_help&pid=kos&version=1.2&customization=KCE_cmake_using_sdk_cmake)
to simplify usage of CMake scripts.

To include the JWT library in your KasperskyOS-based solution, add a directory with header files using
the CMake command `include_directories`. For example:

```
set (JWT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
include_directories (${JWT_ROOT_DIR}/include)
```

### Example

The project example demonstrates how to use of the adapted JWT library in a KasperskyOS-based
solution. You can review the example in the [`./kos/example`](kos/example) directory.

### Tests

[`./kos/unittests`](kos/unittests)—Unit testing using the GoogleTest framework.

## Trademark notices

Registered trademarks and endpoint marks are the property of their respective owners.

GoogleTest is a trademark of Google LLC.

Raspberry Pi is a trademark of the Raspberry Pi Foundation.

## Contributing

Only KasperskyOS-specific changes can be approved. See [CONTRIBUTING.md](CONTRIBUTING.md) for detailed
instructions on code contribution.

## Licensing

This project is licensed under the terms of the MIT license. See [LICENSE](LICENSE) for more information.

[⬆ Back to Top](#Table-of-contents)

© 2024 AO Kaspersky Lab
