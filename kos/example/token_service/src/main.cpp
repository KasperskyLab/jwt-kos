/*
 * © 2024 AO Kaspersky Lab
 * Licensed under the MIT License
 */

#include "general.h"
#include "server.h"

#include <iostream>

#include <sys/types.h>

int main()
{
    std::cout << app::AppTag << "Service started" << std::endl;

    Server server;
    auto retCode = server.Run("some secret word");

    std::cout << app::AppTag << "Service stoped. Exit code = " << retCode << std::endl;

    return retCode;
}
