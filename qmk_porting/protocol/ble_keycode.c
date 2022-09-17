#include "ble_keycode.h"

#ifdef BLE_ENABLE

bool process_record_kb(uint16_t keycode, keyrecord_t *record)
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
            case KC_VENDOR_BT8: {
                tmos_event_hdr_t *pMsg = (tmos_event_hdr_t *)tmos_msg_allocate(sizeof(tmos_event_hdr_t));
                if (pMsg != NULL) {
                    pMsg->event = SLOT_SWITCH_EVT;
                    pMsg->status = keycode - KC_VENDOR_BT1;
                    if (tmos_msg_send(hogpTaskId, (uint8_t *)pMsg) == SUCCESS) {
#ifdef RGB_MATRIX_ENABLE
                        if (g_led_config.flags[keycode - KC_VENDOR_BT1 + 2] & LED_FLAG_KEYLIGHT) {
                            rgb_matrix_set_color(keycode - KC_VENDOR_BT1 + 2, 0, 167, 224);
                        }
#endif
                    }
                }
                return false;
            }
            case KC_VENDOR_BT_UNPAIR_ALL:
                tmos_set_event(hogpTaskId, WIPE_ALL_BONDS_EVT);
                return false;
            default:
                break;
        }
    }
    return process_record_user(keycode, record);
}

#endif