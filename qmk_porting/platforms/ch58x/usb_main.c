#include "usb_main.h"
#include "usb_interface.h"
#include "keycode_config.h"

static uint8_t usbTaskId = INVALID_TASK_ID;
extern uint8_t keyboard_protocol;
extern uint8_t keyboard_idle;

static uint16_t usb_ProcessEvent(uint8_t task_id, uint16_t events)
{
    if (events & USB_RUN_QMK_TASK_EVT) {
        keyboard_task();
        keyboard_check_protocol_mode();
        housekeeping_task();
#ifdef POWER_DETECT_PIN
        if (!readPin(POWER_DETECT_PIN)) {
            // cable removed
            PRINT("Cable pulled out.\n");
            mcu_reset();
        }
#endif
        tmos_start_task(task_id, USB_RUN_QMK_TASK_EVT, 0);

        return (events ^ USB_RUN_QMK_TASK_EVT);
    }
    if (events & USB_PERIODICAL_BIOS_REPORT_EVT) {
#ifdef NKRO_ENABLE
        if (!keymap_config.nkro && keyboard_idle && keyboard_protocol) {
#else
        if (keyboard_idle && keyboard_protocol) {
#endif
            hid_keyboard_send_last_bios_report();
            usb_start_periodical_bios_report();
        }
    }
    return 0;
}

void usb_task_init()
{
    usbTaskId = TMOS_ProcessEventRegister(usb_ProcessEvent);
    tmos_start_task(usbTaskId, USB_RUN_QMK_TASK_EVT, 0);
}

void usb_start_periodical_bios_report()
{
    tmos_stop_task(usbTaskId, USB_PERIODICAL_BIOS_REPORT_EVT);
    tmos_clear_event(usbTaskId, USB_PERIODICAL_BIOS_REPORT_EVT);
    tmos_start_task(usbTaskId, USB_PERIODICAL_BIOS_REPORT_EVT, SYS_TICK_MS(keyboard_idle));
}
