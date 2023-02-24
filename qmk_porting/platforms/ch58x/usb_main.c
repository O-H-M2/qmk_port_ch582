#include "usb_main.h"

static uint8_t usbTaskId = INVALID_TASK_ID;

static uint16_t usb_ProcessEvent(uint8_t task_id, uint16_t events)
{
    if (events & USB_RUN_QMK_TASK_EVT) {
        keyboard_task();
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
    return 0;
}

void usb_task_init()
{
    usbTaskId = TMOS_ProcessEventRegister(usb_ProcessEvent);
    tmos_start_task(usbTaskId, USB_RUN_QMK_TASK_EVT, 0);
}