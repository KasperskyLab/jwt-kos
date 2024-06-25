/*
 * © 2024 AO Kaspersky Lab
 * Licensed under the MIT License
 */

#include "get_data_handler.h"
#include "get_token_handler.h"
#include "general.h"
#include "kos_connector.h"

#include <CivetServer.h>
#include <connections.h>

#include <kos_net.h>

#include <cstring>
#include <iostream>
#include <memory>

#include <openssl/rand.h>
#include <unistd.h>

namespace consts {
constexpr const char *GetDataUri = "/getdata";
constexpr const char *GetTokenUri = "/gettoken";

constexpr const char *DocumentRoot = "/www";
constexpr const char *Port = "1106";
}  // namespace consts

// Exit flag for main loop.
volatile bool exitNow = false;

void Run(const std::vector<std::string>& cppOptions, KosConnectorPtr connector)
{
    CivetServer server(cppOptions);

    GetDataHandler hGetData(connector);
    server.addHandler(consts::GetDataUri, hGetData);

    GetTokenHandler hGetToken(connector);
    server.addHandler(consts::GetTokenUri, hGetToken);

    std::cout << app::AppTag << "WebServer started (port: " << consts::Port << ")" << std::endl;

    while (!exitNow)
    {
        sleep(1);
    }

    std::cout << app::AppTag << "WebServer stoped" << std::endl;
}

int main()
{
    if (!wait_for_network())
    {
        std::cerr << app::AppTag << "Error: Wait for network failed!" << std::endl;
        return EXIT_FAILURE;
    }

    auto kosCon = std::make_shared<KosConnector>();
    if (!kosCon->Connect())
    {
        std::cerr << app::AppTag << "Error: Unable to establish IPS connection to "
                "authentication server." << std::endl;
        return EXIT_FAILURE;
    }

    mg_init_library(0);

    std::vector<std::string> options = { "document_root", consts::DocumentRoot, "listening_ports", consts::Port };
    Run(options, kosCon);

    mg_exit_library();

    return EXIT_SUCCESS;
}
