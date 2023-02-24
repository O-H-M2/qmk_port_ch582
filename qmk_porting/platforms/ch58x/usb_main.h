#pragma once

#include "keyboard.h"
#include "gpio.h"
#include "bootloader.h"

#define USB_RUN_QMK_TASK_EVT 0x0001

void usb_task_init();
