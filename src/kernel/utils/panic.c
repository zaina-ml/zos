#include <graphics/graphics.h>
#include <char/stdout.h>


void panic(char *message)
{
    cls();
    draw_panic(message);
    
    while (1) 
    {
        __asm__ volatile("hlt");
    }
}