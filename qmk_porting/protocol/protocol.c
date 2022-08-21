#include "host_driver.h"
#include "host.h"
#include "usb_device_state.h"
#include "usb_main.h"
#include "print.h"
#include "CH58x_common.h"
#include "keycode_config.h"

static uint8_t keyboard_led_state;

uint8_t keyboard_leds()
{
    return keyboard_led_state;
}

uint8_t set_keyboard_leds(uint8_t state)
{
    keyboard_led_state = state;
}

void send_keyboard(report_keyboard_t *report)
{
#ifdef NKRO_ENABLE
    if (keymap_config.nkro) {
        uint8_t report_to_send[EXKEY_IN_EP_SIZE];

        memcpy(report_to_send, &report->nkro, sizeof(report_to_send));
        hid_nkro_keyboard_send_report(report_to_send, sizeof(report_to_send));
    } else
#endif
    {
        uint8_t report_to_send[KEYBOARD_REPORT_SIZE];

        memcpy(report_to_send, report, sizeof(report_to_send));
        hid_bios_keyboard_send_report(report_to_send, sizeof(report_to_send));
    }
}

void send_mouse(report_mouse_t *report)
{
    uint8_t report_to_send[6];

    report_to_send[0] = REPORT_ID_MOUSE;
    memcpy(report_to_send + 1, report, 5);
    hid_exkey_send_report(report_to_send, 6);
}

void send_consumer(uint16_t data)
{
    uint8_t report_to_send[3];

    report_to_send[0] = REPORT_ID_CONSUMER;
    memcpy(report_to_send + 1, &data, 2);
    hid_exkey_send_report(report_to_send, 3);
}

void send_system(uint16_t data)
{
    uint8_t report_to_send[3];

    report_to_send[0] = REPORT_ID_SYSTEM;
    memcpy(report_to_send + 1, &data, 2);
    hid_exkey_send_report(report_to_send, 3);
}

void send_programmable_button(uint32_t data)
{
}

void send_digitizer(report_digitizer_t *report)
{
}

host_driver_t ch582_driver = { keyboard_leds, send_keyboard, send_mouse, send_system, send_consumer, send_programmable_button };

void protocol_setup()
{
    usb_device_state_init();
}

void protocol_pre_init()
{
    init_usb_driver();
}

void protocol_post_init()
{
    host_set_driver(&ch582_driver);
}

void protocol_pre_task()
{
}

void protocol_post_task()
{
    // this will be handle by the stack
}

#ifdef DEBUG
int8_t ch58x_send_char(uint8_t s)
{
    while (R8_UART1_TFC == UART_FIFO_SIZE) {
        __nop();
    }
    R8_UART1_THR = s;
    return 0;
}
#endif

void keyboard_post_init_user(void)
{
#ifdef DEBUG
    print_set_sendchar(ch58x_send_char);
    print("Set log output for QMK.\n");
#endif
}