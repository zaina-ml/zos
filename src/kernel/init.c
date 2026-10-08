#include <graphics/graphics.h>
#include <char/printk.h>
#include <char/stdout.h>

void init(void) 
{
    init_graphics();
    init_stdout();

    printk("Initialization Finished\n");
}