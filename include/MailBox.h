#ifndef ARM64_OS_FROM_SCRATCH_MAILBOX_H
#define ARM64_OS_FROM_SCRATCH_MAILBOX_H

#include <stdint.h>

/**
 * @file MailBox.h
 * @brief Mailbox interface for GPU communication and framebuffer rendering.
 *
 * Provides functions and definitions for interacting with the Raspberry Pi
 * VideoCore GPU via the mailbox peripheral, including framebuffer
 * initialisation and screen drawing primitives.
 */

/** @brief Base address of the MMIO peripheral region. */
#define MMIO_BASE       0x3F000000UL

/** @brief Base address of the mailbox peripheral. */
#define MBOX_BASE       (MMIO_BASE + 0xB880)

/** @brief Mailbox read register. */
#define MBOX_READ       ((volatile uint32_t *)(MBOX_BASE + 0x00))
/** @brief Mailbox status register. */
#define MBOX_STATUS     ((volatile uint32_t *)(MBOX_BASE + 0x18))
/** @brief Mailbox write register. */
#define MBOX_WRITE      ((volatile uint32_t *)(MBOX_BASE + 0x20))

/** @brief Status flag indicating the mailbox is full (cannot write). */
#define MBOX_FULL       0x80000000
/** @brief Status flag indicating the mailbox is empty (cannot read). */
#define MBOX_EMPTY      0x40000000
/** @brief Mailbox channel 8: property tags (ARM → VC). */
#define MBOX_PROP       8

/**
 * @brief Shared mailbox buffer, 16-byte aligned for DMA access.
 *
 * Used to pass property tag messages to and from the GPU.
 */
extern volatile uint32_t mailbox[64] __attribute__((aligned(16)));

/** @brief Framebuffer width in pixels, populated by framebuffer_init(). */
extern uint32_t screenWidth;
/** @brief Framebuffer height in pixels, populated by framebuffer_init(). */
extern uint32_t screenHeight;
/** @brief Framebuffer pitch (bytes per row), populated by framebuffer_init(). */
extern uint32_t screenPitch;
/** @brief Pointer to the base of the framebuffer memory. */
extern uint8_t *framebuffer;

/**
 * @brief Sends the mailbox buffer to the GPU on the specified channel.
 *
 * Waits until the mailbox is ready, writes the buffer address and channel,
 * then waits for and validates the GPU response.
 *
 * @param channel Mailbox channel to use (typically @ref MBOX_PROP).
 * @return Non-zero on success, 0 on failure.
 */
int mailbox_call(uint8_t channel);

/**
 * @brief Initialises the framebuffer via the mailbox property interface.
 *
 * Requests a framebuffer from the GPU with the configured screen dimensions
 * and bit depth, and populates @ref screenWidth, @ref screenHeight,
 * @ref screenPitch, and @ref framebuffer.
 *
 * @return Non-zero on success, 0 if the GPU request failed.
 */
int framebuffer_init(void);

/**
 * @brief Draws a single pixel at the given coordinates.
 *
 * @param x     Horizontal position in pixels (0 = left).
 * @param y     Vertical position in pixels (0 = top).
 * @param color 32-bit RGBA/RGB colour value.
 */
void draw_pixel(unsigned int x, unsigned int y, uint32_t color);

/**
 * @brief Draws a single character at the given coordinates.
 *
 * Renders the character @p c using the built-in 8×8 bitmap font.
 *
 * @param x     Horizontal position of the top-left corner in pixels.
 * @param y     Vertical position of the top-left corner in pixels.
 * @param c     ASCII character to draw.
 * @param color Pixel colour to use for set bits in the font bitmap.
 */
void draw_char(unsigned int x, unsigned int y, char c, unsigned int color);

/**
 * @brief Draws a null-terminated string at the given coordinates.
 *
 * Renders each character of @p s in sequence, advancing horizontally
 * by one character width (8 pixels) per character.
 *
 * @param x     Horizontal position of the first character in pixels.
 * @param y     Vertical position of the first character in pixels.
 * @param s     Null-terminated ASCII string to draw.
 * @param color Pixel colour for the text.
 */
void draw_string(unsigned int x, unsigned int y, const char *s, unsigned int color);

/**
 * @brief Draws a filled square at the given coordinates.
 *
 * @param x     Horizontal position of the top-left corner in pixels.
 * @param y     Vertical position of the top-left corner in pixels.
 * @param size  Side length of the square in pixels.
 * @param color Fill colour.
 */
void draw_square(unsigned int x, unsigned int y, unsigned int size, unsigned int color);

/**
 * @brief Copies a 2D pixel array to the framebuffer.
 *
 * Writes a full screen's worth of pixel data from @p screenData
 * directly to the framebuffer. Each row is assumed to be 640 pixels wide.
 *
 * @param screenData 2D array of 32-bit colour values to display.
 */
void draw_screen(uint32_t screenData[][640]);

/**
 * @brief Clears a square region of the screen to black (0x000000).
 *
 * @param x    Horizontal position of the top-left corner in pixels.
 * @param y    Vertical position of the top-left corner in pixels.
 * @param size Side length of the square in pixels.
 */
void clear_square(unsigned int x, unsigned int y, unsigned int size);

/**
 * @brief Fills the entire screen with a solid colour.
 *
 * @param color 32-bit colour value to fill the screen with.
 */
void clear_screen(uint32_t color);

#endif //ARM64_OS_FROM_SCRATCH_MAILBOX_H