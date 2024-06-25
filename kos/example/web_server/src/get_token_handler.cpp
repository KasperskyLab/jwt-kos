/*
* © 2024 AO Kaspersky Lab
* Licensed under the MIT License
*/

#include "get_token_handler.h"

#include "general.h"
#include "logger.h"

#include <iostream>
#include <string>

GetTokenHandler::GetTokenHandler(ipc::ITokenReaderPtr tokenReader) :
        m_tokenReader(std::move(tokenReader))
{}

bool GetTokenHandler::handleGet(CivetServer*, mg_connection* conn)
{
    logger::Log(conn);

    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: "
              "text/html\r\nConnection: close\r\n\r\n");

    std::string token;

    m_tokenReader->Generate(token);

    mg_printf(conn, "%s\r\n", token.c_str());

    return true;
}
