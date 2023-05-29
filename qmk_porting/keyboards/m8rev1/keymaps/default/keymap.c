// Copyright 2023 alin elena <https://github.com/alinelena> <https://gitlab.com/drFaustroll>
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#ifdef RGBLIGHT_ENABLE

const rgblight_segment_t PROGMEM my_qwerty_layer[] = RGBLIGHT_LAYER_SEGMENTS({ 0, RGBLED_NUM, HSV_PURPLE });
const rgblight_segment_t PROGMEM my_lwr_layer[] = RGBLIGHT_LAYER_SEGMENTS({ 0, RGBLED_NUM, HSV_CYAN });
const rgblight_segment_t PROGMEM my_rse_layer[] = RGBLIGHT_LAYER_SEGMENTS({ 0, RGBLED_NUM, HSV_RED });
const rgblight_segment_t PROGMEM my_adj_layer[] = RGBLIGHT_LAYER_SEGMENTS({ 0, RGBLED_NUM, HSV_GREEN });

const rgblight_segment_t *const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(my_qwerty_layer, my_lwr_layer, my_rse_layer, my_adj_layer);

#endif

const uint32_t PROGMEM unicode_map[] = {
    [la] = 0x03B1, // α
    [lA] = 0x0307, //
    [lb] = 0x03B2, // β
    [lB] = 0x2229, // ∩
    [rc] = 0x2284, // ⊄
    [rC] = 0x2286, // ⊆
    [rv] = 0x2285, // ⊅
    [rV] = 0x2287, //⊇
    [rb] = 0x2227, // ∧
    [rB] = 0x22BC, //⊼
    [rn] = 0x2228, //∨
    [rN] = 0x22BB  //⊻
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QW] = LAYOUT_ortho_4x2(
       KC_1   , KC_2     ,
       KC_3   , KC_4     ,
       KC_5   , KC_6     ,
      TT(_LWR), TT(_RSE)),

  [_LWR] = LAYOUT_ortho_4x2(
       XP(la, lA) , XP(lb, lB),
       XP(rb, rB) , XP(rv, rV),
       XP(rc, rC) , XP(rn, rN),
           _______,  _______ ),

  [_RSE] = LAYOUT_ortho_4x2(
      KC_CAPS, UC_LINX ,
      KC_K   , KC_Z    ,
      KC_L   , KC_W    ,
      _______, _______),

  [_ADJ] = LAYOUT_ortho_4x2(
      RGB_TOG , QK_BOOT ,
      KC_O    , UC_LINX ,
      KC_P    , KC_T    ,
       _______, _______),

};
// clang-format on

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_QW] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_LWR] = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI) },
    [_RSE] = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI) },
    [_ADJ] = { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD) },
};
#endif

bool led_update_user(led_t led_state)
{
    // Disable the default LED update code, so that lock LEDs could be reused to show layer status.
    return false;
}

void matrix_scan_user(void)
{
    led_t led_state = host_keyboard_led_state();
    led_caps(led_state.caps_lock);
}

layer_state_t layer_state_set_user(layer_state_t state)
{
#ifdef RGBLIGHT_ENABLE

    set_rgb_layers(state);

#endif

    return update_tri_layer_state(state, _LWR, _RSE, _ADJ);
}

#ifdef RGBLIGHT_ENABLE

layer_state_t default_layer_state_set_user(layer_state_t state)
{
    set_default_rgb_layers(state);
    return state;
}

#endif

#ifdef RGBLIGHT_ENABLE

void set_rgb_layers(layer_state_t state)
{
    rgblight_set_layer_state(0, layer_state_cmp(state, _QW));
    rgblight_set_layer_state(1, layer_state_cmp(state, _LWR));
    rgblight_set_layer_state(2, layer_state_cmp(state, _RSE));
    rgblight_set_layer_state(3, layer_state_cmp(state, _ADJ));
}

void set_default_rgb_layers(layer_state_t state)
{
    rgblight_set_layer_state(0, layer_state_cmp(state, _QW));
}

const rgblight_segment_t *const *my_rgb(void)
{
    return my_rgb_layers;
}

#endif

void keyboard_post_init_user(void)
{
#ifdef RGBLIGHT_ENABLE
    // Enable the LED layers
    rgblight_layers = my_rgb();
#endif
}
