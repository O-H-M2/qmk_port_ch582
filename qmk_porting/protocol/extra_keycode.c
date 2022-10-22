#include "extra_keycode.h"
#include "quantum.h"

#ifdef BLE_ENABLE

bool process_record_ble_keycode(uint16_t keycode, keyrecord_t *record)
{
    if (record->event.pressed) {
        switch (keycode) {
            case USB_MODE:
                return false;
            case BLE_SLOT0:
            case BLE_SLOT1:
            case BLE_SLOT2:
            case BLE_SLOT3:
            case BLE_SLOT4:
            case BLE_SLOT5:
            case BLE_SLOT6:
            case BLE_SLOT7:
                if (keycode <= BLE_SLOT0 + BLE_SLOT_NUM - 1) {
                    hogp_slot_switch(keycode - BLE_SLOT0);
                }
                return false;
            case BLE_ALL_CLEAR:
                hogp_slot_clear(UINT8_MAX);
                return false;
            case ESB_MODE:
                return false;
            default:
                break;
        }
    }
    return process_record_user(keycode, record);
}

#endif