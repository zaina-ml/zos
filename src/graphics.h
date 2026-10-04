#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <limine.h>

typedef struct
{
    void *address;
    uint64_t width;
    uint64_t height;
    uint64_t pitch;
    uint16_t bpp;
} FrameBuffer;

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

#endif

