#include "usb_main.h"

static uint8_t usbTaskId = INVALID_TASK_ID;

static uint16_t usb_ProcessEvent(uint8_t task_id, uint16_t events)
{
    if (events & USB_RUN_QMK_TASK_EVT) {
        keyboard_task();
        housekeeping_task();
#ifdef POWER_DETECT_PIN
#error 1
static uint8_t last_state = 0;
uint8_t state = readPin(POWER_DETECT_PIN);

        if (last_state!=state) {
            // cable removed
            PRINT("State changed to: %d\n",state);
            // mcu_reset();
            last_state = state;
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