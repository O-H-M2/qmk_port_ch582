#pragma once

#include <stdint.h>
#include "quantum.h"
#include "quantum_keycodes.h"

enum {
    KC_VENDOR_BT1 = SAFE_RANGE + 1,
    KC_VENDOR_BT2,
    KC_VENDOR_BT3,
    KC_VENDOR_BT4,
    KC_VENDOR_BT5,
    KC_VENDOR_BT6,
    KC_VENDOR_BT7,
    KC_VENDOR_BT8,
    KC_VENDOR_BT_UNPAIR_ALL,
    MK01_SAFE_RANGE,
};

// #undef SAFE_RANGE
// #define SAFE_RANGE MK01_SAFE_RANGE
