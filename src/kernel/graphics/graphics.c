#include <stdint.h>
#include <stddef.h>

#include <graphics/graphics.h>
#include <graphics/colors.h>
#include <graphics/font.h>

#include <limine/limine.h>
#include <limine/request.h>
#include <utils/string.h>

typedef struct
{
    void *address;
    uint64_t width;
    uint64_t height;
    uint64_t pitch;
    uint16_t bpp;
} FrameBuffer;

static FrameBuffer framebuffer;

static int init_framebuffer(volatile struct limine_framebuffer_request *request)
{
    if (request == NULL || request->response == NULL ||
        request->response->framebuffer_count == 0 ||
        request->response->framebuffers == NULL ||
        request->response->framebuffers[0] == NULL)
    {
        return 1;
    }

    struct limine_framebuffer *fr = request->response->framebuffers[0];

    if (fr->address == NULL)
    {
        return 1;
    }

    framebuffer.address = fr->address;
    framebuffer.width = fr->width;
    framebuffer.height = fr->height;
    framebuffer.pitch = fr->pitch;
    framebuffer.bpp = fr->bpp;

    return 0;
}

void init_graphics(void)
{
    init_framebuffer(get_framebuffer_request());
    draw_menu();
}

void draw_pixel(uint64_t x, uint64_t y, uint32_t color)
{
    if (x >= framebuffer.width || y >= framebuffer.height)
        return;

    uint8_t *base = (uint8_t *)framebuffer.address;

    volatile uint32_t *pixel =
        (volatile uint32_t *)
        (base + y * framebuffer.pitch + x * (framebuffer.bpp / 8));

    *pixel = color;
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
            draw_pixel(xp, yp, color); 
        } 
    } 
}

void cls(void)
{
    draw_rect(0, 0, framebuffer.width, framebuffer.height, BLACK);
}

void draw_menu(void) 
{
    char *title = "zOS";

    draw_rect(0, 0, framebuffer.width, 50, BLUE);
    draw_chars(cntrstring(framebuffer.width, title), 25, title, WHITE);
}

void draw_panic(char *message) 
{
    char *title = "Kernel Panic :(";

    draw_rect(
        0,
        (framebuffer.height - 100) / 2,
        framebuffer.width,
        100,
        BLUE
    );

    draw_chars(cntrstring(framebuffer.width, title), (framebuffer.height - 50) / 2, title, WHITE);
    draw_chars(cntrstring(framebuffer.width, message), (framebuffer.height + 10) / 2, message, WHITE);
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
                draw_pixel(x + col, y + row, color);
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
