/*
 * © 2024 AO Kaspersky Lab
 * Licensed under the MIT License
 */

#include "utils.h"

#include <errno.h>
#include <string.h>

#include <iomanip>
#include <iostream>
#include <sstream>

#include <rtl/string.h>

namespace utils {
std::string GetArenaString(const nk_arena* arena, const nk_ptr_t* src)
{
    nk_uint32_t msgLength = 0;
    nk_char_t* nkMsg = nk_arena_get(nk_char_t, arena, src, &msgLength);
    return nkMsg != nullptr ? nkMsg : "";
}

nk_err_t StringToArena(const std::string& str, nk_arena* arena, nk_ptr_t* dst)
{
    nk_char_t* nk_str = nk_arena_alloc(nk_char_t, arena, dst, str.length() + 1);
    if (nk_str == nullptr)
    {
        return NK_ENOMEM;
    }
    rtl_strncpy(nk_str, str.c_str(), str.length() + 1);
    return NK_EOK;
}

}  // namespace utils
