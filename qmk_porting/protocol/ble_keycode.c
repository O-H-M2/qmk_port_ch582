#include "ble_keycode.h"
#include "quantum.h"

#ifdef BLE_ENABLE

bool process_record_ble_keycode(uint16_t keycode, keyrecord_t *record)
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
                hogp_slot_switch(-1);
                return false;
            default:
                break;
        }
    }
    return process_record_user(keycode, record);
}

#endif