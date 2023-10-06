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

#pragma once

#include <stdint.h>

#define LAMP_NOT_PROGRAMMABLE 0x00
#define LAMP_IS_PROGRAMMABLE  0x01

#define LAMP_UPDATE_FLAG_UPDATE_COMPLETE 1

#define MILLISECONDS_TO_MICROSECONDS(x) (x * 1000)
#define MILLIMETERS_TO_MICROMETERS(x)   (x * 1000)

enum LampPurposeKind {
    LampPurposeControl = 1,
    LampPurposeAccent = 2,
    LampPurposeBranding = 4,
    LampPurposeStatus = 8,
    LampPurposeIllumination = 16,
    LampPurposePresentation = 32,
};

enum LampArrayKind {
    LampArrayKindKeyboard = 1,
    LampArrayKindMouse = 2,
    LampArrayKindGameController = 3,
    LampArrayKindPeripheral = 4,
    LampArrayKindScene = 5,
    LampArrayKindNotification = 6,
    LampArrayKindChassis = 7,
    LampArrayKindWearable = 8,
    LampArrayKindFurniture = 9,
    LampArrayKindArt = 10,
};

typedef struct __packed {
    uint8_t RedChannel;
    uint8_t GreenChannel;
    uint8_t BlueChannel;
    uint8_t IntensityChannel;
} LampArrayColor;

typedef struct __packed {
    uint16_t LampId;
    uint32_t PositionXInMillimeters;
    uint32_t PositionYInMillimeters;
    uint32_t PositionZInMillimeters;
    uint32_t UpdateLatencyInMilliseconds;
    uint32_t LampPurposes;
    uint8_t RedLevelCount;
    uint8_t GreenLevelCount;
    uint8_t BlueLevelCount;
    uint8_t IntensityLevelCount;
    uint8_t IsProgrammable;
    uint8_t LampKey;
} LampAttributes;

#define LAMP_ARRAY_ATTRIBUTES_REPORT_ID 1
typedef struct __packed {
    uint8_t ReportId;
    uint16_t LampCount;
    uint32_t BoundingBoxWidthInMillimeters;
    uint32_t BoundingBoxHeightInMillimeters;
    uint32_t BoundingBoxDepthInMillimeters;
    uint32_t LampArrayKind;
    uint32_t MinUpdateIntervalInMilliseconds;
} LampArrayAttributesReport;

#define LAMP_ATTRIBUTES_REQUEST_REPORT_ID 2
typedef struct __packed {
    uint8_t ReportId;
    uint16_t LampId;
} LampAttributesRequestReport;

#define LAMP_ATTRIBUTES_RESPONSE_REPORT_ID 3
typedef struct __packed {
    uint8_t ReportId;
    LampAttributes Attributes;
} LampAttributesResponseReport;

#define LAMP_MULTI_UPDATE_REPORT_ID  4
#define LAMP_MULTI_UPDATE_LAMP_COUNT 8
typedef struct __packed {
    uint8_t ReportId;
    uint8_t LampCount;
    uint8_t LampUpdateFlags;
    uint16_t LampIds[LAMP_MULTI_UPDATE_LAMP_COUNT];
    LampArrayColor UpdateColors[LAMP_MULTI_UPDATE_LAMP_COUNT];
} LampMultiUpdateReport;

#define LAMP_RANGE_UPDATE_REPORT_ID 5
typedef struct __packed {
    uint8_t ReportId;
    uint8_t LampUpdateFlags;
    uint16_t LampIdStart;
    uint16_t LampIdEnd;
    LampArrayColor UpdateColor;
} LampRangeUpdateReport;

#define LAMP_ARRAY_CONTROL_REPORT_ID 6
typedef struct __packed {
    uint8_t ReportId;
    uint8_t AutonomousMode;
} LampArrayControlReport;

void dynamic_lighting_UpdateRequestLampFromLampAttributesRequestReport(uint8_t *data, uint16_t length);
void dynamic_lighting_UpdateLampStateCacheFromMultiUpdateReport(uint8_t *data, uint16_t length);
void dynamic_lighting_UpdateLampStateCacheFromRangeUpdateReport(uint8_t *data, uint16_t length);
void dynamic_lighting_ProcessControlReport(uint8_t *data, uint16_t length);

void dynamic_lighting_handle_get_report(uint8_t report_id, uint8_t **data, uint32_t *len);
