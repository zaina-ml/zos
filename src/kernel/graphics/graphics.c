#include <stdint.h>
#include <stddef.h>

#include <graphics/graphics.h>
#include <graphics/colors.h>
#include <graphics/font.h>

#include <drivers/video/framebuffer.h>
#include <lib/string.h>

void init_graphics(void)
{
    init_framebuffer();
    draw_menu();
}

void draw_rect(
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
            fb_draw_pixel(xp, yp, color);
        } 
    } 
}

void cls(void)
{
    draw_rect(0, 0, fb_get_width(), fb_get_height(), BLACK);
}

void draw_menu(void) 
{
    char *title = "zOS";

    draw_rect(0, 0, fb_get_width(), 50, BLUE);
    draw_chars(cntrstring(fb_get_width(), title), 25, title, WHITE);
}

void draw_panic(char *message) 
{
    char *title = "Kernel Panic :(";

    draw_rect(
        0,
        (fb_get_height() - 100) / 2,
        fb_get_width(),
        100,
        BLUE
    );

    draw_chars(cntrstring(fb_get_width(), title), (fb_get_height() - 50) / 2, title, WHITE);
    draw_chars(cntrstring(fb_get_width(), message), (fb_get_height() + 10) / 2, message, WHITE);
}


void draw_glyph(
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
                fb_draw_pixel(x + col, y + row, color);
            }
        }
    }
    
    
}

void draw_chars(
    uint32_t x,
    uint64_t y,
    char *str,
    uint32_t color
)
{

    while (*str != '\0') 
    {
        draw_glyph(x, y, *str, color);

        x+=8;
        str++;
    }
}
