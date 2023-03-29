// Copyright 2023 Y&R-Biu (@jiaxin96)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"
#include "extra_keycode.h"

#define LAYOUT_all(\
    k00, k01, k02, k03,\
    k10, k11, k12, K13,\
    k20, k21, k22, k23,\
    k30, k31, k32,\
         k41, k42\
) {\
    { k00,   k01, k02, k03   },\
    { k10,   k11, k12, K13   },\
    { k20,   k21, k22, k23   },\
    { k30,   k31, k32, KC_NO },\
    { KC_NO, K41, k42, KC_NO }\
}

