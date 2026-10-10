#include <arch/x86_64/pic.h>
#include <arch/x86_64/io.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1
#define PIC_EOI      0x20

void pic_init(void)
{
    /* ICWs: initialize both 8259 PICs and remap them past CPU exceptions. */
    outb(PIC1_COMMAND, 0x11); io_wait();
    outb(PIC2_COMMAND, 0x11); io_wait();
    outb(PIC1_DATA, 0x20); io_wait(); /* master vectors 0x20..0x27 */
    outb(PIC2_DATA, 0x28); io_wait(); /* slave vectors 0x28..0x2F */
    outb(PIC1_DATA, 0x04); io_wait(); /* slave on master's IRQ2 */
    outb(PIC2_DATA, 0x02); io_wait();
    outb(PIC1_DATA, 0x01); io_wait(); /* 8086/88 mode */
    outb(PIC2_DATA, 0x01); io_wait();

    /* All hardware IRQs initially masked until their handlers exist. */
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
}

void pic_unmask(uint8_t irq)
{
    if (irq >= 16)
        return;

    uint16_t port = irq < 8 ? PIC1_DATA : PIC2_DATA;
    uint8_t bit = irq & 7;
    outb(port, inb(port) & (uint8_t)~(1u << bit));

    if (irq >= 8)
        outb(PIC1_DATA, inb(PIC1_DATA) & (uint8_t)~(1u << 2));
}

void pic_send_eoi(uint8_t irq)
{
    if (irq >= 8)
        outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}
