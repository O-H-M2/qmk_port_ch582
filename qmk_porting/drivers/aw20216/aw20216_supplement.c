/*
Copyright 2022 Huckies <https://github.com/Huckies>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
