#ifndef ARCH_X86_64_PIC_H
#define ARCH_X86_64_PIC_H

#include <stdint.h>

void pic_init(void);
void pic_unmask(uint8_t irq);
void pic_send_eoi(uint8_t irq);

#endif
