/*
 * © 2024 AO Kaspersky Lab
 * Licensed under the MIT License
 */
#ifndef WEB_SERVER_INCLUDE_KOS_CONNECTOR_H
#define WEB_SERVER_INCLUDE_KOS_CONNECTOR_H
#pragma once

#include "common.h"
#include "i_tokenreader.h"

#include <IToken.idl.h>

#include <memory>

class KosConnector: public ipc::ITokenReader
{
public:
    KosConnector() = default;

    // Open IPC connection.
    bool Connect();

    bool Generate(std::string& token) override;
    bool Verify(const std::string& token) override;

private:
    NkKosTransport m_transport;
    IToken_proxy m_tokenProxy;
};

using KosConnectorPtr = std::shared_ptr<KosConnector>;
#endif  // WEB_SERVER_INCLUDE_KOS_CONNECTOR_H
