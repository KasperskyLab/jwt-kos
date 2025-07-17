/*
* © 2025 AO Kaspersky Lab
* Licensed under the MIT License
*/

#include "get_data_handler.h"
#include "general.h"
#include "logger.h"

#include <iostream>
#include <string>

GetDataHandler::GetDataHandler(ipc::ITokenReaderPtr tokenReader) :
        m_tokenReader(std::move(tokenReader))
{}

bool GetDataHandler::handleGet(CivetServer*, mg_connection* conn)
{
    logger::Log(conn);

    auto token = GetParam(conn, "jwt");

    mg_printf(conn, "HTTP/1.1 200 OK\r\n"
              "Content-Type: text/html\r\n"
              "Connection: close\r\n\r\n");

    if (m_tokenReader->Verify(token))
    {
        mg_printf(conn, "%s\r\n", "<div class=\"success\">Data access succeeded</div>");
    }
    else
    {
        mg_printf(conn, "%s\r\n", "<div class=\"error\">Wrong token or token service unavailable</div>");
    }

    return true;
}

std::string GetDataHandler::GetParam(mg_connection* conn, const std::string& key) const
{
    std::string value;
    if (!CivetServer::getParam(conn, key.c_str(), value))
    {
        return "";
    }
    return value;
}
