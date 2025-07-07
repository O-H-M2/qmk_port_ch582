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

#include QMK_KEYBOARD_H
#include "extra_keycode.h"

enum unicode_names {
    la = NEW_SAFE_RANGE,
    lA,
    rc,
    rC,
    rv,
    rV,
    rb,
    rB,
    rn,
    rN
};

const uint32_t PROGMEM unicode_map[] = {
    [la] = 0x03B1, // α
    [lA] = 0x0307, //
    [rc] = 0x2284, // ⊄
    [rC] = 0x2286, // ⊆
    [rv] = 0x2285, // ⊅
    [rV] = 0x2287, //⊇
    [rb] = 0x2227, // ∧
    [rB] = 0x22BC, //⊼
    [rn] = 0x2228, //∨
    [rN] = 0x22BB  //⊻
};
#define POWER_PIN A13
// 在 keymap.h 或 keymap.c 中定义枚举
enum my_keycodes {
    MY_POWER = SAFE_RANGE, // 自定义键码
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_all(/* 0: qwerty */
                     _______, _______, _______, KC_ESC, KC_TAB, KC_GRV, KC_MINS, KC_EQL, KC_LBRC, KC_RBRC,
                     _______, _______, _______, KC_1, KC_2, KC_3, KC_4, KC_5, KC_BSLS, KC_CAPS,
                     _______, _______, _______, KC_6, KC_7, KC_8, KC_9, KC_0, MS_BTN2, KC_BSPC,
                     KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P,
                     KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_ENT,
                     KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH,
                     KC_LSFT, MO(1), KC_LCTL, KC_LALT, KC_SPC, KC_SCLN, KC_QUOT, KC_RCTL, MO(1), KC_RSFT),
    [1] = LAYOUT_all(/* 1: fn */
                     _______, _______, _______, MY_POWER, KC_TAB, KC_GRV, KC_MINS, KC_EQL, KC_LBRC, KC_RBRC,
                     _______, _______, _______, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F11, KC_CAPS,
                     _______, _______, _______, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F12, KC_BSPC,
                     KC_Q, KC_UP, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P,
                     KC_LEFT, KC_DOWN, KC_RIGHT, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_ENT,
                     KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH,
                     KC_LSFT, _______, KC_LCTL, KC_LALT, KC_SPC, KC_SCLN, KC_QUOT, KC_RCTL, _______, KC_RSFT),
    [2] = LAYOUT_all(/* 2: fn */
                     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),
    [3] = LAYOUT_all(/* 3: fn */
                     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

};

#ifdef ENCODER_MAP_ENABLE

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [2] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [3] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
};

#endif

// Thumbstick keymap, change KC_XXX to whatever you need

#define THUMBSTICK_RIGHT_TAP KC_RIGHT
#define THUMBSTICK_LEFT_TAP  KC_LEFT
#define THUMBSTICK_UP_TAP    KC_UP
#define THUMBSTICK_DOWN_TAP  KC_DOWN

// Thumbstick code, no customisation needed

bool cursor_mode = false;
bool scrolling_mode = false;
bool tapping_mode = false;

// tracks if thumbstick was released
bool returned_to_zero = true;

// tracks how many times mouse_report.x/y have been read zero in succession
uint16_t zero_reads = 0;

#ifndef SCROLLING_LAYER
#define SCROLLING_LAYER 1
#endif
#ifndef TAPPING_LAYER
#define TAPPING_LAYER 2
#endif
#ifndef CURSOR_SPEED
#define CURSOR_SPEED 70
#endif
#ifndef SCROLL_SPEED
#define SCROLL_SPEED 25
#endif
#ifndef SCROLL_DELAY_MS
#define SCROLL_DELAY_MS 70
#endif

// // manipulate mouse report based on current mode
// report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {

//     if (cursor_mode) {
//         mouse_report.x = CURSOR_SPEED * mouse_report.x/100;
//         mouse_report.y = CURSOR_SPEED * mouse_report.y/100;
//     }
//     if (scrolling_mode) {
//         mouse_report.h = SCROLL_SPEED * mouse_report.x/100;
//         mouse_report.v = SCROLL_SPEED * mouse_report.y/100;
//         mouse_report.x = 0;
//         mouse_report.y = 0;
//         if ((mouse_report.h != 0) | (mouse_report.v != 0)) {
//             _delay_ms(SCROLL_DELAY_MS);
//         }

//     } else if (tapping_mode) {
//         if ((mouse_report.x || mouse_report.y) != 0) {
//             if (returned_to_zero) {
//                 if (mouse_report.x > 0) {
//                     tap_code16(THUMBSTICK_RIGHT_TAP);
//                 }
//                 if (mouse_report.x < 0) {
//                     tap_code16(THUMBSTICK_LEFT_TAP);
//                 }
//                 if (mouse_report.y > 0) {
//                     tap_code16(THUMBSTICK_DOWN_TAP);
//                 }
//                 if (mouse_report.y < 0) {
//                     tap_code16(THUMBSTICK_UP_TAP);
//                 }
//                 returned_to_zero = false;
//             }
//             zero_reads = 0;
//         } else if (zero_reads < 20) {
//             zero_reads++;
//         }
//         if (zero_reads >= 20) {
//             if (returned_to_zero == false) {
//                 returned_to_zero = true;
//             }
//         }
//         mouse_report.x = 0;
//         mouse_report.y = 0;
//     }

//     return mouse_report;
// }

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
    if (keycode == MY_POWER) {
        if (record->event.pressed) {
            gpio_write_pin_high(POWER_PIN);
            gpio_set_pin_output(POWER_PIN);
        } else {
            gpio_write_pin_low(POWER_PIN);
            gpio_set_pin_output(POWER_PIN);
        }
    }
}