#include "MailBox.h"
#include "Helper.h"
#include "GraphicsDemo.h"
#include "Timer.h"

extern void system_restart(void);

void reboot(void)
{
    system_restart();

    while (1) {
        __asm__ volatile ("wfe");
    }
}

void kernel_main(void) {
    framebuffer_init();
    clear_screen(0x000000);
    char buf[64];

    draw_string(20,20,"Framebuffer",0x00FFFFFF);

    int_to_string((int)screenWidth,buf);
    draw_string(20,40,"screenWidth:",0x00FFFF00);
    draw_string(130,40,buf,0x00FFFFFF);

    int_to_string((int)screenHeight,buf);
    draw_string(20,60,"screenHeight:",0x00FFFF00);
    draw_string(130,60,buf,0x00FFFFFF);

    int_to_string((int)screenPitch,buf);
    draw_string(20,80,"screenPitch:",0x00FFFF00);
    draw_string(130,80,buf,0x00FFFFFF);

    delay_ms(10000);

    int exit = 0;

    while (exit==0){

        exit = playDemo();

    }

    reboot();


}
