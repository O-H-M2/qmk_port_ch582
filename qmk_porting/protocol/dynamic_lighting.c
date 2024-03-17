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
#include "keymap_introspection.h"

static uint16_t m_lastLampIdRequested = 0;
static bool m_isAutonomousMode = true;
static uint8_t LampArrayReportBuffer[64] = {};

__attribute__((weak)) const LampArrayAttributesReport lampArrayAttributesReport = {
    .ReportId = LAMP_ARRAY_ATTRIBUTES_REPORT_ID,
    .LampCount = RGB_MATRIX_LED_COUNT,
    .BoundingBoxWidthInMillimeters = (MATRIX_COLS < 16) ? ((MATRIX_COLS + 1) * 19.05) : ((MATRIX_COLS < 19) ? ((MATRIX_COLS + 2) * 19.05) : ((MATRIX_COLS + 3) * 19.05)),
    .BoundingBoxHeightInMillimeters = MATRIX_ROWS * 19.05,
    .BoundingBoxDepthInMillimeters = 2,
    .LampArrayKind = LampArrayKindKeyboard,
    .MinUpdateIntervalInMilliseconds = 10,
};
__attribute__((weak)) const LampAttributes m_lampAttributes[] = {};
static const bool m_useQMKPositionRecord = sizeof(m_lampAttributes) == 0;

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
    if (!m_isAutonomousMode) {
        auxiliary_mode_confirm();
    }

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
    if (!m_isAutonomousMode) {
        auxiliary_mode_confirm();
    }

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
            };

            if (m_useQMKPositionRecord) {
                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.LampId = m_lastLampIdRequested;

                // calculate position from g_led_config
                uint8_t index = 0;

                for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                    for (uint8_t j = 0; j < MATRIX_COLS; j++) {
                        if (g_led_config.matrix_co[i][j] == ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.LampId) {
                            // fill in XY coordinates
                            ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.PositionXInMillimeters =
                                MILLIMETERS_TO_MICROMETERS(lampArrayAttributesReport.BoundingBoxWidthInMillimeters *
                                                           g_led_config.point[index].x / (QMKPositionRecordMaximumX - QMKPositionRecordMinimumX));
                            ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.PositionYInMillimeters =
                                MILLIMETERS_TO_MICROMETERS(lampArrayAttributesReport.BoundingBoxHeightInMillimeters *
                                                           g_led_config.point[index].y / (QMKPositionRecordMaximumY - QMKPositionRecordMinimumY));

                            // fill in lamp purpose and Z coordinate if needed
                            if (g_led_config.flags[index] & LED_FLAG_UNDERGLOW) {
                                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.PositionZInMillimeters =
                                    MILLIMETERS_TO_MICROMETERS(lampArrayAttributesReport.BoundingBoxDepthInMillimeters);
                                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.LampPurposes = LampPurposeAccent;
                            } else if (g_led_config.flags[index] & LED_FLAG_INDICATOR) {
                                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.LampPurposes = LampPurposeStatus;
                            } else {
                                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.LampPurposes = LampPurposeControl;
                            }

                            // fill in keycode
                            uint16_t keycode = keycode_at_keymap_location(get_highest_layer(default_layer_state), i, j);

                            if (IS_BASIC_KEYCODE(keycode) || IS_MODIFIER_KEYCODE(keycode)) {
                                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.LampKey = keycode;
                            } else {
                                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.LampKey = KC_NO;
                            }
                        } else if (g_led_config.matrix_co[i][j] != NO_LED) {
                            index++;
                        }
                    }
                }
                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.PositionZInMillimeters = MILLIMETERS_TO_MICROMETERS(0);
                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.UpdateLatencyInMilliseconds = MILLISECONDS_TO_MICROSECONDS(4);
                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.RedLevelCount = UINT8_MAX;
                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.GreenLevelCount = UINT8_MAX;
                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.BlueLevelCount = UINT8_MAX;
                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.IntensityLevelCount = 1;
                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.IsProgrammable = 1;
            } else {
                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.LampId = m_lampAttributes[m_lastLampIdRequested].LampId;
                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.PositionXInMillimeters = MILLIMETERS_TO_MICROMETERS(m_lampAttributes[m_lastLampIdRequested].PositionXInMillimeters);
                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.PositionYInMillimeters = MILLIMETERS_TO_MICROMETERS(m_lampAttributes[m_lastLampIdRequested].PositionYInMillimeters);
                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.PositionZInMillimeters = MILLIMETERS_TO_MICROMETERS(m_lampAttributes[m_lastLampIdRequested].PositionZInMillimeters);
                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.UpdateLatencyInMilliseconds = MILLISECONDS_TO_MICROSECONDS(m_lampAttributes[m_lastLampIdRequested].UpdateLatencyInMilliseconds);
                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.LampPurposes = m_lampAttributes[m_lastLampIdRequested].LampPurposes;
                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.RedLevelCount = m_lampAttributes[m_lastLampIdRequested].RedLevelCount ? m_lampAttributes[m_lastLampIdRequested].RedLevelCount : UINT8_MAX;
                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.GreenLevelCount = m_lampAttributes[m_lastLampIdRequested].GreenLevelCount ? m_lampAttributes[m_lastLampIdRequested].GreenLevelCount : UINT8_MAX;
                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.BlueLevelCount = m_lampAttributes[m_lastLampIdRequested].BlueLevelCount ? m_lampAttributes[m_lastLampIdRequested].BlueLevelCount : UINT8_MAX;
                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.IntensityLevelCount = m_lampAttributes[m_lastLampIdRequested].IntensityLevelCount;
                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.IsProgrammable = m_lampAttributes[m_lastLampIdRequested].IsProgrammable;
                ((LampAttributesResponseReport *)LampArrayReportBuffer)->Attributes.LampKey = m_lampAttributes[m_lastLampIdRequested].LampKey;
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
