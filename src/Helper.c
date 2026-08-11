#include "Helper.h"

#include <stdint.h>

unsigned int get_current_el(void) {
    unsigned long el;
    // Read from system register 'CurrentEL' into variable 'el'
    __asm__ volatile("mrs %0, CurrentEL" : "=r" (el));
    return (el >> 2) & 3; // Bits [3:2] contain the EL number (0, 1, 2, or 3)
}

void int_to_string(int num, char* str) {
    int i = 0;
    int is_negative = 0;

    // Handle 0 explicitly, otherwise the loop won't execute
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // Handle negative numbers
    if (num < 0) {
        is_negative = 1;
        num = -num; // Make number positive for digit extraction
    }

    // Process individual digits from right to left
    while (num != 0) {
        int rem = num % 10;
        str[i++] = rem + '0'; // Convert single digit to ASCII character
        num = num / 10;
    }

    // Append negative sign if necessary
    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0'; // Append null terminator

    // Reverse the string since digits were extracted backward
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *d = (uint8_t *)dest;
    const uint8_t *s = (const uint8_t *)src;

    while (n--) {
        *d++ = *s++;
    }
    return dest;
}