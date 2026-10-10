#include <graphics/graphics.h>

#include <init.h>
#include <terminal/printk.h>
#include <terminal/terminal.h>

#include <panic.h>
#include <drivers/input/keyboard.h>

#include <arch/x86_64/io.h>

__attribute__((noreturn))

void kmain(void)
{
    init();

    // Disable interrupt-driven keyboard handling
    __asm__ volatile("cli" ::: "memory");

    // Mask IRQ1 on the master PIC
    outb(0x21, inb(0x21) | 0x02);

    printk("PS/2 raw scancode test:\n");

    const char hex[] = "0123456789ABCDEF";

    for (;;) {
        // Does the controller have a byte ready?
        if (inb(0x64) & 0x01) {
            uint8_t code = inb(0x60);

            terminal_drawchar(hex[code >> 4]);
            terminal_drawchar(hex[code & 0x0F]);
            terminal_drawchar(' ');
        }
    }
}
