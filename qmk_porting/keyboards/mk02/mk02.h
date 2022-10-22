#pragma once

#include "quantum.h"
#include "extra_keycode.h"

// clang-format off
#define LAYOUT_all(         \
	K000, K001, K002, K003, \
	K100, K101, K102,       \
	K200, K201, K202, K203, \
	K300, K301, K302,       \
	K400, K401, K402        \
) \
{ \
    { K000, K001, K002, K003 }, \
    { K100, K101, K102 },       \
    { K200, K201, K202, K203 }, \
    { K300, K301, K302       }, \
    { K400, KC_NO, K401, K402 } \
}
// clang-format on
