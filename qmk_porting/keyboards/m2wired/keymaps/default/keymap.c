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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_all(/* 0: qwerty */
                     XP(la, lA), XP(rc, rC), XP(rv, rV), XP(rb, rB), XP(rn, rN), KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A,
                     UC_NEXT, UC_PREV, UC_LINX, UC_WINC, UC_MAC, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A,
                     KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A,
                     KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A,
                     KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A,
                     KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A),
    [1] = LAYOUT_all(/* 1: fn */
                     KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A,
                     KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A,
                     KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A,
                     KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A,
                     KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A,
                     KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A),
    [2] = LAYOUT_all(/* 2: fn */
                     KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A,
                     KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A,
                     KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A,
                     KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A,
                     KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A,
                     KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A),
    [3] = LAYOUT_all(/* 3: fn */
                     KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A,
                     KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A,
                     KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A,
                     KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A,
                     KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A,
                     KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A),

};

#ifdef ENCODER_MAP_ENABLE

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [2] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [3] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
};

#endif
