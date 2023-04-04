// Copyright 2023 alin elena <https://github.com/alinelena> <https://gitlab.com/drFaustroll>
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// let us assume we start with both layers off
static bool toggle_lwr = false;
static bool toggle_rse = false;

#ifdef RGBLIGHT_ENABLE

const rgblight_segment_t PROGMEM my_qwerty_layer[] = RGBLIGHT_LAYER_SEGMENTS({0, RGBLED_NUM, HSV_PURPLE});
const rgblight_segment_t PROGMEM my_lwr_layer[]    = RGBLIGHT_LAYER_SEGMENTS({0, RGBLED_NUM, HSV_CYAN});
const rgblight_segment_t PROGMEM my_rse_layer[]    = RGBLIGHT_LAYER_SEGMENTS({0, RGBLED_NUM, HSV_RED});
const rgblight_segment_t PROGMEM my_adj_layer[]    = RGBLIGHT_LAYER_SEGMENTS({0, RGBLED_NUM, HSV_GREEN});

const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(my_qwerty_layer, my_lwr_layer, my_rse_layer, my_adj_layer);

#endif

#ifdef OLED_ENABLE

  static uint32_t oled_logo_timer = 0;
  static bool clear_logo = true;
  static const char PROGMEM m65_logo[] = {
    0x92, 0x92, 0x93, 0x94, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90,
    0x92, 0x92, 0x93, 0x94, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0,
    0xB2, 0x92, 0xB3, 0xB4, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0,
    0};

#endif


const uint32_t PROGMEM unicode_map[] = {
    [la]  = 0x03B1 , // α
    [lA]  = 0x0307  //
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QW] = LAYOUT_ortho_2x2(
       KC_2    , KC_4    ,
      TT(_LWR),  TT(_RSE)),

  [_LWR] = LAYOUT_ortho_2x2(
       XP(la,lA), KC_CAPS,
       _______,  _______ ),

  [_RSE] = LAYOUT_ortho_2x2(
      KC_5 , KC_6  ,
      _______, _______),

  [_ADJ] = LAYOUT_ortho_2x2(
      RGB_TOG, QK_BOOT  ,
       _______ , _______),

};
// clang-format on

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_QW]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_LWR] = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI) },
    [_RSE] = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI) },
    [_ADJ] = { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD) },
};
#endif

bool led_update_user(led_t led_state) {
    // Disable the default LED update code, so that lock LEDs could be reused to show layer status.
    return false;
}

