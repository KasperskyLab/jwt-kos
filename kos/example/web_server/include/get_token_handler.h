/*
 * © 2024 AO Kaspersky Lab
 * Licensed under the MIT License
 */

#ifndef WEB_SERVER_INCLUDE_GET_TOKEN_HANDLER_H
#define WEB_SERVER_INCLUDE_GET_TOKEN_HANDLER_H
#pragma once

#include <CivetServer.h>

#include "i_tokenreader.h"

class GetTokenHandler: public CivetHandler
{
public:
    GetTokenHandler(ipc::ITokenReaderPtr tokenReader);
    ~GetTokenHandler() override = default;

    bool handleGet(CivetServer* server, mg_connection* conn) override;

private:
    ipc::ITokenReaderPtr m_tokenReader;
};

#endif  // WEB_SERVER_INCLUDE_GET_TOKEN_HANDLER_H
