/*
Copyright 2011 Jun Wako <wakojun@gmail.com>
Copyright 2021 Simon Arlott

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

// The platform is 32-bit, so prefer 32-bit timers to avoid overflow
#define FAST_TIMER_T_SIZE 32

#include <stdint.h>
#include "CH58x_common.h"
#include "CH583SFR.h"

#define TIMER_DIFF(a, b, max)           ((max == UINT8_MAX) ? ((uint8_t)((a) - (b))) : ((max == UINT16_MAX) ? ((uint16_t)((a) - (b))) : ((max == UINT32_MAX) ? ((uint32_t)((a) - (b))) : ((a) >= (b) ? (a) - (b) : (max) + 1 - (b) + (a)))))
#define TIMER_DIFF_8(a, b)              TIMER_DIFF(a, b, UINT8_MAX)
#define TIMER_DIFF_16(a, b)             TIMER_DIFF(a, b, UINT16_MAX)
#define TIMER_DIFF_32(a, b)             TIMER_DIFF(a, b, UINT32_MAX)
#define TIMER_DIFF_RAW(a, b)            TIMER_DIFF_8(a, b)
#define TIMER_DIFF_RTC(_past, _current) ((_current > _past) ? (_current - _past) : (_current + RTC_TIMER_MAX_VALUE - _past))

#define CLK_PER_US (1.0 / ((1.0 / FREQ_RTC) * 1000 * 1000)) // 0.032 clk/us
#define CLK_PER_MS (CLK_PER_US * 1000)                      // 32clk/ms

#define US_PER_CLK (1.0 / CLK_PER_US)    //   31.25us/clk
#define MS_PER_CLK (US_PER_CLK / 1000.0) //   0.03125ms/clk

#define SYS_TO_US(clk) ((uint32_t)((clk)*US_PER_CLK))
#define SYS_TO_MS(clk) ((uint32_t)((clk)*MS_PER_CLK))

#define US_TO_SYS(us) ((uint32_t)((us)*CLK_PER_US))
#define MS_TO_SYS(ms) ((uint32_t)((ms)*CLK_PER_MS))

#ifdef __cplusplus
extern "C" {
#endif

extern volatile uint32_t timer_count;

void timer_init(void);
void timer_clear(void);
uint16_t timer_read(void);
uint32_t timer_read32(void);
uint16_t timer_elapsed(uint16_t last);
uint32_t timer_elapsed32(uint32_t last);
uint32_t timer_readraw(void);

// Utility functions to check if a future time has expired & autmatically handle time wrapping if checked / reset frequently (half of max value)
#define timer_expired(current, future)   ((uint16_t)(current - future) < UINT16_MAX / 2)
#define timer_expired32(current, future) ((uint32_t)(current - future) < UINT32_MAX / 2)

#define TIMER_DIFF_FAST(a, b)               TIMER_DIFF_32(a, b)
#define timer_expired_fast(current, future) timer_expired32(current, future)
typedef uint32_t fast_timer_t;
fast_timer_t inline timer_read_fast(void)
{
    return timer_read32();
}
fast_timer_t inline timer_elapsed_fast(fast_timer_t last)
{
    return timer_elapsed32(last);
}

#ifdef __cplusplus
}
#endif
