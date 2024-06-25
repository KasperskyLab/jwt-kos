/*
 * © 2024 AO Kaspersky Lab
 * Licensed under the MIT License
 */

#include "server.h"
#include "general.h"
#include "token_handler.h"

#include <connections.h>
#include <coresrv/nk/transport-kos.h>
#include <coresrv/sl/sl_api.h>

#include <fcntl.h>
#include <iostream>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

Server::Server()
    : m_reqArena(NK_ARENA_INITIALIZER(m_reqBuffer, m_reqBuffer + sizeof(m_reqBuffer)))
    , m_resArena(NK_ARENA_INITIALIZER(m_resBuffer, m_resBuffer + sizeof(m_resBuffer)))
{}

int Server::Run(std::string secretWord)
{
    ServiceId iid;
    NkKosTransport transport;

    // Get server IPC-descriptor for connection
    auto handleClients = ServiceLocatorRegister(connections::ConnectionName, NULL, 0, &iid);
    if (handleClients == INVALID_HANDLE)
    {
        std::cerr
          << app::AppTag << "Error: can`t establish static IPC connection! Connection name: "
          << connections::ConnectionName
          << std::endl;
        return EXIT_FAILURE;
    }

    // Initialize transport to clients
    NkKosTransport_Init(&transport, handleClients, NK_NULL, 0);

    // Initialize Logger entity dispatcher
    TokenService_entity entity;
    TokenService_entity_init(&entity, TokenHandler::CreateImpl(std::move(secretWord)));

    // Main cycle: requests execution.
    while (true)
    {
        nk_req_reset(&m_req);
        nk_arena_reset(&m_reqArena);
        nk_req_reset(&m_res);
        nk_arena_reset(&m_resArena);

        auto resCode = nk_transport_recv(&transport.base, &m_req.base_, &m_reqArena);
        if (resCode == NK_EOK)
        {
            TokenService_entity_dispatch(&entity, &m_req.base_, &m_reqArena, &m_res.base_, &m_resArena);
        }
        else
        {
            std::cerr << app::AppTag << "Error: nk_transport_recv is not OK. Error code = " << resCode << std::endl;
        }

        resCode = nk_transport_reply(&transport.base, &m_res.base_, &m_resArena);
        if (resCode != NK_EOK)
        {
            std::cerr << app::AppTag << "Error: nk_transport_reply is not OK. Error code = " << resCode << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
