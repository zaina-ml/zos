#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <limine/limine.h>

typedef struct
{
    void *address;
    uint64_t width;
    uint64_t height;
    uint64_t pitch;
    uint16_t bpp;
} FrameBuffer;

int validate_request(struct limine_framebuffer_request *request);

FrameBuffer init_buff(
    volatile struct limine_framebuffer_request *request
);

void draw_pixel(
    FrameBuffer *fb,
    uint64_t x,
    uint64_t y,
    uint32_t color
);

void draw_rect(
    FrameBuffer fb,
    uint64_t x,
    uint64_t y,
    uint64_t width,
    uint64_t height,
    uint32_t color
);

void cls(FrameBuffer fb);
void draw_menu(FrameBuffer fb);

void draw_glyph(
    FrameBuffer fb,
    uint32_t x,
    uint64_t y,
    unsigned char c,
    uint32_t color
);

void _printk(
    FrameBuffer fb,
    uint32_t x,
    uint64_t y,
    char *str,
    uint32_t color
);

#endif