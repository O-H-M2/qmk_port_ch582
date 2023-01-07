#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "color.h"
#include "usb_main.h"
#include "rgb_matrix.h"

enum hidrgb_mode {
    HID_MODE_OPENRGB = 1,
    HID_MODE_SIGNALRGB,
};

void rgb_raw_hid_receive(uint8_t *data, uint8_t length);
void rgb_raw_hid_send(uint8_t *data, uint8_t length);

extern uint16_t g_auxiliary_rgb_timer;
extern bool g_auxiliary_rgb_anim_playing;

void auxiliary_rgb_set_color(int index, uint8_t red, uint8_t green, uint8_t blue); //set one led color
void auxiliary_rgb_set_mode(int mode);                                             //set hidrgb software mode, HID_MODE_OPENRGB or HID_MODE_SIGNALRGB
uint8_t auxiliary_rgb_get_mode();                                                  //get software mode

void auxiliary_rgb_reload_openrgb_colors(); //reload openrgb colors
void auxiliary_rgb_reload_openrgb_anim();   //reload openrgb colors in animation
RGB *auxiliary_rgb_get_openrgb_colors();    //get openrgb colors
RGB auxiliary_rgb_get_openrgb_color(int index);