// Copyright 2023 alin elena <https://github.com/alinelena> <https://gitlab.com/drFaustroll>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"
#include "extra_keycode.h"


#define LAYOUT_ortho_4x2(k0A, k0B, k1A, k1B, k2A, k2B, k3A, k3B ) {\
	 {k0A, k0B}, \
	 {k1A, k1B}, \
	 {k2A, k2B}, \
	 {k3A, k3B}, \
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

// let us assume we start with both layers off
static bool toggle_lwr = false;
static bool toggle_rse = false;

#ifdef OLED_ENABLE
void user_oled_magic(void);
void render_logo(void);
void clear_screen(void);
void init_timer(void);
#endif

#ifdef RGBLIGHT_ENABLE
void set_rgb_layers(layer_state_t);
const rgblight_segment_t * const*  my_rgb(void);
void set_default_rgb_layers(layer_state_t);
#endif


void toggle_leds(void);
void set_led_toggle(const uint8_t, const bool);

static inline void init_lwr_rse_led(void) {
#ifdef LED_LWR_PIN
    setPinOutput(LED_LWR_PIN);
    writePin(LED_LWR_PIN, toggle_lwr);
    wait_ms(30);
#endif

#ifdef LED_RSE_PIN
    setPinOutput(LED_RSE_PIN);
    writePin(LED_RSE_PIN, toggle_rse);
    wait_ms(30);
#endif
}

static inline void led_lwr(const bool on) {
#ifdef LED_LWR_PIN
    writePin(LED_LWR_PIN, !on);
#endif
}

static inline void led_rse(const bool on) {
#ifdef LED_RSE_PIN
    writePin(LED_RSE_PIN, !on);
#endif
}
static inline void led_caps(const bool on) {
#ifdef LED_CAPS_LOCK_PIN
    writePin(LED_CAPS_LOCK_PIN, !on);
#endif
}
