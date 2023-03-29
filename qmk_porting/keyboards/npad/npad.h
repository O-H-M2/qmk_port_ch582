// Copyright 2023 Y&R-Biu (@jiaxin96)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"
#include "extra_keycode.h"

#define LAYOUT_all(\
    K00, K01, K02, K03,\
    K10, K11, K12, K13,\
    K20, K21, K22, K23,\
    K30, K31, K32,\
         K41, K42\
) {\
    { K00,   K01, K02, K03   },\
    { K10,   K11, K12, K13   },\
    { K20,   K21, K22, K23   },\
    { K30,   K31, K32, KC_NO },\
    { KC_NO, K41, K42, KC_NO }\
}

