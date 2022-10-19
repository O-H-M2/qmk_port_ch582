#include "ble_keycode.h"
#include "quantum.h"

#ifdef BLE_ENABLE

// process_record_kb() is used
bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
    if (record->event.pressed) {
        switch (keycode) {
            case KC_VENDOR_BT1:
            case KC_VENDOR_BT2:
            case KC_VENDOR_BT3:
            case KC_VENDOR_BT4:
            case KC_VENDOR_BT5:
            case KC_VENDOR_BT6:
            case KC_VENDOR_BT7:
            case KC_VENDOR_BT8:
                if (hogp_slot_switch(keycode - KC_VENDOR_BT1)) {
#ifdef RGB_MATRIX_ENABLE
                    if (g_led_config.flags[keycode - KC_VENDOR_BT1 + 2] & LED_FLAG_KEYLIGHT) {
                        rgb_matrix_set_color(keycode - KC_VENDOR_BT1 + 2, 0, 167, 224);
                    }
#endif
                }
                return false;
            case KC_VENDOR_BT_UNPAIR_ALL:
                tmos_set_event(hogpTaskId, WIPE_ALL_BONDS_EVT);
                return false;
            default:
                break;
        }
    }
    return true;
}

#endif