/*
 * © 2024 AO Kaspersky Lab
 * Licensed under the MIT License
 */

#include "logger.h"
#include "general.h"

#include <iostream>

void logger::Log(const mg_connection* conn)
{
    if (conn == nullptr)
        return;

    const mg_request_info* ri = mg_get_request_info(conn);
    if (ri == nullptr)
        return;

    std::cout << app::AppTag << "Local URL: " << ri->local_uri << std::endl;
    std::cout << app::AppTag << "Request: " << ri->request_method << std::endl;
    std::cout << app::AppTag << "RequestURI: " << ri->request_uri << std::endl;
}
