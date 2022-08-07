#include "timer.h"

static uint32_t ticks_offset = 0;

void timer_init(void)
{
    timer_clear();
}

inline uint16_t timer_read()
{
    return (uint16_t)(timer_read32() & 0xFFFF);
}

inline uint32_t timer_read32()
{
    uint32_t tick;

    do {
        tick = R32_RTC_CNT_32K;
    } while (tick != R32_RTC_CNT_32K);

    return SYS_TO_MS(tick - ticks_offset);
}

inline uint64_t timer_read64(void)
{
    return (uint64_t)timer_read32();
}

inline uint16_t timer_elapsed(uint16_t tlast)
{
    return TIMER_DIFF_16(timer_read(), tlast);
}

inline uint32_t timer_elapsed32(uint32_t tlast)
{
    return TIMER_DIFF_32(timer_read32(), tlast);
}

void timer_clear(void)
{
    ticks_offset = timer_read32();
}
