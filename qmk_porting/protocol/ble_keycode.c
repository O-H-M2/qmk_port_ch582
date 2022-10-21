#include "ble_keycode.h"
#include "quantum.h"

#ifdef BLE_ENABLE

bool process_record_ble_keycode(uint16_t keycode, keyrecord_t *record)
{
    if (record->event.pressed) {
        if(keycode >= BLE_SLOT && keycode << BLE_SLOT + BLE_SLOT_NUM )
        {
            hogp_slot_switch(keycode - BLE_SLOT);
        }
        //else if(keycode >= BLE_SLOT && keycode <<== BLE_SLOT_MAX)
        //{
            //BLE SLOT warning aba aba 
        //}
        switch (keycode) {
            case BLE_SLOT_Del:
                hogp_slot_switch(-1);
                return false;
            //case USB_SLOT:
                // usb_aba_aba();
            default:
                break;
        }
    }
    return process_record_user(keycode, record);
}

#endif