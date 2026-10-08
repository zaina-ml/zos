#include <graphics/graphics.h>
#include <terminal/printk.h>
#include <terminal/terminal.h>

void init(void) 
{
    init_graphics();
    init_terminal();

    printk("Initialization Finished\n");
}