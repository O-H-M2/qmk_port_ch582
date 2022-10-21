#pragma once

#ifdef DEBUG
#undef DEBUG
#endif
#include "quantum_keycodes.h"

enum {
    BLE_SLOT= 0x5F80,
    BLE_SLOT0 = 0x5F80,
    BLE_SLOT1,
    BLE_SLOT2,
    BLE_SLOT3,
    BLE_SLOT4,
    BLE_SLOT5,
    BLE_SLOT6,
    BLE_SLOT7,
    BLE_SLOT_MAX = 0x5F87,
    BLE_SLOT_Del = 0x5F88,
    USB_SLOT,
};

// #undef SAFE_RANGE
// #define SAFE_RANGE MK01_SAFE_RANGE
