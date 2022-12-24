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

#ifndef __MCUBOOT_CONFIG_H__
#define __MCUBOOT_CONFIG_H__

#include "CH58x_common.h"

/*
 * Template configuration file for MCUboot.
 *
 * When porting MCUboot to a new target, copy it somewhere that your
 * include path can find it as mcuboot_config/mcuboot_config.h, and
 * make adjustments to suit your platform.
 *
 * For examples, see:
 *
 * boot/zephyr/include/mcuboot_config/mcuboot_config.h
 * boot/mynewt/mcuboot_config/include/mcuboot_config/mcuboot_config.h
 */

/*
 * Signature types
 *
 * You must choose exactly one signature type.
 */

/* Uncomment for RSA signature support */
#if defined BLE_ENABLE || (defined ESB_ENABLE && (ESB_ENABLE == 1 || ESB_ENABLE == 2))
#define MCUBOOT_SIGN_RSA
#endif

/* Uncomment for ECDSA signatures using curve P-256. */
// #define MCUBOOT_SIGN_EC256

/*
 * Upgrade mode
 *
 * The default is to support A/B image swapping with rollback.  Other modes
 * with simpler code path, which only supports overwriting the existing image
 * with the update image or running the newest image directly from its flash
 * partition, are also available.
 *
 * You can enable only one mode at a time from the list below to override
 * the default upgrade mode.
 */

/* Uncomment to enable the overwrite-only code path. */
/* #define MCUBOOT_OVERWRITE_ONLY */

#ifdef MCUBOOT_OVERWRITE_ONLY
/* Uncomment to only erase and overwrite those primary slot sectors needed
 * to install the new image, rather than the entire image slot. */
/* #define MCUBOOT_OVERWRITE_ONLY_FAST */
#endif

/* Uncomment to enable the direct-xip code path. */
#define MCUBOOT_DIRECT_XIP
/* Uncomment to enable the revert mechanism in direct-xip mode. */
/* #define MCUBOOT_DIRECT_XIP_REVERT */

/* Uncomment to enable the ram-load code path. */
/* #define MCUBOOT_RAM_LOAD */

/*
 * Cryptographic settings
 *
 * You must choose between mbedTLS and Tinycrypt as source of
 * cryptographic primitives. Other cryptographic settings are also
 * available.
 */

/* Uncomment to use ARM's mbedTLS cryptographic primitives */
#define MCUBOOT_USE_MBED_TLS
/* Uncomment to use Tinycrypt's. */
/* #define MCUBOOT_USE_TINYCRYPT */

/*
 * Always check the signature of the image in the primary slot before booting,
 * even if no upgrade was performed. This is recommended if the boot
 * time penalty is acceptable.
 */
#define MCUBOOT_VALIDATE_PRIMARY_SLOT

/*
 * Flash abstraction
 */

/* Uncomment if your flash map API supports flash_area_get_sectors().
 * See the flash APIs for more details. */
/* #define MCUBOOT_USE_FLASH_AREA_GET_SECTORS */

/* Default maximum number of flash sectors per image slot; change
 * as desirable. */
#define MCUBOOT_MAX_IMG_SECTORS 128

/* Default number of separately updateable images; change in case of
 * multiple images. */
#define MCUBOOT_IMAGE_NUMBER 1

/*
 * Logging
 */

/*
 * If logging is enabled the following functions must be defined by the
 * platform:
 *
 *    MCUBOOT_LOG_MODULE_REGISTER(domain)
 *      Register a new log module and add the current C file to it.
 *
 *    MCUBOOT_LOG_MODULE_DECLARE(domain)
 *      Add the current C file to an existing log module.
 *
 *    MCUBOOT_LOG_ERR(...)
 *    MCUBOOT_LOG_WRN(...)
 *    MCUBOOT_LOG_INF(...)
 *    MCUBOOT_LOG_DBG(...)
 *
 * The function priority is:
 *
 *    MCUBOOT_LOG_ERR > MCUBOOT_LOG_WRN > MCUBOOT_LOG_INF > MCUBOOT_LOG_DBG
 */
#define MCUBOOT_HAVE_LOGGING 0

/*
 * Assertions
 */

/* Uncomment if your platform has its own mcuboot_config/mcuboot_assert.h.
 * If so, it must provide an ASSERT macro for use by bootutil. Otherwise,
 * "assert" is used. */
/* #define MCUBOOT_HAVE_ASSERT_H */

/*
 * Watchdog feeding
 */

/* This macro might be implemented if the OS / HW watchdog is enabled while
 * doing a swap upgrade and the time it takes for a swapping is long enough
 * to cause an unwanted reset. If implementing this, the OS main.c must also
 * enable the watchdog (if required)!
 *
 * #define MCUBOOT_WATCHDOG_FEED()
 *    do { do watchdog feeding here! } while (0)
 */
#define MCUBOOT_WATCHDOG_FEED() \
    do {                        \
        R8_WDOG_COUNT = 0;      \
    } while (0);

/* If a OS ports support single thread mode or is bare-metal then:
 * This macro implements call that switches CPU to an idle state, from which
 * the CPU may be woken up by, for example, UART transmission event.
 * 
 * Otherwise this macro should be no-op.
 */
#define MCUBOOT_CPU_IDLE() \
    do {                   \
        __nop();           \
    } while (0);

#endif /* __MCUBOOT_CONFIG_H__ */

#ifdef MCUBOOT_SIGN_RSA
#define MCUBOOT_SIGN_RSA_LEN 2048
#elif defined(MCUBOOT_SIGN_EC256)
#define MCUBOOT_SIGN_EC256
#endif
