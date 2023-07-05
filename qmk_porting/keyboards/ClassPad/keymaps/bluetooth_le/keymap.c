/*
Copyright 2023 OctopusZ <https://github.com/OctopusZ>

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
    // clang-format off
    [0] = LAYOUT_all(/* 0: qwerty */
                     KC_NUM, KC_KP_SLASH, KC_KP_ASTERISK, KC_KP_MINUS, KC_DEL, KC_HOME, MO(1),
                     KC_KP_7, KC_KP_8   , KC_KP_9,                     LCTL(KC_C),  KC_HOME,   LCTL(KC_V),
                     KC_KP_4, KC_KP_5   , KC_KP_6,        KC_KP_PLUS,  EE_CLR, KC_MUTE,KC_MUTE,
                     KC_KP_1, KC_KP_2   , KC_KP_3, 
                     KC_KP_0, KC_KP_DOT , KC_PENT),
    [1] = LAYOUT_all(/* 1: fn */
                     _______, _______, _______, _______,     _______, MO(2),   _______,
                     _______, _______, _______,              _______, _______, _______,
                     _______, _______, _______, _______,     _______, _______, _______,
                     _______, _______, _______,
                     _______,          _______, _______),
    [2] = LAYOUT_all(/* 2: fn */
                    USB_MODE,BLE_SLOT0,BLE_SLOT1,BLE_SLOT2,  _______, _______, _______,
                     _______, _______, _______,              _______, _______, _______,
                     _______, _______, _______, _______,     _______, _______, _______,
                     _______, _______, _______,
                     BLE_ALL_CLEAR,          _______, _______),
    [3] = LAYOUT_all(/* 2: fn */
                     _______, _______, _______, _______,     _______, _______, _______,
                     _______, _______, _______,              _______, _______, _______,
                     _______, _______, _______, _______,     _______, _______, _______,
                     _______, _______, _______,
                     _______,          _______, _______),
    [4] = LAYOUT_all(/* 2: fn */
                     _______, _______, _______, _______,     _______, _______, _______,
                     _______, _______, _______,              _______, _______, _______,
                     _______, _______, _______, _______,     _______, _______, _______,
                     _______, _______, _______,
                     _______,          _______, _______),
    [5] = LAYOUT_all(/* 2: fn */
                     _______, _______, _______, _______,     _______, _______, _______,
                     _______, _______, _______,              _______, _______, _______,
                     _______, _______, _______, _______,     _______, _______, _______,
                     _______, _______, _______,
                     _______,          _______, _______),
    [6] = LAYOUT_all(/* 2: fn */
                     _______, _______, _______, _______,     _______, _______, _______,
                     _______, _______, _______,              _______, _______, _______,
                     _______, _______, _______, _______,     _______, _______, _______,
                     _______, _______, _______,
                     _______,          _______, _______),
    [7] = LAYOUT_all(/* 2: fn */
                     _______, _______, _______, _______,     _______, _______, _______,
                     _______, _______, _______,              _______, _______, _______,
                     _______, _______, _______, _______,     _______, _______, _______,
                     _______, _______, _______,
                     _______,          _______, _______),
    [8] = LAYOUT_all(/* 2: fn */
                     _______, _______, _______, _______,     _______, _______, _______,
                     _______, _______, _______,              _______, _______, _______,
                     _______, _______, _______, _______,     _______, _______, _______,
                     _______, _______, _______,
                     _______,          _______, _______),
    [9] = LAYOUT_all(/* 2: fn */
                     _______, _______, _______, _______,     _______, _______, _______,
                     _______, _______, _______,              _______, _______, _______,
                     _______, _______, _______, _______,     _______, _______, _______,
                     _______, _______, _______,
                     _______,          _______, _______),
    // clang-format on
};

#ifdef ENCODER_MAP_ENABLE

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [2] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [3] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [4] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [5] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [6] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [7] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [8] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [9] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
};

#endif
