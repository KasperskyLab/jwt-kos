/*
 * © 2024 AO Kaspersky Lab
 * Licensed under the MIT License
 */

#ifndef TOKEN_SERVICE_INCLUDE_TOKEN_HANDLER_H
#define TOKEN_SERVICE_INCLUDE_TOKEN_HANDLER_H
#pragma once

#include "common.h"
#include "TokenService.edl.h"

#include <fstream>
#include <map>
#include <string>
#include <variant>

class TokenHandler
{
public:
    static IToken* CreateImpl(std::string secretWord);

    TokenHandler()  = delete;
    ~TokenHandler() = default;

    std::string Generate();
    bool Verify(const std::string& token);

private:
    TokenHandler(std::string secretWord);

private:
    std::string m_secretWord;
};

#endif  // TOKEN_SERVICE_INCLUDE_TOKEN_HANDLER_H
