/*
 * © 2024 AO Kaspersky Lab
 * Licensed under the MIT License
 */

#ifndef TOKEN_SERVICE_INCLUDE_UTILS_H
#define TOKEN_SERVICE_INCLUDE_UTILS_H
#pragma once

#include <coresrv/nk/transport-kos.h>

#include <string>
#include <memory>

namespace utils {

std::string GetArenaString(const nk_arena* arena, const nk_ptr_t* src);

nk_err_t StringToArena(const std::string& str, nk_arena* arena, nk_ptr_t* dst);

}  // namespace utils

#endif  // TOKEN_SERVICE_INCLUDE_UTILS_H
