#include <graphics/graphics.h>

#include <init.h>
#include <char/printk.h>
#include <char/stdout.h>

#include <utils/panic.h>


__attribute__((noreturn))
void kmain(void)
{
    init();

    printk("(zosroot$) test\n");
    printk("whats up!\n");
    
    for (;;) {}
}