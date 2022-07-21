#include "usb_porting.h"
#include "host_driver.h"
#include "host.h"

uint8_t keyboard_leds(void);
void    send_keyboard(report_keyboard_t *report);
void    send_mouse(report_mouse_t *report);
void    send_system(uint16_t data);
void    send_consumer(uint16_t data);
void    send_programmable_button(uint32_t data);
void    send_digitizer(report_digitizer_t *report);

host_driver_t ch582_driver = { keyboard_leds, send_keyboard, send_mouse, send_system, send_consumer, send_programmable_button };

void protocol_setup()
{
    usb_device_state_init();
}

void protocol_pre_init()
{
    usb_dc_low_level_init();
}

void protocol_post_init()
{
    host_set_driver(&ch582_driver);
}