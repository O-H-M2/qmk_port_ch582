/*
Copyright 2022 Huckies <https://github.com/Huckies>
Copyright 2022 puterjam<puterjam@gmail.com>

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

#ifdef RGB_RAW_ENABLE
RGB_MATRIX_EFFECT(AUXILIARY_RGB)
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

// #if !defined(AUXILIARY_RGB_WELCOME_ANIM_SPD)
// #define AUXILIARY_RGB_WELCOME_ANIM_SPD 255
// #endif

static inline void set_openrgb_color(uint8_t led_index, uint8_t val)
{
    extern RGB auxiliary_rgb_get_color_buffer_element(int index);

    RGB rgb = auxiliary_rgb_get_color_buffer_element(led_index);

#ifdef AUXILIARY_RGB_USE_UNIVERSAL_BRIGHTNESS
    float brightness = (float)val / UINT8_MAX;

    rgb_matrix_set_color(led_index, rgb.r * brightness, rgb.g * brightness, rgb.b * brightness);
#else
    rgb_matrix_set_color(led_index, rgb.r, rgb.g, rgb.b);
#endif
}

bool AUXILIARY_RGB(effect_params_t *params)
{
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    // // if (auxiliary_rgb_get_mode() == HID_MODE_OPENRGB) {
    // extern uint16_t g_auxiliary_rgb_timer;
    // extern bool g_auxiliary_rgb_anim_playing;
    // uint8_t time = scale16by8(g_auxiliary_rgb_timer, AUXILIARY_RGB_WELCOME_ANIM_SPD);

    HSV hsv = rgb_matrix_config.hsv;
    // g_auxiliary_rgb_anim_playing = (time != UINT8_MAX);

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        //     if (g_auxiliary_rgb_anim_playing) { // play loop loading animation
        //         int16_t dx = g_led_config.point[i].x - k_rgb_matrix_center.x;
        //         int16_t dy = g_led_config.point[i].y - k_rgb_matrix_center.y;
        //         hsv.v = 0;

        //         int16_t effect = time - sqrt16(dx * dx + dy * dy);
        //         if (effect < 0)
        //             effect = 0;

        //         hsv.v = qadd8(hsv.v, effect);
        //         hsv.v = scale8(hsv.v, rgb_matrix_config.hsv.v);
        //     }

        set_openrgb_color(i, hsv.v);
    }

    // if (g_auxiliary_rgb_anim_playing)
    //     g_auxiliary_rgb_timer++; // tick counter
    // // }

    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // AUXILIARY_RGB_ENABLE