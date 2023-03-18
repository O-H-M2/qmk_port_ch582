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

#pragma once

#include <stdint.h>

// Macro to help make GPIO and other controls atomic.

#if defined BLE_ENABLE || defined ESB_ENABLE

#define ATOMIC_BLOCK              for (uint8_t __ToDo = 1; __ToDo; __ToDo = 0)
#define ATOMIC_BLOCK_RESTORESTATE ATOMIC_BLOCK
#define ATOMIC_BLOCK_FORCEON      ATOMIC_BLOCK

#else

static uint32_t irq_status;

static __inline__ uint8_t __interrupt_disable__(void)
{
    irq_status = (PFIC->ISR[0] >> 8) | (PFIC->ISR[1] << 24);
    PFIC->IRER[0] = 0xffffffff;
    PFIC->IRER[1] = 0xffffffff;

    return 1;
}

static __inline__ void __interrupt_enable__(const uint8_t *__s)
{
    (void)__s;

    PFIC->IENR[0] = (irq_status << 8);
    PFIC->IENR[1] = (irq_status >> 24);

    __nop();
}

#define ATOMIC_BLOCK(type)        for (type, __ToDo = __interrupt_disable__(); __ToDo; __ToDo = 0)
#define ATOMIC_FORCEON            uint8_t sreg_save __attribute__((__cleanup__(__interrupt_enable__))) = 0
#define ATOMIC_BLOCK_RESTORESTATE _Static_assert(0, "ATOMIC_BLOCK_RESTORESTATE not implemented")
#define ATOMIC_BLOCK_FORCEON      ATOMIC_BLOCK(ATOMIC_FORCEON)

#endif