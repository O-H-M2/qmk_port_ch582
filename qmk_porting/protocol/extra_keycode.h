#pragma once

#ifdef DEBUG
#undef DEBUG
#endif
#include "quantum_keycodes.h"

enum {
    USB_MODE = 0x5F80,
    BLE_SLOT0,
    BLE_SLOT1,
    BLE_SLOT2,
    BLE_SLOT3,
    BLE_SLOT4,
    BLE_SLOT5,
    BLE_SLOT6,
    BLE_SLOT7,
    BLE_ALL_CLEAR,
    ESB_MODE,
};

// #undef SAFE_RANGE
// #define SAFE_RANGE MK01_SAFE_RANGE
