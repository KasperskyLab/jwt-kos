/*
 * © 2024 AO Kaspersky Lab
 * Licensed under the MIT License
 */

#include "kos_connector.h"
#include "connections.h"
#include "general.h"

#include <rtl/string.h>

#include <iostream>

#include "utils.h"

constexpr const char *Tag = "[KosConnector] ";


bool KosConnector::Connect()
{
    auto handle = ServiceLocatorConnect(connections::ConnectionName);
    if (handle == INVALID_HANDLE)
    {
        std::cerr << app::AppTag << Tag << "Error: can`t establish static IPC connection! "
                  "Connection Name: " << connections::ConnectionName << std::endl;

        return false;
    }
    NkKosTransport_Init(&m_transport, handle, NK_NULL, 0);

    // Init runtime implementations of ILoginForm.
    {
        nk_iid_t riid = ServiceLocatorGetRiid(handle, connections::TokenConnectionPoint);
        if (riid == INVALID_RIID)
        {
            std::cerr << app::AppTag << "Error: can`t get runtime implementation ID (RIID) of "
                    "interface '" << connections::TokenConnectionPoint << "'" << std::endl;
            return false;
        }
        IToken_proxy_init(&m_tokenProxy, &m_transport.base, riid);
    }

    return true;
}

bool KosConnector::Generate(std::string& token)
{
    IToken_Generate_req request { };
    IToken_Generate_res response { };
    char resBuffer[IToken_Generate_res_arena_size];

    nk_arena resArena = NK_ARENA_INITIALIZER(resBuffer, resBuffer + sizeof(resBuffer));

    auto code = IToken_Generate(&m_tokenProxy.base, &request, nullptr, &response, &resArena);
    if (code == NK_EOK)
    {
        uint64_t msgLen = 0;
        nk_char_t* msgPtr = nk_arena_get(nk_char_t, &resArena, &response.token, &msgLen);
        token = msgPtr != nullptr ? msgPtr : "";
    }

    return code == NK_EOK;
}

bool KosConnector::Verify(const std::string& token)
{
    IToken_Verify_req request { };
    char reqBuffer[IToken_Verify_req_arena_size];
    IToken_Verify_res response { };
    char resBuffer[IToken_Verify_res_arena_size + 1];

    nk_arena reqArena = NK_ARENA_INITIALIZER(reqBuffer, reqBuffer + sizeof(reqBuffer));
    nk_arena resArena = NK_ARENA_INITIALIZER(resBuffer, resBuffer + sizeof(resBuffer));

    if (auto code = utils::StringToArena(token, &reqArena, &request.token); code != NK_EOK)
    {
        std::cerr << app::AppTag << __func__ << " Error: unable to allocate arena request token: '" << token << "'!" << std::endl;
        return code;
    }

    auto code = IToken_Verify(&m_tokenProxy.base, &request, &reqArena, &response, &resArena);

    return code == NK_EOK ? response.result : false;
}
