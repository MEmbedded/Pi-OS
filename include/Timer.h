#ifndef ARM64_OS_FROM_SCRATCH_TIMER_H
#define ARM64_OS_FROM_SCRATCH_TIMER_H

#include <stdint.h>

/**
 * @file Timer.h
 * @brief System timer utilities and delay functions for AArch64.
 *
 * Provides inline functions for reading the ARM generic timer counter
 * and frequency, and for performing busy-wait delays.
 */

/**
 * @brief Reads the current value of the physical counter register.
 *
 * Reads the `cntpct_el0` system register, which increments at the
 * frequency reported by @ref get_timer_frequency().
 *
 * @return Current 64-bit physical counter value.
 */
static inline uint64_t get_timer_count(void)
{
    uint64_t value;

    __asm__ volatile(
        "mrs %0, cntpct_el0"
        : "=r"(value)
    );

    return value;
}

/**
 * @brief Reads the frequency of the system counter in Hz.
 *
 * Reads the `cntfrq_el0` system register, which holds the frequency
 * at which the physical counter (@ref get_timer_count) increments.
 *
 * @return Counter frequency in Hz (e.g. 54000000 for 54 MHz).
 */
static inline uint64_t get_timer_frequency(void)
{
    uint64_t value;

    __asm__ volatile(
        "mrs %0, cntfrq_el0"
        : "=r"(value)
    );

    return value;
}

/**
 * @brief Busy-waits for the specified number of milliseconds.
 *
 * Spins by repeatedly reading the physical counter until the elapsed
 * count corresponds to @p ms milliseconds, based on the counter
 * frequency from @ref get_timer_frequency().
 *
 * @param ms Number of milliseconds to wait.
 */
static inline void delay_ms(uint64_t ms)
{
    uint64_t start = get_timer_count();
    uint64_t freq = get_timer_frequency();

    uint64_t wait = (freq / 1000) * ms;

    while ((get_timer_count() - start) < wait)
    {
        __asm__ volatile("nop");
    }
}

#endif //ARM64_OS_FROM_SCRATCH_TIMER_H