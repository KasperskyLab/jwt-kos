/*
 * © 2024 AO Kaspersky Lab
 * Licensed under the MIT License
 */

#include "token_handler.h"
#include "general.h"
#include "utils.h"

#include <jwt/jwt.hpp>

#include <coresrv/nk/transport-kos.h>
#include <coresrv/sl/sl_api.h>
#include <rtl/stdio.h>
#include <rtl/string.h>

#include <iostream>

namespace {
    const char* payloadKey   = "secret";
    const char* payloadValue = "TOKEN";
}

struct TokenHandlerImpl: IToken_ops
{
    TokenHandler* handler = nullptr;
};

nk_err_t GenerateImpl(IToken* self,
                      const IToken_Generate_req* req,
                      const nk_arena* reqArena,
                      IToken_Generate_res* res,
                      nk_arena* resArena)
{
    auto ops = self != nullptr ? static_cast<const TokenHandlerImpl*>(self->ops) : nullptr;
    if (ops == nullptr)
    {
        return NK_ENOENT;
    }

    auto token = ops->handler->Generate();

    return utils::StringToArena(token, resArena, &res->token);
}

nk_err_t VerifyImpl(IToken* self, const IToken_Verify_req* req, const nk_arena* reqArena, IToken_Verify_res* res, nk_arena* resArena)
{
    auto ops = self != nullptr ? static_cast<const TokenHandlerImpl*>(self->ops) : nullptr;
    if (ops == nullptr)
    {
        return NK_ENOENT;
    }

    res->result = ops->handler->Verify(utils::GetArenaString(reqArena, &req->token));

    return NK_EOK;
}

IToken* TokenHandler::CreateImpl(std::string secretWord)
{
    static TokenHandler token(std::move(secretWord));
    static TokenHandlerImpl ops { };
    static IToken impl = { .ops = &ops };

    ops.Generate = GenerateImpl;
    ops.Verify = VerifyImpl;
    ops.handler = &token;

    return &impl;
}

TokenHandler::TokenHandler(std::string secretWord) :
        m_secretWord(std::move(secretWord))
{
}

bool TokenHandler::Verify(const std::string& token)
{
    try
    {
        auto dec_obj = jwt::decode(token, jwt::params::algorithms( { "HS256" }), jwt::params::secret(m_secretWord));
        std::cout << "[" << app::AppTag << "][HEADER  ]" << dec_obj.header() << std::endl;
        std::cout << "[" << app::AppTag << "][PAYLOAD ]" << dec_obj.payload() << std::endl;
        if (dec_obj.payload().has_claim(payloadKey) && dec_obj.payload().get_claim_value<std::string>(payloadKey) == payloadValue)
        {
            return true;
        }
    } catch (jwt::SignatureFormatError& err)
    {
        std::cout << "[" << app::AppTag << "] Token parse error: " << err.what() << std::endl;
    }

    return false;
}

std::string TokenHandler::Generate()
{
    std::cout << "[" << app::AppTag << "] *** [GetJWTToken] ***" << std::endl;

    std::string token;

    try
    {
        // Create JWT object
        jwt::jwt_object obj { jwt::params::algorithm("HS256"), jwt::params::payload( { { payloadKey, payloadValue } }),
                          jwt::params::secret(m_secretWord) };

        token = obj.signature();

    } catch (std::exception& err)
    {
        std::cout << "[" << app::AppTag << "] Token create error: " << err.what() << std::endl;
    }

    return token;
}