void matrix_scan_user(void) {
    toggle_leds(toggle_lwr, toggle_rse);
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {

    switch (keycode) {
        case (TT(_LWR)):
            if (!record->event.pressed && record->tap.count == TAPPING_TOGGLE) {
                // This runs before the TT() handler toggles the layer state, so the current layer state is the opposite of the final one after toggle.
                set_led_toggle(_LWR, !layer_state_is(_LWR));
            }
            return true;
            break;
        case (TT(_RSE)):
            if (record->event.pressed && record->tap.count == TAPPING_TOGGLE) {
                set_led_toggle(_RSE, !layer_state_is(_RSE));
            }
            return true;
            break;
        default:
            return true;
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {

#ifdef RGBLIGHT_ENABLE

   set_rgb_layers(state);

#endif

    return update_tri_layer_state(state, _LWR, _RSE, _ADJ);
}

#ifdef RGBLIGHT_ENABLE

layer_state_t default_layer_state_set_user(layer_state_t state) {

    set_default_rgb_layers(state);
    return state;
}

#endif

 #ifdef RGBLIGHT_ENABLE

  void set_rgb_layers(layer_state_t state){

      rgblight_set_layer_state(0, layer_state_cmp(state, _QW));
      rgblight_set_layer_state(1, layer_state_cmp(state, _LWR));
      rgblight_set_layer_state(2, layer_state_cmp(state, _RSE));
      rgblight_set_layer_state(3, layer_state_cmp(state, _ADJ));

  }

  void set_default_rgb_layers(layer_state_t state){
      rgblight_set_layer_state(0, layer_state_cmp(state, _QW));
  }

  const rgblight_segment_t * const* my_rgb(void){
      return my_rgb_layers;
  }

  #endif


void set_led_toggle(const uint8_t layer, const bool state){

      switch (layer) {
          case _LWR:
            toggle_lwr = state;
            break;
          case _RSE:
            toggle_rse = state;
            break;
          default:
            break;
      }
  }

#ifdef OLED_ENABLE

  void init_timer(void){
    oled_logo_timer = timer_read32();
  };

  void user_oled_magic(void) {
      // Host Keyboard Layer Status
      oled_write_P(PSTR("Layer: "), false);

      switch (get_highest_layer(layer_state)) {
          case _QW:
              oled_write_P(PSTR("Default\n"), false);
              break;
          case _LWR:
              oled_write_P(PSTR("Lower\n"), false);
              break;
          case _RSE:
              oled_write_P(PSTR("Raise\n"), false);
              break;
          case _ADJ:
              oled_write_P(PSTR("ADJ\n"), false);
              break;
          default:
              // Or use the write_ln shortcut over adding '\n' to the end of your string
              oled_write_ln_P(PSTR("Undefined"), false);
      }

      // Host Keyboard LED Status
      led_t led_state = host_keyboard_led_state();
      oled_write_P(led_state.num_lock ? PSTR("Lower ") : PSTR("    "), false);
      oled_write_P(led_state.scroll_lock ? PSTR("Raise ") : PSTR("    "), false);
      oled_write_P(led_state.caps_lock ? PSTR("CapsLock ") : PSTR("    "), false);
#    ifdef UNICODE_COMMON_ENABLE
    oled_write_P(PSTR("\nunicode: "), false);
    switch (get_unicode_input_mode()) {
      case UNICODE_MODE_LINUX:
        oled_write_P(PSTR("Linux"), false);
        break;
      case UNICODE_MODE_MACOS:
        oled_write_P(PSTR("apple"), false);
        break;
      case UNICODE_MODE_WINDOWS:
        oled_write_P(PSTR("windows"), false);
        break;
      case UNICODE_MODE_WINCOMPOSE:
        oled_write_P(PSTR("windows c"), false);
        break;
      case UNICODE_MODE_BSD:
        oled_write_P(PSTR("bsd"), false);
        break;
      case UNICODE_MODE_EMACS:
        oled_write_P(PSTR("emacs"), false);
        break;
      default:
        oled_write_ln_P(PSTR("not supported"), false);
    }
#    endif

  #ifdef WPM_ENABLE
      oled_write_P(PSTR("\nwpm: "), false);
      uint8_t wpm = get_current_wpm();
      oled_write_P(wpm != 0 ? get_u8_str(wpm,' ') : PSTR("   "), false);
  #endif
  }

  void render_logo(void) {
      oled_write_P(m65_logo, false);
  }

  void clear_screen(void) {
      if (clear_logo){
        for (uint8_t i = 0; i < OLED_DISPLAY_HEIGHT; ++i) {
          for (uint8_t j = 0; j < OLED_DISPLAY_WIDTH; ++j) {
            oled_write_raw_byte(0x0, i*OLED_DISPLAY_WIDTH + j);
          }
        }
        clear_logo = false;
      }
  }

  oled_rotation_t oled_init_user(oled_rotation_t rotation) {
      return OLED_ROTATION_180;
  }

  #    define SHOW_LOGO 5000
  bool oled_task_kb(void) {
      if (!oled_task_user()) { return false; }
      if ((timer_elapsed32(oled_logo_timer) < SHOW_LOGO)){
          render_logo();
      }else{
        clear_screen();
        user_oled_magic();
      }
      return false;
  }

  #endif


void keyboard_post_init_user(void) {
#ifdef RGBLIGHT_ENABLE
    init_rgb_layers();
#endif

#ifdef OLED_ENABLE
    init_timer();
#endif
}
