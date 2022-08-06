#include "timer.h"

void timer_init(void)
{
    R32_TMR0_CNT_END = FREQ_SYS / 1000;
    R8_TMR0_CTRL_MOD = RB_TMR_ALL_CLEAR;
    R8_TMR0_CTRL_MOD = RB_TMR_COUNT_EN;
    // clear interrupt bits
    R8_TMR0_INTER_EN = 0;
    // and disable interrupt
    PFIC_EnableIRQ(TMR0_IRQn);
}

uint16_t timer_read(void)
{
    return R16_TMR0_COUNT;
}

uint32_t timer_read32(void)
{
    return R32_TMR0_COUNT;
}

uint64_t timer_read64(void)
{
    return (uint64_t)R32_TMR0_COUNT;
}

uint16_t timer_elapsed(uint16_t tlast)
{
    return TIMER_DIFF_16(timer_read(), tlast);
}

uint32_t timer_elapsed32(uint32_t tlast)
{
    return TIMER_DIFF_32(timer_read32(), tlast);
}

void timer_clear(void)
{
    R8_TMR0_CTRL_MOD = RB_TMR_ALL_CLEAR;
}
