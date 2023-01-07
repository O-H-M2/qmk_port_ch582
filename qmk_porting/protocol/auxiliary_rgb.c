#include "quantum.h"
#include "auxiliary_rgb.h"

static uint8_t rgb_raw_hid_buffer[64];

void rgb_raw_hid_receive(uint8_t *data, uint8_t length)
{
    for (uint32_t i = 0; i < 32; i++) {
        rgb_raw_hid_buffer[i] = data[i];
        /* code */
    }
    rgb_raw_hid_send(rgb_raw_hid_buffer, 64);
}

#if !defined(AUXILIARY_RGB_STARTUP_RED)
#define AUXILIARY_RGB_STARTUP_RED 20
#endif

#if !defined(AUXILIARY_RGB_STARTUP_GREEN)
#define AUXILIARY_RGB_STARTUP_GREEN 120
#endif

#if !defined(AUXILIARY_RGB_STARTUP_BLUE)
#define AUXILIARY_RGB_STARTUP_BLUE 190
#endif

// global
uint16_t g_auxiliary_rgb_timer = 0;
bool g_auxiliary_rgb_anim_playing = false;

// internals
static uint8_t auxiliary_rgb_mode = HID_MODE_OPENRGB;
static RGB last_openrgb_colors[DRIVER_LED_TOTAL] = { [0 ... DRIVER_LED_TOTAL - 1] = { AUXILIARY_RGB_STARTUP_GREEN, AUXILIARY_RGB_STARTUP_RED, AUXILIARY_RGB_STARTUP_BLUE } };

//set one matrix led color
void auxiliary_rgb_set_color(int index, uint8_t red, uint8_t green, uint8_t blue)
{
    if (rgb_matrix_get_mode() != RGB_MATRIX_CUSTOM_AUXILIARY_RGB) { // if matrix mode not signalrgb, do not streaming led.
        return;
    }

    //cache openRGB colors
    if (auxiliary_rgb_get_mode() == HID_MODE_OPENRGB) {
        last_openrgb_colors[index].r = red;
        last_openrgb_colors[index].g = green;
        last_openrgb_colors[index].b = blue;
    } else {
#ifdef AUXILIARY_RGB_USE_UNIVERSAL_BRIGHTNESS
        float brightness = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
        rgb_matrix_set_color(index, brightness * red, brightness * green, brightness * blue);
#else
        rgb_matrix_set_color(index, red, green, blue);
#endif
    }
}

//set openrgb colors
void auxiliary_rgb_reload_openrgb_colors(void)
{
    if (auxiliary_rgb_get_mode() == HID_MODE_OPENRGB) {
        for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
            auxiliary_rgb_set_color(i, last_openrgb_colors[i].r, last_openrgb_colors[i].g, last_openrgb_colors[i].b);
        }
    }
}

void auxiliary_rgb_reload_openrgb_anim(void)
{
    if (!g_auxiliary_rgb_anim_playing)
        g_auxiliary_rgb_timer = 0;
    rgb_matrix_set_color_all(0, 0, 0);
}

//get openrgb colors
RGB *auxiliary_rgb_get_openrgb_colors(void)
{
    return last_openrgb_colors;
}

RGB auxiliary_rgb_get_openrgb_color(int index)
{
    return last_openrgb_colors[index];
}

//set hidrgb software mode, HID_MODE_OPENRGB or HID_MODE_SIGNALRGB
void auxiliary_rgb_set_mode(int mode)
{
    auxiliary_rgb_mode = mode;
}

//get hidrgb software mode, HID_MODE_OPENRGB or HID_MODE_SIGNALRGB
uint8_t auxiliary_rgb_get_mode(void)
{
    return auxiliary_rgb_mode;
}

//receive HIDRGB usbhid raw data.
void rgb_hid_receive(uint8_t *data, uint8_t length)
{
    bool send = false;

    switch (*data) {
#ifdef OPENRGB_ENABLE
        case 1 ... 9: {
            send = openrgb_command_handler(data, length);
            break;
        }
#endif

#ifdef SIGNALRGB_SUPPORT_ENABLE
        case 0x21 ... 0x28: {
            send = signal_rgb_command_handler(data, length);
            break;
        }
#endif
        default: {
            // id_unhandled
            break;
        }
    }

    if (send) {
        rgb_raw_hid_send(data, length);
    }
}
