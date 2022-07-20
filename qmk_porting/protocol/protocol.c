#include "usb_porting.h"

void protocol_setup(void)
{
    usb_device_state_init();
}

void protocol_pre_init(void)
{
    usb_dc_low_level_init();
    PFIC_EnableAllIRQ();
}