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
    .BoundingBoxWidthInMillimeters = 282,
    .BoundingBoxHeightInMillimeters = 94,
    .BoundingBoxDepthInMillimeters = 2,
    .LampArrayKind = LampArrayKindKeyboard,
    .MinUpdateIntervalInMilliseconds = 10,
};
__attribute__((weak)) const LampAttributes m_lampAttributes[] = {
    { .LampId = 0, .PositionXInMillimeters = 9, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_ESC },
    { .LampId = 1, .PositionXInMillimeters = 28, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_1 },
    { .LampId = 2, .PositionXInMillimeters = 47, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_2 },
    { .LampId = 3, .PositionXInMillimeters = 66, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_3 },
    { .LampId = 4, .PositionXInMillimeters = 85, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_4 },
    { .LampId = 5, .PositionXInMillimeters = 104, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_5 },
    { .LampId = 6, .PositionXInMillimeters = 123, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_6 },
    { .LampId = 7, .PositionXInMillimeters = 142, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_7 },
    { .LampId = 8, .PositionXInMillimeters = 161, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_8 },
    { .LampId = 9, .PositionXInMillimeters = 180, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_9 },
    { .LampId = 10, .PositionXInMillimeters = 199, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_0 },
    { .LampId = 11, .PositionXInMillimeters = 218, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_MINS },
    { .LampId = 12, .PositionXInMillimeters = 237, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_EQL },
    { .LampId = 13, .PositionXInMillimeters = 266, .PositionYInMillimeters = 9, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_BSPC },

    { .LampId = 14, .PositionXInMillimeters = 14, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_TAB },
    { .LampId = 15, .PositionXInMillimeters = 38, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_Q },
    { .LampId = 16, .PositionXInMillimeters = 57, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_W },
    { .LampId = 17, .PositionXInMillimeters = 76, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_E },
    { .LampId = 18, .PositionXInMillimeters = 95, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_R },
    { .LampId = 19, .PositionXInMillimeters = 114, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_T },
    { .LampId = 20, .PositionXInMillimeters = 133, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_Y },
    { .LampId = 21, .PositionXInMillimeters = 152, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_U },
    { .LampId = 22, .PositionXInMillimeters = 171, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_I },
    { .LampId = 23, .PositionXInMillimeters = 190, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_O },
    { .LampId = 24, .PositionXInMillimeters = 209, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_P },
    { .LampId = 25, .PositionXInMillimeters = 228, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_LBRC },
    { .LampId = 26, .PositionXInMillimeters = 247, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_RBRC },
    { .LampId = 27, .PositionXInMillimeters = 271, .PositionYInMillimeters = 28, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_BSLS },

    { .LampId = 28, .PositionXInMillimeters = 16, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_CAPS },
    { .LampId = 29, .PositionXInMillimeters = 42, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_A },
    { .LampId = 30, .PositionXInMillimeters = 61, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_S },
    { .LampId = 31, .PositionXInMillimeters = 80, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_D },
    { .LampId = 32, .PositionXInMillimeters = 99, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_F },
    { .LampId = 33, .PositionXInMillimeters = 118, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_G },
    { .LampId = 34, .PositionXInMillimeters = 137, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_H },
    { .LampId = 35, .PositionXInMillimeters = 156, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_J },
    { .LampId = 36, .PositionXInMillimeters = 175, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_K },
    { .LampId = 37, .PositionXInMillimeters = 194, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_L },
    { .LampId = 38, .PositionXInMillimeters = 213, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_SCLN },
    { .LampId = 39, .PositionXInMillimeters = 232, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_QUOT },
    { .LampId = 40, .PositionXInMillimeters = 264, .PositionYInMillimeters = 47, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_ENT },

    { .LampId = 41, .PositionXInMillimeters = 21, .PositionYInMillimeters = 66, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_LSFT },
    { .LampId = 42, .PositionXInMillimeters = 52, .PositionYInMillimeters = 66, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_Z },
    { .LampId = 43, .PositionXInMillimeters = 71, .PositionYInMillimeters = 66, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_X },
    { .LampId = 44, .PositionXInMillimeters = 90, .PositionYInMillimeters = 66, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_C },
    { .LampId = 45, .PositionXInMillimeters = 109, .PositionYInMillimeters = 66, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_V },
    { .LampId = 46, .PositionXInMillimeters = 128, .PositionYInMillimeters = 66, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_B },
    { .LampId = 47, .PositionXInMillimeters = 147, .PositionYInMillimeters = 66, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_N },
    { .LampId = 48, .PositionXInMillimeters = 166, .PositionYInMillimeters = 66, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_M },
    { .LampId = 49, .PositionXInMillimeters = 185, .PositionYInMillimeters = 66, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_COMM },
    { .LampId = 50, .PositionXInMillimeters = 204, .PositionYInMillimeters = 66, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_DOT },
    { .LampId = 51, .PositionXInMillimeters = 223, .PositionYInMillimeters = 66, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_SLSH },
    { .LampId = 52, .PositionXInMillimeters = 259, .PositionYInMillimeters = 66, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_RSFT },

    { .LampId = 53, .PositionXInMillimeters = 11, .PositionYInMillimeters = 85, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_LCTL },
    { .LampId = 54, .PositionXInMillimeters = 35, .PositionYInMillimeters = 85, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_LGUI },
    { .LampId = 55, .PositionXInMillimeters = 59, .PositionYInMillimeters = 85, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_LALT },
    { .LampId = 56, .PositionXInMillimeters = 130, .PositionYInMillimeters = 85, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_SPC },
    { .LampId = 57, .PositionXInMillimeters = 202, .PositionYInMillimeters = 85, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_RALT },
    { .LampId = 58, .PositionXInMillimeters = 226, .PositionYInMillimeters = 85, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_RGUI },
    { .LampId = 59, .PositionXInMillimeters = 250, .PositionYInMillimeters = 85, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_MENU },
    { .LampId = 60, .PositionXInMillimeters = 274, .PositionYInMillimeters = 85, .UpdateLatencyInMilliseconds = 4, .LampPurposes = LampPurposeAccent, .IntensityLevelCount = 1, .IsProgrammable = 1, .LampKey = KC_RCTL },
};

