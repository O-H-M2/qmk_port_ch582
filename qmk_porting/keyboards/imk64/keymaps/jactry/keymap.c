/*
Copyright 2023 SharpSnail01 <https://github.com/SharpSnail01>
Copyright 2023 Jactry Zeng

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


/* Tap dance configuration. */
enum {
    TD_LSFT_CAPS,
};

tap_dance_action_t tap_dance_actions[] = {
    [TD_LSFT_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_LSFT, KC_CAPS),
};

/* Some customize keys. */
#define CK_LSFT TD(TD_LSFT_CAPS)

enum custom_keycodes {
    CK_OSMD = NEW_SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /*
   * ,-----------------------------------------------------------.
   * |Esc~| 1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|Backsp |
   * |-----------------------------------------------------------|
   * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|  \  |
   * |-----------------------------------------------------------|
   * |Ctrl  |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|Return  |
   * |-----------------------------------------------------------|
   * |Shift  |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /| PU|  ↑| PD|
   * |-----------------------------------------------------------|
   * |Ctrl|Gui |Alt |         Space          |MO2|MO1|  ←|  ↓|  →|
   * `-----------------------------------------------------------'
   *
   */
    [0] = LAYOUT_all(/* 0: qwerty */
        QK_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
        KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
        CK_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_PGUP, KC_UP,   KC_PGDN,
        KC_LCTL, KC_LGUI, KC_LALT, KC_SPC,  MO(2),   MO(1),   KC_LEFT, KC_DOWN, KC_RGHT),
    [1] = LAYOUT_all(/* 1: fn */
        KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, KC_VOLD, KC_VOLU, KC_HOME, KC_INS,  KC_END,
        _______, _______, _______, _______, _______, _______, _______, _______, _______),
    [2] = LAYOUT_all(/* 2: fn */
        _______, KC_BRID, KC_BRIU, KC_MCTL, KC_LPAD, RGB_VAD, RGB_VAI, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, KC_PSCR,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, CK_OSMD, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______),
    [3] = LAYOUT_all(/* 3: fn */
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______),
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [2] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [3] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
};
#endif

enum OS_MODE {
    FIRST_MODE = 0,
    LINUX_MODE = FIRST_MODE,
    MAC_MODE,
    WIN_MODE,
    LAST_MODE = WIN_MODE,
};

/* User settings structure for the EEPROM. */
typedef union {
    uint32_t raw;
    struct {
        uint8_t os_mode : 8;
    };
} user_config_t;

user_config_t user_config;

static int os_mode = FIRST_MODE;
static uint16_t ck_left_gui = KC_NO;
static uint16_t ck_left_alt = KC_NO;
static uint16_t ck_right_gui = KC_NO;
static uint16_t ck_right_alt = KC_NO;
static int os_mode_led_cycle = 0;
static bool user_rgb_state = FALSE;
static bool override_atl_esc = FALSE;

static void set_os_mode(int os_mode) {
    switch (os_mode) {
        case MAC_MODE:
            ck_left_gui = KC_LOPT;
            ck_left_alt = KC_LCMD;
            ck_right_gui = KC_ROPT;
            ck_right_alt = KC_RCMD;
            override_atl_esc = FALSE;
            break;
        case WIN_MODE:
            ck_left_gui = KC_LWIN;
            ck_left_alt = KC_LALT;
            ck_right_gui = KC_RWIN;
            ck_right_alt = KC_RALT;
            override_atl_esc = FALSE;
            break;
        case LINUX_MODE:
        default:
            ck_left_gui = KC_LGUI;
            ck_left_alt = KC_LALT;
            ck_right_gui = KC_RGUI;
            ck_right_alt = KC_RALT;
            override_atl_esc = TRUE;
            break;
    }
    if (!os_mode_led_cycle)  {
        rgb_matrix_set_color_all(RGB_BLACK);
        user_rgb_state = rgb_matrix_is_enabled();
    }
    rgb_matrix_enable_noeeprom();
    os_mode_led_cycle = 500;
    user_config.os_mode = os_mode;
    eeconfig_update_user(user_config.raw);
}

void keyboard_post_init_user(void) {
    user_config.raw = eeconfig_read_user();
    os_mode = user_config.os_mode;
    set_os_mode(os_mode);
}

void eeconfig_init_user(void) {
    os_mode = FIRST_MODE;
    user_config.raw = 0;
    set_os_mode(os_mode);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case CK_OSMD:
            if (++os_mode > LAST_MODE) {
                os_mode = FIRST_MODE;
            }
            set_os_mode(os_mode);
            return false;
        case KC_LALT:
            if (record->event.pressed) {
                register_code(ck_left_alt);
            }
            else {
                unregister_code(ck_left_alt);
            }
            return false;
        case KC_LGUI:
            if (record->event.pressed) {
                register_code(ck_left_gui);
            }
            else {
                unregister_code(ck_left_gui);
            }
            return false;
        case KC_RALT:
            if (record->event.pressed) {
                register_code(ck_right_alt);
            }
            else {
                unregister_code(ck_right_alt);
            }
            return false;
        case KC_RGUI:
            if (record->event.pressed) {
                register_code(ck_right_gui);
            }
            else {
                unregister_code(ck_right_gui);
            }
            return false;
        case QK_GESC:
            if (override_atl_esc && get_mods() == MOD_BIT(KC_LALT)) {
                if (record->event.pressed) {
                    register_code(KC_GRV);
                } else {
                    unregister_code(KC_GRV);
                }
                return false;
            }
            return true;
    }
    return true;
}

bool rgb_matrix_indicators_user() {
    if (os_mode_led_cycle) {
        os_mode_led_cycle--;
        rgb_matrix_set_color_all(RGB_BLACK);
        switch (os_mode) {
            case MAC_MODE:
                rgb_matrix_set_color(18, RGB_WHITE);
                break;
            case WIN_MODE:
                rgb_matrix_set_color(18, RGB_BLUE);
                break;
            case LINUX_MODE:
                rgb_matrix_set_color(18, RGB_YELLOW);
                break;
        }
        if (os_mode_led_cycle  == 1) {
            if (user_rgb_state) {
                rgb_matrix_reload_from_eeprom();
            }
            else {
                rgb_matrix_disable_noeeprom();
            }
        }
    }
    return false;
}
