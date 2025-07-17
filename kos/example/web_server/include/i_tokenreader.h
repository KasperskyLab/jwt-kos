/*
 * © 2024 AO Kaspersky Lab
 * Licensed under the MIT License
 */

#ifndef WEB_SERVER_INCLUDE_I_TOKENREADER_H
#define WEB_SERVER_INCLUDE_I_TOKENREADER_H
#pragma once

#include <memory>

namespace ipc {
struct ITokenReader
{
    virtual ~ITokenReader() = default;

    virtual bool Generate(std::string& token) = 0;
    virtual bool Verify(const std::string& token) = 0;
};

using ITokenReaderPtr = std::shared_ptr<ITokenReader>;

}  // namespace ipc
#endif  // WEB_SERVER_INCLUDE_I_TOKENREADER_H
