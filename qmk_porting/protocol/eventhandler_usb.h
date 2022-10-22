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
        default:
            PRINT("Unhandled usb event: %d\n", event);
            break;
    }
}