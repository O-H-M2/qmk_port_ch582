// Copyright 2023 alin elena <https://github.com/alinelena> <https://gitlab.com/drFaustroll>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"
#include "extra_keycode.h"

#define LAYOUT_ortho_4x2(k0A, k0B, k1A, k1B, k2A, k2B, k3A, k3B) \
    {                                                            \
        { k0A, k0B },                                            \
            { k1A, k1B },                                        \
            { k2A, k2B },                                        \
            { k3A, k3B },                                        \
    }

enum unicode_names {
    la = NEW_SAFE_RANGE,
    lA,
    lb,
    lB,
    rc,
    rC,
    rv,
    rV,
    rb,
    rB,
    rn,
    rN
};

enum layer_names {
    _QW = 0,
    _LWR,
    _RSE,
    _ADJ
};

#ifdef RGBLIGHT_ENABLE
void set_rgb_layers(layer_state_t);
const rgblight_segment_t *const *my_rgb(void);
void set_default_rgb_layers(layer_state_t);
#endif

static inline void led_caps(const bool on)
{
#ifdef LED_CAPS_LOCK_PIN
    writePin(LED_CAPS_LOCK_PIN, !on);
#endif
}
