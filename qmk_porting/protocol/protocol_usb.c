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

#include "protocol_usb.h"
#include "usb_device_state.h"
#include "keycode_config.h"
#include "usb_main.h"
#include "usb_interface.h"
#ifdef RGB_RAW_ENABLE
#include "auxiliary_rgb.h"
#endif
#ifdef RAW_ENABLE
#include "raw_hid.h"
#endif

extern int usbd_deinitialize();

static void send_keyboard(report_keyboard_t *report)
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

static void send_mouse(report_mouse_t *report)
{
#ifdef MOUSE_ENABLE
    uint8_t report_to_send[6];

    report_to_send[0] = REPORT_ID_MOUSE;
    memcpy(report_to_send + 1, report, 5);
    hid_exkey_send_report(report_to_send, 6);
#endif
}

static void send_extra(report_extra_t *report)
{
    hid_exkey_send_report((uint8_t *)report, sizeof(report_extra_t));
}

#ifdef RAW_ENABLE
static void send_qmk_raw(uint8_t *data, uint8_t length)
{
    hid_qmk_raw_send_report(data, length);
}
#endif

#ifdef RGB_RAW_ENABLE
static void send_rgb_raw(uint8_t *data, uint8_t length)
{
    hid_rgb_raw_send_report(data, length);
}
#endif

static void platform_initialize()
{
#ifndef PLF_DEBUG
    while ((R8_UART1_LSR & RB_LSR_TX_ALL_EMP) == 0) {
        __nop();
    }
    UART1_Reset();
    setPinInputLow(A8);
    setPinInputLow(A9);
#endif
}

static void protocol_setup()
{
    usb_device_state_init();
    usb_task_init();
}

static void protocol_init()
{
    init_usb_driver();
}

static void platform_reboot()
{
    // wait for usb sending done
    DelayMs(10);
    PRINT("Reboot execute.\n");
    WAIT_FOR_DBG;
    SYS_ResetExecute();
}

__HIGH_CODE static void protocol_task()
{
    TMOS_SystemProcess();
}

const ch582_interface_t ch582_protocol_usb = {
    .ch582_common_driver.keyboard_leds = keyboard_leds,
    .ch582_common_driver.send_keyboard = send_keyboard,
    .ch582_common_driver.send_mouse = send_mouse,
    .ch582_common_driver.send_extra = send_extra,
#ifdef RAW_ENABLE
    .send_qmk_raw = send_qmk_raw,
    .receive_qmk_raw = raw_hid_receive,
#endif
#ifdef RGB_RAW_ENABLE
    .send_rgb_raw = send_rgb_raw,
    .receive_rgb_raw = rgb_raw_hid_receive,
#endif
    .ch582_platform_initialize = platform_initialize,
    .ch582_protocol_setup = protocol_setup,
    .ch582_protocol_init = protocol_init,
    .ch582_protocol_task = protocol_task,
    .ch582_platform_reboot = platform_reboot,
};
