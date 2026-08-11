#include "GraphicsDemo.h"
#include "GPIO.h"
#include "MailBox.h"
#include "Timer.h"

static uint32_t displayGrid[48][64];
static uint32_t oldDisplayGrid[48][64];

void draw_square_and_boarder(unsigned int x, unsigned int y, unsigned int size, unsigned int color) {
    for (unsigned int yy = 0; yy < size; yy++)
    {
        for (unsigned int xx = 0; xx < size; xx++)
        {
            if (x+xx==0||y+yy==0||xx+x==(640-1)||yy+y==(480-1)) {

            }else {
                draw_pixel(x + xx, y + yy, color);
            }
        }
    }
}

void initGrid() {
    for (int y = 0; y < 48; y++) {
        for (int x = 0; x < 64; x++) {
            displayGrid[y][x] = 0;
        }
    }
}

void updateGrid() {
    for (int y = 0; y < 48; y++) {
        for (int x = 0; x < 64; x++) {
            if (displayGrid[y][x] != oldDisplayGrid[y][x]) {
                oldDisplayGrid[y][x] = displayGrid[y][x];
                draw_square_and_boarder(x*10,y*10, 10, displayGrid[y][x]);
            }
        }
    }
}

void drawBorder() {
    for (int y = 0; y < 480; y++) {
        for (int x = 0; x < 640; x++) {
            if (x==0||y==0||x==(640-1)||y==(480-1)) {
                draw_pixel(x,y,0x00FFFFFF);
            }
        }
    }
}

int playDemo() {
    initGrid();
    clear_screen(0x000000);
    drawBorder();
    int end = 0;
    int x = 0, y = 0;
    while (!end) {
        if (displayGrid[y][x] == 0x00FFFF00) {
            displayGrid[y][x] = 0;
        }
        else displayGrid[y][x] = 0x00FFFF00;
        updateGrid();

        delay_ms(10);
        ++x;

        if (x == 64) {
            x = 0;
            ++y;
        }
        if (y == 48) {
            y = 0;
            //end = 1;
        }
    }
    return end;
}