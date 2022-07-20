/* Copyright 2021 QMK
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <stdint.h>
#include "core_riscv.h"
#include "CH58x_sys.h"

// Macro to help make GPIO and other controls atomic.
// The SYS_DisableAllIrq() & SYS_RecoverIrq() way ignores all interrupt toggle operations in critical session

// static uint32_t irq_status;

static __inline__ uint8_t __interrupt_disable__(void)
{
    PFIC_DisableAllIRQ();
    // SYS_DisableAllIrq(&irq_status);

    return 1;
}

static __inline__ void __interrupt_enable__(const uint8_t *__s)
{
    PFIC_EnableAllIRQ();
    // SYS_RecoverIrq(irq_status);

    __nop();
    (void)__s;
}

#define ATOMIC_BLOCK(type) for (type, __ToDo = __interrupt_disable__(); __ToDo; __ToDo = 0)
#define ATOMIC_FORCEON     uint8_t sreg_save __attribute__((__cleanup__(__interrupt_enable__))) = 0

#define ATOMIC_BLOCK_RESTORESTATE _Static_assert(0, "ATOMIC_BLOCK_RESTORESTATE not implemented")
#define ATOMIC_BLOCK_FORCEON      ATOMIC_BLOCK(ATOMIC_FORCEON)
