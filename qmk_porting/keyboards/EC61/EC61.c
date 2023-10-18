/*
Copyright 2022 OctopusZ <https://github.com/OctopusZ>

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

#ifdef RGB_MATRIX_ENABLE

#ifdef AW20216
const aw_led PROGMEM g_aw_leds[DRIVER_LED_TOTAL] = {

    { 0, CS1_SW12, CS3_SW12, CS2_SW12 },
    { 0, CS1_SW11, CS3_SW11, CS2_SW11 },
    { 0, CS1_SW10, CS3_SW10, CS2_SW10 },
    { 0, CS1_SW9, CS3_SW9, CS2_SW9 },
    { 0, CS1_SW8, CS3_SW8, CS2_SW8 },
    { 0, CS1_SW7, CS3_SW7, CS2_SW7 },
    { 0, CS1_SW6, CS3_SW6, CS2_SW6 },
    { 0, CS1_SW5, CS3_SW5, CS2_SW5 },
    { 0, CS1_SW4, CS3_SW4, CS2_SW4 },
    { 0, CS1_SW3, CS3_SW3, CS2_SW3 },
    { 0, CS1_SW2, CS3_SW2, CS2_SW2 },
    { 0, CS1_SW1, CS3_SW1, CS2_SW1 },

    { 0, CS16_SW1, CS18_SW1, CS17_SW1 },
    { 0, CS16_SW2, CS18_SW2, CS17_SW2 },

    { 0, CS4_SW12, CS6_SW12, CS5_SW12 },
    { 0, CS4_SW11, CS6_SW11, CS5_SW11 },
    { 0, CS4_SW10, CS6_SW10, CS5_SW10 },
    { 0, CS4_SW9, CS6_SW9, CS5_SW9 },
    { 0, CS4_SW8, CS6_SW8, CS5_SW8 },
    { 0, CS4_SW7, CS6_SW7, CS5_SW7 },
    { 0, CS4_SW6, CS6_SW6, CS5_SW6 },
    { 0, CS4_SW5, CS6_SW5, CS5_SW5 },
    { 0, CS4_SW4, CS6_SW4, CS5_SW4 },
    { 0, CS4_SW3, CS6_SW3, CS5_SW3 },
    { 0, CS4_SW2, CS6_SW2, CS5_SW2 },
    { 0, CS4_SW1, CS6_SW1, CS5_SW1 },

    { 0, CS16_SW4, CS18_SW4, CS17_SW4 },
    { 0, CS16_SW3, CS18_SW3, CS17_SW3 },

    { 0, CS7_SW12, CS9_SW12, CS8_SW12 },
    { 0, CS7_SW11, CS9_SW11, CS8_SW11 },
    { 0, CS7_SW10, CS9_SW10, CS8_SW10 },
    { 0, CS7_SW9, CS9_SW9, CS8_SW9 },
    { 0, CS7_SW8, CS9_SW8, CS8_SW8 },
    { 0, CS7_SW7, CS9_SW7, CS8_SW7 },
    { 0, CS7_SW6, CS9_SW6, CS8_SW6 },
    { 0, CS7_SW5, CS9_SW5, CS8_SW5 },
    { 0, CS7_SW4, CS9_SW4, CS8_SW4 },
    { 0, CS7_SW3, CS9_SW3, CS8_SW3 },
    { 0, CS7_SW2, CS9_SW2, CS8_SW2 },
    { 0, CS7_SW1, CS9_SW1, CS8_SW1 },

    { 0, CS16_SW5, CS18_SW5, CS17_SW5 },

    { 0, CS10_SW12, CS12_SW12, CS11_SW12 },
    { 0, CS10_SW11, CS12_SW11, CS11_SW11 },
    { 0, CS10_SW10, CS12_SW10, CS11_SW10 },
    { 0, CS10_SW9, CS12_SW9, CS11_SW9 },
    { 0, CS10_SW8, CS12_SW8, CS11_SW8 },
    { 0, CS10_SW7, CS12_SW7, CS11_SW7 },
    { 0, CS10_SW6, CS12_SW6, CS11_SW6 },
    { 0, CS10_SW5, CS12_SW5, CS11_SW5 },
    { 0, CS10_SW4, CS12_SW4, CS11_SW4 },
    { 0, CS10_SW3, CS12_SW3, CS11_SW3 },
    { 0, CS10_SW2, CS12_SW2, CS11_SW2 },
    { 0, CS10_SW1, CS12_SW1, CS11_SW1 },

    { 0, CS13_SW12, CS15_SW12, CS14_SW12 },
    { 0, CS13_SW11, CS15_SW11, CS14_SW11 },
    { 0, CS13_SW10, CS15_SW10, CS14_SW10 },

    { 0, CS13_SW7, CS15_SW7, CS14_SW7 },

    { 0, CS13_SW4, CS15_SW4, CS14_SW4 },
    { 0, CS13_SW3, CS15_SW3, CS14_SW3 },
    { 0, CS13_SW2, CS15_SW2, CS14_SW2 },
    { 0, CS13_SW1, CS15_SW1, CS14_SW1 },

};

void spi_master_pre_transmit_cb()
{
    aw20216_power_check();
}

#endif

#ifdef RGB_RAW_ENABLE
const LampAttributes m_lampAttributes[] = {
    { .LampId = 0, .PositionXInMillimeters = 9, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_ESC },
    { .LampId = 1, .PositionXInMillimeters = 28, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_1 },
    { .LampId = 2, .PositionXInMillimeters = 47, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_2 },
    { .LampId = 3, .PositionXInMillimeters = 66, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_3 },
    { .LampId = 4, .PositionXInMillimeters = 85, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_4 },
    { .LampId = 5, .PositionXInMillimeters = 104, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_5 },
    { .LampId = 6, .PositionXInMillimeters = 123, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_6 },
    { .LampId = 7, .PositionXInMillimeters = 142, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_7 },
    { .LampId = 8, .PositionXInMillimeters = 161, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_8 },
    { .LampId = 9, .PositionXInMillimeters = 180, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_9 },
    { .LampId = 10, .PositionXInMillimeters = 199, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_0 },
    { .LampId = 11, .PositionXInMillimeters = 218, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_MINS },
    { .LampId = 12, .PositionXInMillimeters = 237, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_EQL },
    { .LampId = 13, .PositionXInMillimeters = 266, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_BSPC },
    { .LampId = 14, .PositionXInMillimeters = 14, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_TAB },
    { .LampId = 15, .PositionXInMillimeters = 38, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_Q },
    { .LampId = 16, .PositionXInMillimeters = 57, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_W },
    { .LampId = 17, .PositionXInMillimeters = 76, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_E },
    { .LampId = 18, .PositionXInMillimeters = 95, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_R },
    { .LampId = 19, .PositionXInMillimeters = 114, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_T },
    { .LampId = 20, .PositionXInMillimeters = 133, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_Y },
    { .LampId = 21, .PositionXInMillimeters = 152, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_U },
    { .LampId = 22, .PositionXInMillimeters = 171, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_I },
    { .LampId = 23, .PositionXInMillimeters = 190, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_O },
    { .LampId = 24, .PositionXInMillimeters = 209, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_P },
    { .LampId = 25, .PositionXInMillimeters = 228, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_LBRC },
    { .LampId = 26, .PositionXInMillimeters = 247, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_RBRC },
    { .LampId = 27, .PositionXInMillimeters = 271, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_BSLS },
    { .LampId = 28, .PositionXInMillimeters = 16, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_CAPS },
    { .LampId = 29, .PositionXInMillimeters = 42, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_A },
    { .LampId = 30, .PositionXInMillimeters = 61, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_S },
    { .LampId = 31, .PositionXInMillimeters = 80, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_D },
    { .LampId = 32, .PositionXInMillimeters = 99, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_F },
    { .LampId = 33, .PositionXInMillimeters = 118, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_G },
    { .LampId = 34, .PositionXInMillimeters = 137, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_H },
    { .LampId = 35, .PositionXInMillimeters = 156, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_J },
    { .LampId = 36, .PositionXInMillimeters = 175, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_K },
    { .LampId = 37, .PositionXInMillimeters = 194, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_L },
    { .LampId = 38, .PositionXInMillimeters = 213, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_SCLN },
    { .LampId = 39, .PositionXInMillimeters = 232, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_QUOT },
    { .LampId = 40, .PositionXInMillimeters = 264, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_ENT },
    { .LampId = 41, .PositionXInMillimeters = 21, .PositionYInMillimeters = 66, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_LSFT },
    { .LampId = 42, .PositionXInMillimeters = 52, .PositionYInMillimeters = 66, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_Z },
    { .LampId = 43, .PositionXInMillimeters = 71, .PositionYInMillimeters = 66, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_X },
    { .LampId = 44, .PositionXInMillimeters = 90, .PositionYInMillimeters = 66, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_C },
    { .LampId = 45, .PositionXInMillimeters = 109, .PositionYInMillimeters = 66, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_V },
    { .LampId = 46, .PositionXInMillimeters = 128, .PositionYInMillimeters = 66, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_B },
    { .LampId = 47, .PositionXInMillimeters = 147, .PositionYInMillimeters = 66, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_N },
    { .LampId = 48, .PositionXInMillimeters = 166, .PositionYInMillimeters = 66, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_M },
    { .LampId = 49, .PositionXInMillimeters = 185, .PositionYInMillimeters = 66, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_COMM },
    { .LampId = 50, .PositionXInMillimeters = 204, .PositionYInMillimeters = 66, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_DOT },
    { .LampId = 51, .PositionXInMillimeters = 223, .PositionYInMillimeters = 66, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_SLSH },
    { .LampId = 52, .PositionXInMillimeters = 259, .PositionYInMillimeters = 66, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_RSFT },
    { .LampId = 53, .PositionXInMillimeters = 11, .PositionYInMillimeters = 85, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_LCTL },
    { .LampId = 54, .PositionXInMillimeters = 35, .PositionYInMillimeters = 85, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_LGUI },
    { .LampId = 55, .PositionXInMillimeters = 59, .PositionYInMillimeters = 85, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_LALT },
    { .LampId = 56, .PositionXInMillimeters = 130, .PositionYInMillimeters = 85, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_SPC },
    { .LampId = 57, .PositionXInMillimeters = 202, .PositionYInMillimeters = 85, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_RALT },
    { .LampId = 58, .PositionXInMillimeters = 226, .PositionYInMillimeters = 85, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_RGUI },
    { .LampId = 59, .PositionXInMillimeters = 250, .PositionYInMillimeters = 85, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_MENU },
    { .LampId = 60, .PositionXInMillimeters = 274, .PositionYInMillimeters = 85, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .RedLevelCount = UINT8_MAX, .GreenLevelCount = UINT8_MAX, .BlueLevelCount = UINT8_MAX, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_RCTL },
};
#endif

/* clang-format off */
led_config_t g_led_config = {
	{
        {     0,       1,       2,       3,       4,        5,        6,        7,        8,        9,       10,       11,       12,      13}, 
        {    14,      15,      16,      17,      18,       19,       20,       21,       22,       23,       24,       25,       26,      27}, 
        {    28,      29,      30,      31,      32,       33,       34,       35,       36,       37,       38,       39,       40,  NO_LED}, 
        {    41,      42,      43,      44,      45,       46,       47,       48,       49,       50,       51,   NO_LED,       52,  NO_LED}, 
        {    53,      54,      55,  NO_LED,  NO_LED,       56,   NO_LED,   NO_LED,   NO_LED,       57,       58,       59,       60,  NO_LED}, 
	}, {
          {7,3},  {22,3},  {37,3},  {52,3},  {67,3},   {82,3},   {97,3},  {112,3},  {127,3},  {142,3},  {157,3},  {172,3},  {187,3},  {209,3},
        {11,16}, {30,16}, {45,16}, {60,16}, {75,16},  {90,16}, {105,16}, {119,16}, {134,16}, {149,16}, {164,16}, {179,16}, {194,16}, {213,16},
        {13,29}, {34,29}, {49,29}, {63,29}, {78,29},  {93,29}, {108,29}, {123,29}, {138,29}, {153,29}, {168,29}, {183,29}, {207,29},
        {17,41}, {41,41}, {56,41}, {71,41}, {86,41}, {101,41}, {116,41}, {131,41}, {146,41}, {161,41}, {175,41},           {203,41},
         {9,54}, {28,54}, {47,54},                   {103,54},                                         {159,54}, {177,54}, {196,54}, {215,54},
	}, {
		      4,       4,       4,       4,       4,        4,        4,        4,        4,        4,        4,        4,        4,        4,
		      4,       4,       4,       4,       4,        4,        4,        4,        4,        4,        4,        4,        4,        4,
		      4,       4,       4,       4,       4,        4,        4,        4,        4,        4,        4,        4,        4, 
		      4,       4,       4,       4,       4,        4,        4,        4,        4,        4,        4,                  4,
		      4,       4,       4,                          4,                                      4,        4,        4,        4,
	}
};
/* clang-format on */

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max)
{
    if (led_min <= 28 && led_max > 28 && host_keyboard_led_state().caps_lock) {
        RGB_MATRIX_INDICATOR_SET_COLOR(28, 0xFF, 0x00, 0x00);
    }

    return true;
}

#endif

int main()
{
    extern void protocol_setup();
    extern void protocol_init();
    extern void protocol_task();

    platform_setup();

    protocol_setup();
#if !defined ESB_ENABLE || ESB_ENABLE != 2
    keyboard_setup();
#endif

    protocol_init();

    /* Main loop */
    for (;;) {
        protocol_task();
        //! housekeeping_task() is handled by platform
    }
}
