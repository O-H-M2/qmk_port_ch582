#pragma once

#include <stdint.h>
#include "host.h"

void platform_initialize_usb();
void protocol_init_usb();
void protocol_task_usb();
void platform_reboot_usb();

void send_keyboard_usb(report_keyboard_t *report);
void send_mouse_usb(report_mouse_t *report);
void send_extra_usb(report_extra_t *report);
void raw_hid_send_usb(uint8_t *data, uint8_t length);
void rgb_raw_hid_send_usb(uint8_t *data, uint8_t length);
