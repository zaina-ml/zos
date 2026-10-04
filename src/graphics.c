#include "graphics.h"

FrameBuffer init_buff(
    volatile struct limine_framebuffer_request *request
)
{
    struct limine_framebuffer *limine_fb =
        request->response->framebuffers[0];

    FrameBuffer fb;

    fb.address = limine_fb->address;
    fb.width   = limine_fb->width;
    fb.height  = limine_fb->height;
    fb.pitch   = limine_fb->pitch;
    fb.bpp     = limine_fb->bpp;

    return fb;
}

void draw_pixel(
    FrameBuffer *fb,
    uint64_t x,
    uint64_t y,
    uint32_t color
)
{
    if (x >= fb->width || y >= fb->height)
        return;

    uint8_t *base = (uint8_t *)fb->address;

    volatile uint32_t *pixel =
        (volatile uint32_t *)
        (base + y * fb->pitch + x * 4);

    *pixel = color;
}

void draw_rect( FrameBuffer fb, uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint32_t color ) { 
    for (uint64_t yp = y; yp < y + height; yp++) {
        for (uint64_t xp = x; xp < x + width; xp++) {
            draw_pixel(&fb, xp, yp, color); 
        } 
    } 
}
