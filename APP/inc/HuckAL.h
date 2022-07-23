#ifndef _HuckAL_
#define _HuckAL_

#include <stdint.h>
#include "CH58x_common.h"
#include "usbd_core.h"
#include "usbd_hid.h"

#ifdef DEBUG
#define HUCKAL_DEBUG 1
#endif

// prepare TRACE
#if HUCKAL_DEBUG
#define TRACE(...) PRINT(__VA_ARGS__)
#else
#define TRACE(...)
#endif

#define printf_ printf

// assert macros
#define _ASSERT(x)          \
    s = x;                  \
    if (s != ERR_SUCCESS) { \
        return (s);         \
    } else {                \
        ;                   \
    }

#define _assert(x)                          \
    s = x;                                  \
    if (s != ERR_SUCCESS) {                 \
        TRACE("err = %02X\n", (uint16_t)s); \
    } else {                                \
        ;                                   \
    }

#define SAFETY_DELAY \
    mDelayuS(300);

#endif