/*
Copyright 2022 Huckies <https://github.com/Huckies>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#ifdef USB_ENABLE
#include "eventhandler_usb.h"
_Static_assert(BLE_EVENT_START_INDEX > USB_EVENT_MAX, "");
#endif
#ifdef BLE_ENABLE
#include "eventhandler_ble.h"
_Static_assert(ESB_EVENT_START_INDEX > BLE_EVENT_MAX, "");
#endif
#ifdef ESB_ENABLE
#include "eventhandler_esb.h"
#endif

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
