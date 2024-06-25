/*
 * © 2024 AO Kaspersky Lab
 * Licensed under the MIT License
 */
#ifndef WEB_SERVER_INCLUDE_LOGGER_H
#define WEB_SERVER_INCLUDE_LOGGER_H
#pragma once

#include <CivetServer.h>

namespace logger {
// Log output of the WEB-server handler parameters.
void Log(const mg_connection* conn);

}  // namespace logger

#endif  // WEB_SERVER_INCLUDE_LOGGER_H
