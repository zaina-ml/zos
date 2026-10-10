#include <arch/x86_64/idt.h>
#include <arch/x86_64/pic.h>
#include <drivers/input/keyboard.h>
#include <graphics/graphics.h>
#include <terminal/printk.h>
#include <terminal/terminal.h>
#include <panic.h>

void init(void)
{
    __asm__ volatile("cli" ::: "memory");

    init_graphics();
    init_terminal();
    idt_init();
    pic_init();

    if (!keyboard_init())
        panic("PS/2 keyboard initialization failed");

    pic_unmask(1); /* unmask keyboard IRQ1 only */
    __asm__ volatile("sti" ::: "memory");

    printk("Initialization finished.\n");
}
