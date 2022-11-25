#pragma once

#include <stdint.h>
#include "host.h"

void platform_initialize_usb();
void protocol_pre_init_usb();
void protocol_task_usb();
void platform_reboot_usb();

void send_keyboard_usb(report_keyboard_t *report);
void send_mouse_usb(report_mouse_t *report);
void send_system_usb(uint16_t data);
void send_consumer_usb(uint16_t data);
void send_programmable_button_usb(uint32_t data);
void raw_hid_send_usb(uint8_t *data, uint8_t length);
