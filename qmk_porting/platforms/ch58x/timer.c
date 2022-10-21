#include "timer.h"

static uint32_t ticks_offset = 0;

inline void timer_init(void)
{
    timer_clear();
}

inline uint16_t timer_read(void)
{
    return (uint16_t)(timer_read32() & 0xFFFF);
}

uint32_t timer_read32(void)
{
    return SYS_TO_MS(timer_readraw() - ticks_offset);
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

__HIGH_CODE uint32_t timer_readraw(void)
{
    uint32_t i;

    do {
        i = R32_RTC_CNT_32K;
    } while (i != R32_RTC_CNT_32K);

    return (i);
}
