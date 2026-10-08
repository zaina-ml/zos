#include <graphics/graphics.h>

#include <init.h>
#include <terminal/printk.h>

#include <panic.h>


__attribute__((noreturn))
void kmain(void)
{
    init();

    printk("(zosroot$) test\n");
    printk("whats up!\ndon't.");
    
    for (;;) {
        __asm__ volatile("hlt");
    }
}