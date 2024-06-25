/*
 * © 2024 AO Kaspersky Lab
 * Licensed under the MIT License
 */

#ifndef WEB_SERVER_INCLUDE_GET_DATA_HANDLER_H
#define WEB_SERVER_INCLUDE_GET_DATA_HANDLER_H
#pragma once

#include <CivetServer.h>

#include "i_tokenreader.h"

class GetDataHandler: public CivetHandler
{
public:
    GetDataHandler(ipc::ITokenReaderPtr tokenReader);
    ~GetDataHandler() override = default;

    bool handleGet(CivetServer* server, mg_connection* conn) override;

private:
    std::string GetParam(mg_connection* conn, const std::string& key) const;

    ipc::ITokenReaderPtr m_tokenReader;
};

#endif  // WEB_SERVER_INCLUDE_GET_DATA_HANDLER_H
