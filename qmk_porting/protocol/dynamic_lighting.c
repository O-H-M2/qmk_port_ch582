/*
Copyright 2023 Huckies <https://github.com/Huckies>

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

#include "dynamic_lighting.h"
#include "auxiliary_rgb.h"
#include "quantum.h"

static uint16_t m_lastLampIdRequested = 0;
static bool m_isAutonomousMode = true;
static uint8_t LampArrayReportBuffer[64] = {};

__attribute__((weak)) const LampArrayAttributesReport lampArrayAttributesReport = {
    .ReportId = LAMP_ARRAY_ATTRIBUTES_REPORT_ID,
    .LampCount = 61,
    // TODO: adjust these dimensions to GH60
    .BoundingBoxWidthInMillimeters = 282,
    .BoundingBoxHeightInMillimeters = 95,
    .BoundingBoxDepthInMillimeters = 2,
    .LampArrayKind = LampArrayKindKeyboard,
    .MinUpdateIntervalInMilliseconds = 10,
};
__attribute__((weak)) const LampAttributes m_lampAttributes[] = {
    // TODO: adjust these to GH60 compatible
    { .LampId = 0, .PositionXInMillimeters = 262, .PositionYInMillimeters = 13, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_BSPC }, // backspace
    { .LampId = 1, .PositionXInMillimeters = 234, .PositionYInMillimeters = 13, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_EQL },  // +
    { .LampId = 2, .PositionXInMillimeters = 215, .PositionYInMillimeters = 13, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_MINS }, // -
    { .LampId = 3, .PositionXInMillimeters = 196, .PositionYInMillimeters = 13, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_0 },
    { .LampId = 4, .PositionXInMillimeters = 177, .PositionYInMillimeters = 13, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_9 },
    { .LampId = 5, .PositionXInMillimeters = 158, .PositionYInMillimeters = 13, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_8 },
    { .LampId = 6, .PositionXInMillimeters = 140, .PositionYInMillimeters = 13, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_7 },
    { .LampId = 7, .PositionXInMillimeters = 121, .PositionYInMillimeters = 13, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_6 },
    { .LampId = 8, .PositionXInMillimeters = 102, .PositionYInMillimeters = 13, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_5 },
    { .LampId = 9, .PositionXInMillimeters = 83, .PositionYInMillimeters = 13, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_4 },
    { .LampId = 10, .PositionXInMillimeters = 65, .PositionYInMillimeters = 13, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_3 },
    { .LampId = 11, .PositionXInMillimeters = 46, .PositionYInMillimeters = 13, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_2 },
    { .LampId = 12, .PositionXInMillimeters = 27, .PositionYInMillimeters = 4, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_1 },
    { .LampId = 13, .PositionXInMillimeters = 8, .PositionYInMillimeters = 4, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_ESC },     // escape
    { .LampId = 14, .PositionXInMillimeters = 267, .PositionYInMillimeters = 32, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_BSLS }, // backslash
    { .LampId = 15, .PositionXInMillimeters = 243, .PositionYInMillimeters = 32, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_RBRC }, // ]
    { .LampId = 16, .PositionXInMillimeters = 224, .PositionYInMillimeters = 32, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_LBRC }, // [
    { .LampId = 17, .PositionXInMillimeters = 205, .PositionYInMillimeters = 32, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_P },
    { .LampId = 18, .PositionXInMillimeters = 187, .PositionYInMillimeters = 32, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_O },
    { .LampId = 19, .PositionXInMillimeters = 168, .PositionYInMillimeters = 32, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_I },
    { .LampId = 20, .PositionXInMillimeters = 149, .PositionYInMillimeters = 32, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_U },
    { .LampId = 21, .PositionXInMillimeters = 130, .PositionYInMillimeters = 32, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_Y },
    { .LampId = 22, .PositionXInMillimeters = 112, .PositionYInMillimeters = 32, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_T },
    { .LampId = 23, .PositionXInMillimeters = 93, .PositionYInMillimeters = 32, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_R },
    { .LampId = 24, .PositionXInMillimeters = 74, .PositionYInMillimeters = 32, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_E },
    { .LampId = 25, .PositionXInMillimeters = 55, .PositionYInMillimeters = 32, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_W },
    { .LampId = 26, .PositionXInMillimeters = 37, .PositionYInMillimeters = 32, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_Q },
    { .LampId = 27, .PositionXInMillimeters = 12, .PositionYInMillimeters = 32, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_TAB },  // tab
    { .LampId = 28, .PositionXInMillimeters = 260, .PositionYInMillimeters = 52, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_ENT }, // return
    { .LampId = 29, .PositionXInMillimeters = 228, .PositionYInMillimeters = 52, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_QUOT },
    { .LampId = 30, .PositionXInMillimeters = 210, .PositionYInMillimeters = 52, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_SCLN },
    { .LampId = 31, .PositionXInMillimeters = 191, .PositionYInMillimeters = 52, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_L },
    { .LampId = 32, .PositionXInMillimeters = 172, .PositionYInMillimeters = 52, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_K },
    { .LampId = 33, .PositionXInMillimeters = 153, .PositionYInMillimeters = 52, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_J },
    { .LampId = 34, .PositionXInMillimeters = 135, .PositionYInMillimeters = 52, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_H },
    { .LampId = 35, .PositionXInMillimeters = 116, .PositionYInMillimeters = 52, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_G },
    { .LampId = 36, .PositionXInMillimeters = 97, .PositionYInMillimeters = 52, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_F },
    { .LampId = 37, .PositionXInMillimeters = 78, .PositionYInMillimeters = 52, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_D },
    { .LampId = 38, .PositionXInMillimeters = 60, .PositionYInMillimeters = 52, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_S },
    { .LampId = 39, .PositionXInMillimeters = 41, .PositionYInMillimeters = 52, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_A },
    { .LampId = 40, .PositionXInMillimeters = 15, .PositionYInMillimeters = 52, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_CAPS },    // caps
    { .LampId = 41, .PositionXInMillimeters = 271, .PositionYInMillimeters = 71, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_DELETE }, // delete
    { .LampId = 42, .PositionXInMillimeters = 252, .PositionYInMillimeters = 71, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_UP },     // up
    { .LampId = 43, .PositionXInMillimeters = 226, .PositionYInMillimeters = 71, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_RSFT },   // shift
    { .LampId = 44, .PositionXInMillimeters = 200, .PositionYInMillimeters = 71, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_DOT },
    { .LampId = 45, .PositionXInMillimeters = 181, .PositionYInMillimeters = 71, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_COMM },
    { .LampId = 46, .PositionXInMillimeters = 162, .PositionYInMillimeters = 71, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_M },
    { .LampId = 47, .PositionXInMillimeters = 144, .PositionYInMillimeters = 71, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_N },
    { .LampId = 48, .PositionXInMillimeters = 125, .PositionYInMillimeters = 71, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_B },
    { .LampId = 49, .PositionXInMillimeters = 106, .PositionYInMillimeters = 71, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_V },
    { .LampId = 50, .PositionXInMillimeters = 87, .PositionYInMillimeters = 71, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_C },
    { .LampId = 51, .PositionXInMillimeters = 69, .PositionYInMillimeters = 71, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_X },
    { .LampId = 52, .PositionXInMillimeters = 50, .PositionYInMillimeters = 71, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_Z },
    { .LampId = 53, .PositionXInMillimeters = 19, .PositionYInMillimeters = 71, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_LSFT },   // shift
    { .LampId = 54, .PositionXInMillimeters = 271, .PositionYInMillimeters = 90, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_RIGHT }, // right
    { .LampId = 55, .PositionXInMillimeters = 252, .PositionYInMillimeters = 90, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_DOWN },  // down
    { .LampId = 56, .PositionXInMillimeters = 234, .PositionYInMillimeters = 90, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_LEFT },  // left
    { .LampId = 57, .PositionXInMillimeters = 215, .PositionYInMillimeters = 90, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_RCTL },  // ctrl
    { .LampId = 58, .PositionXInMillimeters = 196, .PositionYInMillimeters = 90, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_RALT },  // alt
    { .LampId = 59, .PositionXInMillimeters = 128, .PositionYInMillimeters = 90, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_SPC },   // space
    { .LampId = 60, .PositionXInMillimeters = 57, .PositionYInMillimeters = 90, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeControl, .RedLevelCount = 0xFF, .GreenLevelCount = 0xFF, .BlueLevelCount = 0xFF, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_LALT },   // alt
};

static inline void dynamic_lighting_local_state_reset()
{
    m_lastLampIdRequested = 0;
    m_isAutonomousMode = true;
    memset(LampArrayReportBuffer, 0x00, sizeof(LampArrayReportBuffer));
}

void dynamic_lighting_UpdateRequestLampFromLampAttributesRequestReport(uint8_t *data, uint16_t length)
{
    auxiliary_mode_confirm();

    LampAttributesRequestReport *report = (LampAttributesRequestReport *)data;

    if (report->ReportId == LAMP_ATTRIBUTES_REQUEST_REPORT_ID) {
        // Per HID spec, if not within bounds, always set LampId to 0.
        if (report->LampId < lampArrayAttributesReport.LampCount) {
            m_lastLampIdRequested = report->LampId;
        } else {
            m_lastLampIdRequested = 0;
        }
    }
}

void dynamic_lighting_UpdateLampStateCacheFromMultiUpdateReport(uint8_t *data, uint16_t length)
{
    auxiliary_mode_confirm();

    LampMultiUpdateReport *report = (LampMultiUpdateReport *)data;

    if (report->ReportId == LAMP_MULTI_UPDATE_REPORT_ID) {
        for (uint8_t i = 0; i < report->LampCount; i++) {
            // Ignore update if not within bounds.
            if (report->LampIds[i] < lampArrayAttributesReport.LampCount) {
                // TODO: should we take IntensityChannel into concern?
                auxiliary_rgb_set_color_buffer(report->LampIds[i], report->UpdateColors[i].RedChannel, report->UpdateColors[i].GreenChannel, report->UpdateColors[i].BlueChannel);
            }
        }

        // Don't want the consumer to update before the Host has completed the batch of updates.
        if (report->LampUpdateFlags & LAMP_UPDATE_FLAG_UPDATE_COMPLETE) {
            // nothing to do
        }
    }
}

void dynamic_lighting_UpdateLampStateCacheFromRangeUpdateReport(uint8_t *data, uint16_t length)
{
    auxiliary_mode_confirm();

    LampRangeUpdateReport *report = (LampRangeUpdateReport *)data;

    if (report->ReportId == LAMP_RANGE_UPDATE_REPORT_ID) {
        // Ignore update if not within bounds.
        if (report->LampIdStart >= 0 && report->LampIdStart < lampArrayAttributesReport.LampCount &&
            report->LampIdEnd >= 0 && report->LampIdEnd < lampArrayAttributesReport.LampCount &&
            report->LampIdStart <= report->LampIdEnd) {
            for (uint8_t i = report->LampIdStart; i <= report->LampIdEnd; i++) {
                // TODO: should we take IntensityChannel into concern?
                auxiliary_rgb_set_color_buffer(i, report->UpdateColor.RedChannel, report->UpdateColor.GreenChannel, report->UpdateColor.BlueChannel);
            }

            // Don't want the consumer to update before the Host has completed the batch of updates.
            if (report->LampUpdateFlags & LAMP_UPDATE_FLAG_UPDATE_COMPLETE) {
                // nothing to do
            }
        }
    }
}

void dynamic_lighting_ProcessControlReport(uint8_t *data, uint16_t length)
{
    auxiliary_mode_confirm();

    LampArrayControlReport *report = (LampArrayControlReport *)data;

    if (report->ReportId == LAMP_ARRAY_CONTROL_REPORT_ID) {
        m_isAutonomousMode = !!report->AutonomousMode;
    }
}

void dynamic_lighting_handle_get_report(uint8_t report_id, uint8_t **data, uint32_t *len)
{
    switch (report_id) {
        case LAMP_ARRAY_ATTRIBUTES_REPORT_ID:
            dynamic_lighting_local_state_reset();
            *(LampArrayAttributesReport *)LampArrayReportBuffer = (LampArrayAttributesReport){
                .ReportId = lampArrayAttributesReport.ReportId,
                .LampCount = lampArrayAttributesReport.LampCount,
                .BoundingBoxWidthInMillimeters = MILLIMETERS_TO_MICROMETERS(lampArrayAttributesReport.BoundingBoxWidthInMillimeters),
                .BoundingBoxHeightInMillimeters = MILLIMETERS_TO_MICROMETERS(lampArrayAttributesReport.BoundingBoxHeightInMillimeters),
                .BoundingBoxDepthInMillimeters = MILLIMETERS_TO_MICROMETERS(lampArrayAttributesReport.BoundingBoxDepthInMillimeters),
                .LampArrayKind = lampArrayAttributesReport.LampArrayKind,
                .MinUpdateIntervalInMilliseconds = MILLISECONDS_TO_MICROSECONDS(lampArrayAttributesReport.MinUpdateIntervalInMilliseconds),
            };

            *data = LampArrayReportBuffer;
            *len = sizeof(LampArrayAttributesReport);
            break;
        case LAMP_ATTRIBUTES_RESPONSE_REPORT_ID:
            *(LampAttributesResponseReport *)LampArrayReportBuffer = (LampAttributesResponseReport){
                .ReportId = LAMP_ATTRIBUTES_RESPONSE_REPORT_ID,
                .Attributes.LampId = m_lampAttributes[m_lastLampIdRequested].LampId,
                .Attributes.PositionXInMillimeters = MILLIMETERS_TO_MICROMETERS(m_lampAttributes[m_lastLampIdRequested].PositionXInMillimeters),
                .Attributes.PositionYInMillimeters = MILLIMETERS_TO_MICROMETERS(m_lampAttributes[m_lastLampIdRequested].PositionYInMillimeters),
                .Attributes.PositionZInMillimeters = MILLIMETERS_TO_MICROMETERS(m_lampAttributes[m_lastLampIdRequested].PositionZInMillimeters),
                .Attributes.UpdateLatencyInMilliseconds = MILLISECONDS_TO_MICROSECONDS(m_lampAttributes[m_lastLampIdRequested].UpdateLatencyInMilliseconds),
                .Attributes.LampPurposes = m_lampAttributes[m_lastLampIdRequested].LampPurposes,
                .Attributes.RedLevelCount = m_lampAttributes[m_lastLampIdRequested].RedLevelCount,
                .Attributes.GreenLevelCount = m_lampAttributes[m_lastLampIdRequested].GreenLevelCount,
                .Attributes.BlueLevelCount = m_lampAttributes[m_lastLampIdRequested].BlueLevelCount,
                .Attributes.IntensityLevelCount = m_lampAttributes[m_lastLampIdRequested].IntensityLevelCount,
                .Attributes.IsProgrammable = m_lampAttributes[m_lastLampIdRequested].IsProgrammable,
                .Attributes.LampKey = m_lampAttributes[m_lastLampIdRequested].LampKey,
            };

            *data = LampArrayReportBuffer;
            *len = sizeof(LampAttributesResponseReport);
            m_lastLampIdRequested++;
            if (m_lastLampIdRequested >= lampArrayAttributesReport.LampCount) {
                // Reset to zero
                m_lastLampIdRequested = 0;
            }
            break;
        default:
            break;
    }
}
