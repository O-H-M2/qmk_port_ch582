#pragma once

#ifdef USB_ENABLE
#include "eventhandler_usb.h"
#endif
#ifdef BLE_ENABLE
#include "eventhandler_ble.h"
#endif
#ifdef ESB_ENABLE
#include "eventhandler_esb.h"
#endif

_Static_assert(BLE_EVENT_START_INDEX > USB_EVENT_MAX, "");
_Static_assert(ESB_EVENT_START_INDEX > BLE_EVENT_MAX, "");

extern volatile uint8_t kbd_protocol_type;

__attribute__((always_inline)) inline void event_propagate(uint8_t event, void *param)
{
    switch (event) {
        case PLATFORM_EVENT_MODE_SELECT:
            // TODO: implement the mode select conditions
#ifdef USB_ENABLE
            if (0) {
                // cable mode
                kbd_protocol_type = kbd_protocol_usb;
            }
#endif
#ifdef BLE_ENABLE
            if (1) {
                // bluetooth mode
                kbd_protocol_type = kbd_protocol_ble;
            }
#endif
#ifdef ESB_ENABLE
            if (0) {
                // 2.4g mode
                kbd_protocol_type = kbd_protocol_esb;
            }
#endif
            return;
    }
#ifdef USB_ENABLE
    if (kbd_protocol_type == kbd_protocol_usb) {
        event_handler_usb(event, param);
    }
#endif
#ifdef BLE_ENABLE
    if (kbd_protocol_type == kbd_protocol_ble) {
        event_handler_ble(event, param);
    }
#endif
#ifdef ESB_ENABLE
    if (kbd_protocol_type == kbd_protocol_esb) {
        event_handler_esb(event, param);
    }
#endif
}
