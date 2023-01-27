#ifndef RAW_ENABLE
#error "RAW HID Communication is not enabled" //This should be impossible to run into afaik. Common_features ensures RAWHID is enabled.
#endif

#include "quantum.h"
#include <string.h>
#include "signalrgb.h"

static uint8_t packet[64];

void get_qmk_version(void) //Grab the QMK Version the board's firmware is built off of
{
    packet[0] = GET_QMK_VERSION;
    packet[1] = QMK_VERSION_BYTE_1;
    packet[2] = QMK_VERSION_BYTE_2;
    packet[3] = QMK_VERSION_BYTE_3;
}

void get_signalrgb_protocol_version(void)
{
    packet[0] = GET_PROTOCOL_VERSION;
    packet[1] = PROTOCOL_VERSION_BYTE_1;
    packet[2] = PROTOCOL_VERSION_BYTE_2;
    packet[3] = PROTOCOL_VERSION_BYTE_3;
}

void get_unique_identifier(void) //Grab the unique identifier for each specific model of keyboard.
{
    packet[0] = GET_UNIQUE_IDENTIFIER;
    packet[1] = DEVICE_UNIQUE_IDENTIFIER_BYTE_1;
    packet[2] = DEVICE_UNIQUE_IDENTIFIER_BYTE_2;
    packet[3] = DEVICE_UNIQUE_IDENTIFIER_BYTE_3;
}

void led_streaming(uint8_t *data) //Stream data from HID Packets to Keyboard.
{
    uint8_t index = data[1];
    uint8_t numberofleds = data[2];

    if (numberofleds >= 10) {
        packet[1] = DEVICE_ERROR_LEDS;
        return;
    }

    for (uint8_t i = 0; i < numberofleds; i++) {
        uint8_t offset = (i * 3) + 3;
        uint8_t r = data[offset];
        uint8_t g = data[offset + 1];
        uint8_t b = data[offset + 2];

        auxiliary_rgb_set_color_buffer(index + i, r, g, b);
    }
}

void signalrgb_mode_enable(void)
{
    rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_AUXILIARY_RGB); //Set RGB Matrix to SignalRGB Compatible Mode
    auxiliary_rgb_flush();
}

void signalrgb_mode_disable(void)
{
    rgb_matrix_reload_from_eeprom(); //Reloading last effect from eeprom
}

void get_total_leds(void) //Grab total number of leds that a board has.
{
    packet[0] = GET_TOTAL_LEDS;
    packet[1] = RGB_MATRIX_LED_COUNT;
}

void get_firmware_type(void) //Grab which fork of qmk a board is running.
{
    packet[0] = GET_FIRMWARE_TYPE;
    packet[1] = FIRMWARE_TYPE_BYTE;
}
bool signal_rgb_command_handler(uint8_t *data, uint8_t length)
{
    switch (*data) {
        case GET_QMK_VERSION:

            get_qmk_version();

            break;
        case GET_PROTOCOL_VERSION:

            get_signalrgb_protocol_version();

            break;
        case GET_UNIQUE_IDENTIFIER:

            get_unique_identifier();

            break;
        case STREAM_RGB_DATA:
            auxiliary_mode_confirm();
            led_streaming(data);

            break;

        case SET_SIGNALRGB_MODE_ENABLE:

            signalrgb_mode_enable();

            break;

        case SET_SIGNALRGB_MODE_DISABLE:

            signalrgb_mode_disable();

            break;

        case GET_TOTAL_LEDS:

            get_total_leds();

            break;

        case GET_FIRMWARE_TYPE:

            get_firmware_type();

            break;

        default:
            break;
    }

    if (*data != STREAM_RGB_DATA) {
        memcpy(data, packet, SINGALRGB_EPSIZE);
        memset(packet, 0x00, SINGALRGB_EPSIZE);
        return true;
    }
    return false;
}