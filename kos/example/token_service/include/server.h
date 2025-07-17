/*
 * © 2024 AO Kaspersky Lab
 * Licensed under the MIT License
 */

#ifndef TOKEN_SERVICE_INCLUDE_SERVER_H
#define TOKEN_SERVICE_INCLUDE_SERVER_H
#pragma once

#include "common.h"

#include "TokenService.edl.h"

#include <string>

class Server
{
public:
    Server();
    ~Server() = default;

    int Run(std::string secretWord);

private:
    TokenService_entity_req m_req { };
    TokenService_entity_res m_res { };
    nk_arena                m_reqArena;
    nk_arena                m_resArena;

    char m_reqBuffer[TokenService_entity_req_arena_size] { };
    char m_resBuffer[TokenService_entity_res_arena_size] { };
};
#endif  // TOKEN_SERVICE_INCLUDE_SERVER_H
