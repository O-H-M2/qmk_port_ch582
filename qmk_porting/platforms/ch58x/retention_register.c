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

#include "retention_register.h"

void retention_register_clear()
{
    R8_GLOB_RESET_KEEP = 0;
}

void retention_register_set_iap()
{
    R8_GLOB_RESET_KEEP = BOOTLOADER_BOOT_MODE_IAP;
}

bool retention_register_get_iap()
{
    return (R8_GLOB_RESET_KEEP == BOOTLOADER_BOOT_MODE_IAP);
}
