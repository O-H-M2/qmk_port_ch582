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

#include "host.h"
#include "usb_device_state.h"
#include "keycode_config.h"
#include "usb_main.h"

extern void keyboard_task();
extern void housekeeping_task();

void send_keyboard_usb(report_keyboard_t *report)
{
#ifdef NKRO_ENABLE
    if (keymap_config.nkro) {
        hid_nkro_keyboard_send_report((uint8_t *)&report->nkro, EXKEY_IN_EP_SIZE);
    } else
#endif
    {
        hid_bios_keyboard_send_report((uint8_t *)report, KEYBOARD_REPORT_SIZE);
    }
}

void send_mouse_usb(report_mouse_t *report)
{
#ifdef MOUSE_ENABLE
    uint8_t report_to_send[6];

    report_to_send[0] = REPORT_ID_MOUSE;
    memcpy(report_to_send + 1, report, 5);
    hid_exkey_send_report(report_to_send, 6);
#endif
}

void send_system_usb(uint16_t data)
{
    uint8_t report_to_send[3];

    report_to_send[0] = REPORT_ID_SYSTEM;
    memcpy(report_to_send + 1, &data, 2);
    hid_exkey_send_report(report_to_send, 3);
}

void send_consumer_usb(uint16_t data)
{
    uint8_t report_to_send[3];

    report_to_send[0] = REPORT_ID_CONSUMER;
    memcpy(report_to_send + 1, &data, 2);
    hid_exkey_send_report(report_to_send, 3);
}

void send_programmable_button_usb(uint32_t data)
{
}

void send_digitizer_usb(report_digitizer_t *report)
{
}

void raw_hid_send_usb(uint8_t *data, uint8_t length)
{
    hid_custom_send_report(data, length);
}

void platform_initialize_usb()
{
    Calibration_LSI(Level_128);
}

void protocol_pre_init_usb()
{
    init_usb_driver();
}

__HIGH_CODE void protocol_task_usb()
{
    for (;;) {
        keyboard_task();
        housekeeping_task();
    }
}

void platform_reboot_usb()
{
    // wait for usb sending done
    DelayMs(10);
    PRINT("Reboot execute.\n");
    WAIT_FOR_DBG;
    SYS_ResetExecute();
}
