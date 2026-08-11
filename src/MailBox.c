#include "MailBox.h"
#include "Helper.h"
#include "font8x8.h"
#include <stddef.h>

volatile uint32_t mailbox[64] __attribute__((aligned(16)));

uint32_t screenWidth;
uint32_t screenHeight;
uint32_t screenPitch;
uint8_t *framebuffer;

int mailbox_call(uint8_t channel)
{
    uint32_t addr =
    ((uint32_t)((uintptr_t)mailbox) & ~0xF) |
    (channel & 0xF);

    while (*MBOX_STATUS & MBOX_FULL)
        ;

    *MBOX_WRITE = addr;

    while (1)
    {
        while (*MBOX_STATUS & MBOX_EMPTY)
            ;

        if (*MBOX_READ == addr)
        {
            return mailbox[1] == 0x80000000;
        }
    }
}

int framebuffer_init(void)
{
    mailbox[0] = 35 * 4;          // Total message size in bytes
    mailbox[1] = 0;               // Request

    // Set physical size
    mailbox[2] = 0x48003;
    mailbox[3] = 8;
    mailbox[4] = 8;
    mailbox[5] = 640;
    mailbox[6] = 480;

    // Set virtual size
    mailbox[7] = 0x48004;
    mailbox[8] = 8;
    mailbox[9] = 8;
    mailbox[10] = 640;
    mailbox[11] = 480;

    // Set virtual offset
    mailbox[12] = 0x48009;
    mailbox[13] = 8;
    mailbox[14] = 8;
    mailbox[15] = 0;
    mailbox[16] = 0;

    // Set depth
    mailbox[17] = 0x48005;
    mailbox[18] = 4;
    mailbox[19] = 4;
    mailbox[20] = 32;

    // Allocate framebuffer
    mailbox[21] = 0x40001;
    mailbox[22] = 8;
    mailbox[23] = 8;
    mailbox[24] = 16;
    mailbox[25] = 0;

    // Get screenPitch
    mailbox[26] = 0x40008;
    mailbox[27] = 4;
    mailbox[28] = 0;
    mailbox[29] = 0;

    mailbox[30] = 0;      // End tag

    if (!mailbox_call(MBOX_PROP))
        return 0;

    framebuffer = (uint8_t *)(uintptr_t)(mailbox[24] & 0x3FFFFFFF);

    screenPitch  = mailbox[29];
    screenWidth  = mailbox[5];
    screenHeight = mailbox[6];

    return framebuffer != NULL;
}

void draw_pixel(unsigned int x, unsigned int y, uint32_t color)
{
    uint32_t *pixel =
        (uint32_t *)(framebuffer + y * screenPitch + x * 4);

    *pixel = color;
}

void draw_char(unsigned int x, unsigned int y, char c, unsigned int color)
{
    for (unsigned int row = 0; row < 8; row++)
    {
        unsigned char bits = font8x8_basic[(unsigned char)c][row];

        for (unsigned int col = 0; col < 8; col++)
        {
            if (bits & (1 << (col)))
                draw_pixel(x + col, y + row, color);
        }
    }
}

void draw_string(unsigned int x, unsigned int y, const char *s, unsigned int color)
{
    while (*s)
    {
        draw_char(x, y, *s++, color);
        x += 8;
    }
}

void draw_square(unsigned int x, unsigned int y, unsigned int size, unsigned int color)
{
    for (unsigned int yy = 0; yy < size; yy++)
    {
        for (unsigned int xx = 0; xx < size; xx++)
        {
            draw_pixel(x + xx, y + yy, color);
        }
    }
}

void draw_screen(uint32_t screenData[][640])
{
    memcpy(framebuffer,
           screenData,
           screenWidth * screenHeight * sizeof(uint32_t));
}

void clear_screen(uint32_t color)
{
    for (uint32_t y = 0; y < screenHeight; y++)
    {
        uint32_t *row = (uint32_t *)(framebuffer + y * screenPitch);

        for (uint32_t x = 0; x < screenWidth; x++)
            row[x] = color;
    }
}

void clear_square(unsigned int x, unsigned int y, unsigned int size)
{
    draw_square(x, y, size, 0x000000);
}