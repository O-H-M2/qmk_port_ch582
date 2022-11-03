#include "aw20216_supplement.h"

static volatile bool aw20216_powered_on = true, aw20216_need_power_off = false;

bool aw20216_power_status_get()
{
    return aw20216_powered_on;
}

void aw20216_power_toggle(bool status)
{
    if (aw20216_powered_on == status) {
        return;
    }

    if (status) {
        writePinHigh(DRIVER_1_EN);
        setPinOutput(DRIVER_1_EN);
#ifdef DRIVER_2_EN
        writePinHigh(DRIVER_2_EN);
        setPinOutput(DRIVER_2_EN);
#endif
    } else {
        writePinLow(DRIVER_1_EN);
        setPinOutput(DRIVER_1_EN);
#ifdef DRIVER_2_EN
        writePinLow(DRIVER_2_EN);
        setPinOutput(DRIVER_2_EN);
#endif
    }
    aw20216_powered_on = status;
}

void aw20216_delayed_power_off_set()
{
    aw20216_need_power_off = true;
}

void aw20216_delayed_power_off_excute()
{
    if (aw20216_need_power_off) {
        aw20216_power_toggle(false);
        aw20216_need_power_off = false;
    }
}
