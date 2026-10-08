#ifndef STDOUT_H
#define STDOUT_H

#include <stdint.h>

void init_stdout(void);
void stdout_write(char *str, uint32_t color);

#endif
