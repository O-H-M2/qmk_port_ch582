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

#include "rgb_led.h"
#include "usb_main.h"

enum {
    USB_EVENT_CONNECT = USB_EVENT_START_INDEX,
    USB_EVENT_MAX,
};

extern void keyboard_task();
extern void housekeeping_task();

__attribute__((always_inline)) inline void event_handler_usb(uint8_t event, void *param)
{
    switch (event) {
        case PLATFORM_EVENT_INITIALIZE:
            Calibration_LSI(Level_64);
            break;
        case PROTOCOL_EVENT_PRE_INIT:
            init_usb_driver();
            break;
        case PROTOCOL_EVENT_RUN:
            keyboard_task();
            housekeeping_task();
            break;
        case PLATFORM_EVENT_REBOOT:
            // wait for usb sending done
            DelayMs(10);
            PRINT("Reboot execute.\n");
            WAIT_FOR_DBG;
            SYS_ResetExecute();
            break;
        default:
            PRINT("Unhandled usb event: %d\n", event);
            break;
    }
}