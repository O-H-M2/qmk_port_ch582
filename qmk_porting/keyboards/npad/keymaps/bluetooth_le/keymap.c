// Copyright 2023 Y&R-Biu (@jiaxin96)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_all(
        LT(1, KC_NUM), KC_PSLS, KC_PAST, KC_PMNS,
        KC_P7,         KC_P8,   KC_P9,   KC_PPLS,
        KC_P4,         KC_P5,   KC_P6,   KC_PENT,
        KC_P1,         KC_P2,   KC_P3,
                       KC_P0,   KC_PDOT
    ),
    [1] = LAYOUT_all(
        KC_TRNS,   KC_TRNS,       KC_TRNS,   KC_TRNS,
        KC_TRNS,   KC_TRNS,       KC_TRNS,   KC_TRNS,
        USB_MODE,  BLE_ALL_CLEAR, BLE_SLOT3, RGB_TOG,
        BLE_SLOT0, BLE_SLOT1,     BLE_SLOT2,
                   KC_TRNS,       KC_TRNS
    ),
    [2] = LAYOUT_all(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS,
                 KC_TRNS, KC_TRNS
    ),
    [3] = LAYOUT_all(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS,
                 KC_TRNS, KC_TRNS
    )
};
