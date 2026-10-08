#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>

void draw_rect(
    uint64_t x,
    uint64_t y,
    uint64_t width,
    uint64_t height,
    uint32_t color
);

void cls(void);
void draw_menu(void);
void draw_panic(char *message);

void draw_glyph(
    uint32_t x,
    uint64_t y,
    unsigned char c,
    uint32_t color
);

void draw_chars(
    uint32_t x,
    uint64_t y,
    char *str,
    uint32_t color
);

void init_graphics(void);

#endif