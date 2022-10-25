/* Copyright 2019 Ryota Goto

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
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_all(/* 0: qwerty */
                     KC_NUM, KC_KP_SLASH, KC_KP_ASTERISK, KC_KP_MINUS,
                     KC_KP_7, KC_KP_8, KC_KP_9,
                     KC_KP_4, KC_KP_5, KC_KP_6, KC_KP_PLUS,
                     KC_KP_1, KC_KP_2, KC_KP_3,
                     KC_KP_0, LT(1, KC_KP_DOT), KC_KP_ENTER),
    [1] = LAYOUT_all(/* 1: fn */
                     RGB_TOG, RGB_VAI, RGB_VAD, RGB_MODE_REVERSE,
                     _______, _______, _______,
                     BLE_SLOT3, _______, _______, RGB_MODE_FORWARD,
                     BLE_SLOT0, BLE_SLOT1, BLE_SLOT2,
                     BLE_ALL_CLEAR, _______, _______),
    [2] = LAYOUT_all(/* 2: fn */
                     _______, _______, _______, _______,
                     _______, _______, _______,
                     _______, _______, _______, _______,
                     _______, _______, _______,
                     _______, _______, _______),
    [3] = LAYOUT_all(/* 3: fn */
                     _______, _______, _______, _______,
                     _______, _______, _______,
                     _______, _______, _______, _______,
                     _______, _______, _______,
                     _______, _______, _______),

};

#ifdef ENCODER_MAP_ENABLE

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [2] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [3] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
};

#endif