static inline void dynamic_lighting_local_state_reset()
{
    m_lastLampIdRequested = 0;
    m_isAutonomousMode = true;
    memset(LampArrayReportBuffer, 0x00, sizeof(LampArrayReportBuffer));
}

void dynamic_lighting_UpdateRequestLampFromLampAttributesRequestReport(uint8_t *data, uint16_t length)
{
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
    LampArrayControlReport *report = (LampArrayControlReport *)data;

    if (report->ReportId == LAMP_ARRAY_CONTROL_REPORT_ID) {
        if (m_isAutonomousMode && !report->AutonomousMode) {
            rgb_matrix_enable_noeeprom();
            dynamic_lighting_local_state_reset();
            auxiliary_mode_confirm();
        } else if (!m_isAutonomousMode && report->AutonomousMode) {
            rgb_matrix_reload_from_eeprom();
        }
        m_isAutonomousMode = !!report->AutonomousMode;
    }
}

void dynamic_lighting_handle_get_report(uint8_t report_id, uint8_t **data, uint32_t *len)
{
    static bool m_useQMKPositionRecord = false;
    static uint8_t QMKPositionRecordMinimumX = UINT8_MAX, QMKPositionRecordMaximumX = 0;
    static uint8_t QMKPositionRecordMinimumY = UINT8_MAX, QMKPositionRecordMaximumY = 0;

    switch (report_id) {
        case LAMP_ARRAY_ATTRIBUTES_REPORT_ID:
            *(LampArrayAttributesReport *)LampArrayReportBuffer = (LampArrayAttributesReport){
                .ReportId = lampArrayAttributesReport.ReportId,
                .LampCount = lampArrayAttributesReport.LampCount,
                .BoundingBoxWidthInMillimeters = MILLIMETERS_TO_MICROMETERS(lampArrayAttributesReport.BoundingBoxWidthInMillimeters),
                .BoundingBoxHeightInMillimeters = MILLIMETERS_TO_MICROMETERS(lampArrayAttributesReport.BoundingBoxHeightInMillimeters),
                .BoundingBoxDepthInMillimeters = MILLIMETERS_TO_MICROMETERS(lampArrayAttributesReport.BoundingBoxDepthInMillimeters),
                .LampArrayKind = lampArrayAttributesReport.LampArrayKind,
                .MinUpdateIntervalInMilliseconds = MILLISECONDS_TO_MICROSECONDS(lampArrayAttributesReport.MinUpdateIntervalInMilliseconds),
            };

            for (uint16_t i = 0; i < sizeof(m_lampAttributes) / sizeof(m_lampAttributes[0]); i++) {
                if (!m_lampAttributes[i].PositionXInMillimeters || !m_lampAttributes[i].PositionYInMillimeters) {
                    m_useQMKPositionRecord = true;
                    break;
                }
            }
            if (m_useQMKPositionRecord) {
                for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
                    if (g_led_config.point[i].x < QMKPositionRecordMinimumX) {
                        QMKPositionRecordMinimumX = g_led_config.point[i].x;
                    }
                    if (g_led_config.point[i].x > QMKPositionRecordMaximumX) {
                        QMKPositionRecordMaximumX = g_led_config.point[i].x;
                    }
                    if (g_led_config.point[i].y < QMKPositionRecordMinimumY) {
                        QMKPositionRecordMinimumY = g_led_config.point[i].y;
                    }
                    if (g_led_config.point[i].y > QMKPositionRecordMaximumY) {
                        QMKPositionRecordMaximumY = g_led_config.point[i].y;
                    }
                }
            }

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
                .Attributes.RedLevelCount = m_lampAttributes[m_lastLampIdRequested].RedLevelCount ? m_lampAttributes[m_lastLampIdRequested].RedLevelCount : UINT8_MAX,
                .Attributes.GreenLevelCount = m_lampAttributes[m_lastLampIdRequested].GreenLevelCount ? m_lampAttributes[m_lastLampIdRequested].GreenLevelCount : UINT8_MAX,
                .Attributes.BlueLevelCount = m_lampAttributes[m_lastLampIdRequested].BlueLevelCount ? m_lampAttributes[m_lastLampIdRequested].BlueLevelCount : UINT8_MAX,
                .Attributes.IntensityLevelCount = m_lampAttributes[m_lastLampIdRequested].IntensityLevelCount,
                .Attributes.IsProgrammable = m_lampAttributes[m_lastLampIdRequested].IsProgrammable,
                .Attributes.LampKey = m_lampAttributes[m_lastLampIdRequested].LampKey,
            };

            if (m_useQMKPositionRecord) {
                // calculate position from g_led_config
                uint8_t index = 0;

                for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                    for (uint8_t j = 0; j < MATRIX_COLS; j++) {
                        if (g_led_config.matrix_co[i][j] == ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.LampId) {
                            ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.PositionXInMillimeters =
                                MILLIMETERS_TO_MICROMETERS(lampArrayAttributesReport.BoundingBoxWidthInMillimeters *
                                                           g_led_config.point[index].x / (QMKPositionRecordMaximumX - QMKPositionRecordMinimumX));
                            ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.PositionYInMillimeters =
                                MILLIMETERS_TO_MICROMETERS(lampArrayAttributesReport.BoundingBoxHeightInMillimeters *
                                                           g_led_config.point[index].y / (QMKPositionRecordMaximumY - QMKPositionRecordMinimumY));
                        } else if (g_led_config.matrix_co[i][j] != NO_LED) {
                            index++;
                        }
                    }
                }
            }

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
