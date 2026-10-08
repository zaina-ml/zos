#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>

void init_terminal(void);
void write_terminal(char *str, uint32_t color);

#endif
