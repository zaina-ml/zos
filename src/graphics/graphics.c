#include <stdint.h>
#include <stddef.h>
#include "graphics.h"

#include <graphics/colors.h>
#include <graphics/font.h>

int validate_request(struct limine_framebuffer_request *request) 
{
    if (request->response == NULL)
        return 1;

    if (request->response->framebuffer_count == 0)
        return 1;

    return 0;
}


FrameBuffer init_buff(volatile struct limine_framebuffer_request *request)
{
    struct limine_framebuffer *fr = request->response->framebuffers[0];

    FrameBuffer fb;

    fb.address = fr->address;
    fb.width   = fr->width;
    fb.height  = fr->height;
    fb.pitch   = fr->pitch;
    fb.bpp     = fr->bpp;

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
        (base + y * fb->pitch + x * (fb->bpp / 8));

    *pixel = color;
}

void draw_rect(
    FrameBuffer fb, 
    uint64_t x,
    uint64_t y,
    uint64_t width, 
    uint64_t height, 
    uint32_t color
)
{ 
    for (uint64_t yp = y; yp < y + height; yp++) 
    {
        for (uint64_t xp = x; xp < x + width; xp++) 
        {
            draw_pixel(&fb, xp, yp, color); 
        } 
    } 
}

void cls(FrameBuffer fb)
{
    draw_rect(fb, 0, 0, fb.width, fb.height, BLACK);
}

void draw_menu(FrameBuffer fb) 
{
    cls(fb);

    draw_rect(fb, 0, 0, fb.width, 50, BLUE);
    _printk(fb, fb.width / 2, 25, "ZOS: ZainOS", WHITE);
}


void draw_glyph(
    FrameBuffer fb,
    uint32_t x,
    uint64_t y,
    unsigned char c,
    uint32_t color
)
{
    const uint8_t *glyph = fontmap[(unsigned char)c];

    for (int row = 0; row < 8; row++) 
    {
        uint8_t bits = glyph[row];

        for (int col = 0; col < 8; col++) 
        {
            if (bits & (1 << col)) 
            {
                draw_pixel(&fb, x + col, y + row, color);
            }
        }
    }
    
    
}

void _printk(
    FrameBuffer fb,
    uint32_t x,
    uint64_t y,
    char *str,
    uint32_t color
)
{

    while (*str != '\0') 
    {
        draw_glyph(fb, x, y, *str, WHITE);

        x+=8;
        str++;
    }
}

