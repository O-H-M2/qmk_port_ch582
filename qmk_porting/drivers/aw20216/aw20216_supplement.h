#pragma once

#include <stdbool.h>
#include "gpio.h"

#define AW20216_POWER_CHECK              \
    if (!aw20216_power_status_get()) {   \
        aw20216_power_toggle(true);      \
    }                                    \
    if (!rgbled_status_check()) {        \
        aw20216_delayed_power_off_set(); \
    }

bool aw20216_power_status_get();
void aw20216_power_toggle(bool status);
void aw20216_delayed_power_off_set();
void aw20216_delayed_power_off_excute();
