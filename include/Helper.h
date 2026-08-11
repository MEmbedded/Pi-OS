#ifndef ARM64_OS_FROM_SCRATCH_DEBUG_H
#define ARM64_OS_FROM_SCRATCH_DEBUG_H
#include <stddef.h>

/**
 * @file Helper.h
 * @brief Useful helper and utility functions for the Pi-OS kernel.
 */

/**
 * @brief Reads the current Exception Level (EL) of the processor.
 *
 * Reads the `CurrentEL` system register to determine the privilege
 * level at which the CPU is currently executing (e.g. EL0, EL1, EL2, EL3).
 *
 * @return The current exception level as an unsigned integer (0–3).
 */
unsigned int get_current_el(void);

/**
 * @brief Converts an integer to its decimal string representation.
 *
 * Writes the null-terminated string representation of @p num into
 * the buffer pointed to by @p str. The caller is responsible for
 * ensuring the buffer is large enough to hold the result.
 *
 * @param num The integer value to convert.
 * @param str Pointer to the destination character buffer.
 */
void int_to_string(int num, char* str);

/**
 * @brief Copies @p n bytes from @p src to @p dest.
 *
 * Copies exactly @p n bytes from the memory area pointed to by @p src
 * to the memory area pointed to by @p dest. The memory areas must not
 * overlap.
 *
 * @param dest Pointer to the destination memory area.
 * @param src  Pointer to the source memory area.
 * @param n    Number of bytes to copy.
 * @return Pointer to the destination memory area @p dest.
 */
void *memcpy(void *dest, const void *src, size_t n);

#endif //ARM64_OS_FROM_SCRATCH_DEBUG_H