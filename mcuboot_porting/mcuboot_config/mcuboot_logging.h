/*
Copyright 2022 Huckies <https://github.com/Huckies>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <stdio.h>

#define MCUBOOT_LOG_LEVEL_OFF     0
#define MCUBOOT_LOG_LEVEL_ERROR   1
#define MCUBOOT_LOG_LEVEL_WARNING 2
#define MCUBOOT_LOG_LEVEL_INFO    3
#define MCUBOOT_LOG_LEVEL_DEBUG   4
#define MCUBOOT_LOG_LEVEL_SIM     5

#ifndef MCUBOOT_LOG_LEVEL
#define MCUBOOT_LOG_LEVEL MCUBOOT_LOG_LEVEL_DEBUG
#endif

#if !defined MCUBOOT_HAVE_LOGGING
#undef MCUBOOT_LOG_LEVEL
#define MCUBOOT_LOG_LEVEL MCUBOOT_LOG_LEVEL_OFF
#endif

#define MCUBOOT_LOG_MODULE_DECLARE(domain)  /* ignore */
#define MCUBOOT_LOG_MODULE_REGISTER(domain) /* ignore */

int sim_log_enabled(int level);

#if MCUBOOT_LOG_LEVEL >= MCUBOOT_LOG_LEVEL_ERROR
#define MCUBOOT_LOG_ERR(_fmt, ...)                  \
    do {                                            \
        printf("[E_MCB]" _fmt "\n", ##__VA_ARGS__); \
    } while (0)
#else
#define MCUBOOT_LOG_ERR(...)
#endif

#if MCUBOOT_LOG_LEVEL >= MCUBOOT_LOG_LEVEL_WARNING
#define MCUBOOT_LOG_WRN(_fmt, ...)                  \
    do {                                            \
        printf("[W_MCB]" _fmt "\n", ##__VA_ARGS__); \
    } while (0)
#else
#define MCUBOOT_LOG_WRN(...)
#endif

#if MCUBOOT_LOG_LEVEL >= MCUBOOT_LOG_LEVEL_INFO
#define MCUBOOT_LOG_INF(_fmt, ...)                  \
    do {                                            \
        printf("[I_MCB]" _fmt "\n", ##__VA_ARGS__); \
    } while (0)
#else
#define MCUBOOT_LOG_INF(...)
#endif

#if MCUBOOT_LOG_LEVEL >= MCUBOOT_LOG_LEVEL_DEBUG
#define MCUBOOT_LOG_DBG(_fmt, ...)                  \
    do {                                            \
        printf("[D_MCB]" _fmt "\n", ##__VA_ARGS__); \
    } while (0)
#else
#define MCUBOOT_LOG_DBG(...)
#endif

#if MCUBOOT_LOG_LEVEL >= MCUBOOT_LOG_LEVEL_SIM
#define MCUBOOT_LOG_SIM(_fmt, ...)                  \
    do {                                            \
        printf("[S_MCB]" _fmt "\n", ##__VA_ARGS__); \
    } while (0)
#else
#define MCUBOOT_LOG_SIM(...)
#endif
