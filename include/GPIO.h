#ifndef ARM64_OS_FROM_SCRATCH_GPIO_H
#define ARM64_OS_FROM_SCRATCH_GPIO_H
#include "Timer.h"

/**
 * @file GPIO.h
 * @brief GPIO control for the Raspberry Pi bare-metal environment.
 *
 * Currently configures GPIO pin 17 as an output and provides a
 * function to blink an externally connected LED.
 */

/** @brief Base address of the GPIO peripheral. */
#define GPIO_BASE 0x3F200000

/** @brief GPIO Function Select 1 register (controls pins 10–19). */
#define GPFSEL1 (*(volatile unsigned int *)(GPIO_BASE + 0x04))
/** @brief GPIO Pin Output Set 0 register (sets pins 0–31 high). */
#define GPSET0  (*(volatile unsigned int *)(GPIO_BASE + 0x1C))
/** @brief GPIO Pin Output Clear 0 register (sets pins 0–31 low). */
#define GPCLR0  (*(volatile unsigned int *)(GPIO_BASE + 0x28))

/**
 * @brief Configures GPIO pin 17 as a push-pull output.
 *
 * Clears then sets bits 21:23 of @ref GPFSEL1 to select the output
 * function (001) for GPIO17.
 */
static inline void gpio17_output(void)
{
    // GPIO17 is controlled by GPFSEL1 bits 21:23
    GPFSEL1 &= ~(7 << 21);  // clear function
    GPFSEL1 |=  (1 << 21);  // set output
}

/**
 * @brief Blinks the LED connected to GPIO pin 17 once.
 *
 * Configures GPIO17 as an output via @ref gpio17_output(), then
 * drives the pin high for 50 ms and low for 50 ms, producing a
 * single blink of the connected LED.
 */
static inline void blink_led(void)
{
    gpio17_output();

    {
        GPSET0 = (1 << 17); // LED ON
        delay_ms(50);

        GPCLR0 = (1 << 17); // LED OFF
        delay_ms(50);
    }
}

#endif //ARM64_OS_FROM_SCRATCH_GPIO_H