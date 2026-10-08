#include <graphics/graphics.h>
#include <terminal/terminal.h>


void panic(char *message)
{
    cls();
    draw_panic(message);
    
    while (1) 
    {
        __asm__ volatile("hlt");
    }
}