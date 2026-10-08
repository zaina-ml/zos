#include <graphics/graphics.h>
#include <terminal/terminal.h>


void panic(char *message)
{
    cls();
    draw_panic(message);
    
    for (;;)
    {
        __asm__ volatile("hlt");
    }
}