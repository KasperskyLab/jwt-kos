/*
 * © 2024 AO Kaspersky Lab
 * Licensed under the MIT License
 */

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <rtl/string.h>

/* Files required for transport initialisation. */
#include <coresrv/nk/transport-kos.h>
#include <coresrv/sl/sl_api.h>

/**
 * NK_USE_UNQUALIFIED_NAMES macro enables usage of short names,
 * generated by nk compiler.
 */
#define NK_USE_UNQUALIFIED_NAMES

#endif
