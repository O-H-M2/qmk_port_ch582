#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "color.h"

#ifndef RGB_RAW_USAGE_PAGE
#define RGB_RAW_USAGE_PAGE 0xFF60
#endif

#ifndef RGB_RAW_USAGE_ID
#define RGB_RAW_USAGE_ID 0x61
#endif

#if !defined(AUXILIARY_RGB_STARTUP_RED)
#define AUXILIARY_RGB_STARTUP_RED 0
#endif

#if !defined(AUXILIARY_RGB_STARTUP_GREEN)
#define AUXILIARY_RGB_STARTUP_GREEN 167
#endif

#if !defined(AUXILIARY_RGB_STARTUP_BLUE)
#define AUXILIARY_RGB_STARTUP_BLUE 224
#endif

enum hidrgb_mode {
    HID_MODE_OPENRGB = 1,
    HID_MODE_SIGNALRGB,
};

void rgb_raw_hid_receive(uint8_t *data, uint8_t length);
void rgb_raw_hid_send(uint8_t *data, uint8_t length);

void auxiliary_rgb_set_color(int index, uint8_t red, uint8_t green, uint8_t blue); //set one led color
void auxiliary_rgb_set_mode(int mode);                                             //set hidrgb software mode, HID_MODE_OPENRGB or HID_MODE_SIGNALRGB
uint8_t auxiliary_rgb_get_mode();                                                  //get software mode

void auxiliary_rgb_reload_openrgb_colors(); //reload openrgb colors
void auxiliary_rgb_reload_openrgb_anim();   //reload openrgb colors in animation
RGB *auxiliary_rgb_get_openrgb_colors();    //get openrgb colors
RGB auxiliary_rgb_get_openrgb_color(int index);