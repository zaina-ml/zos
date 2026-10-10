#include <arch/x86_64/idt.h>
#include <arch/x86_64/pic.h>
#include <drivers/input/keyboard.h>
#include <panic.h>

#include <stdint.h>

struct __attribute__((packed)) idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_middle;
    uint32_t offset_high;
    uint32_t reserved;
};

struct __attribute__((packed)) idtr {
    uint16_t limit;
    uint64_t base;
};

/* GCC supplies the appropriate iretq epilogue for interrupt handlers. */
struct interrupt_frame {
    uint64_t rip, cs, rflags, rsp, ss;
};

static struct idt_entry idt[256];

__attribute__((interrupt))
static void fault_without_error(struct interrupt_frame *frame)
{
    (void)frame;
    panic("CPU exception (no error code)");
}

__attribute__((interrupt))
static void fault_with_error(struct interrupt_frame *frame, uint64_t error_code)
{
    (void)frame;
    (void)error_code;
    panic("CPU exception (error code)");
}

__attribute__((interrupt))
static void irq1_handler(struct interrupt_frame *frame)
{
    (void)frame;
    keyboard_handle_irq();
    pic_send_eoi(1);
}

static void idt_set_gate(uint8_t vector, uintptr_t handler, uint16_t cs)
{
    idt[vector] = (struct idt_entry) {
        .offset_low = (uint16_t)handler,
        .selector = cs,
        .ist = 0,
        .type_attr = 0x8E, /* present, ring 0, 64-bit interrupt gate */
        .offset_middle = (uint16_t)(handler >> 16),
        .offset_high = (uint32_t)(handler >> 32),
        .reserved = 0
    };
}

static int exception_has_error_code(uint8_t vector)
{
    switch (vector) {
        case 8:  /* double fault */
        case 10: /* invalid TSS */
        case 11: /* segment not present */
        case 12: /* stack segment */
        case 13: /* general protection */
        case 14: /* page fault */
        case 17: /* alignment check */
        case 21: /* control protection */
        case 29: /* VMM communication */
        case 30: /* security exception */
            return 1;
        default:
            return 0;
    }
}

void idt_init(void)
{
    uint16_t cs;
    __asm__ volatile("mov %%cs, %0" : "=r"(cs));

    /* Install basic fault handlers so common CPU exceptions don't triple fault. */
    for (uint8_t vector = 0; vector < 32; ++vector) {
        uintptr_t handler = exception_has_error_code(vector)
            ? (uintptr_t)fault_with_error
            : (uintptr_t)fault_without_error;
        idt_set_gate(vector, handler, cs);
    }

    /* PIC is remapped to 0x20, so keyboard IRQ1 becomes IDT vector 0x21. */
    idt_set_gate(0x21, (uintptr_t)irq1_handler, cs);

    struct idtr descriptor = {
        .limit = sizeof(idt) - 1,
        .base = (uintptr_t)idt
    };
    __asm__ volatile("lidt %0" : : "m"(descriptor) : "memory");
}
