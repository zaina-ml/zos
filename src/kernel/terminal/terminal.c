
#include <terminal/terminal.h>

#include <drivers/video/framebuffer.h>
#include <graphics/colors.h>
#include <graphics/graphics.h>

static const uint32_t INIT_TX = 10;
static const uint32_t INIT_TY = 60;
static const uint32_t TY_INC = 15;

static const uint32_t CHAR_WIDTH = 8;
static const uint32_t CHAR_HEIGHT = 8;

static uint32_t tx;
static uint32_t ty;

void init_terminal(void)
{
    tx = INIT_TX;
    ty = INIT_TY;
}

static void terminal_newline(void)
{
    tx = INIT_TX;
    ty += TY_INC;

    // Basic overflow handling, not scrolling yet.
    if ((uint64_t)ty + CHAR_HEIGHT > fb_get_height()) {
        uint64_t height = fb_get_height();

        if (height > 50)
            draw_rect(0, 50, fb_get_width(),
                      height - 50, BLACK);

        ty = INIT_TY;
    }
}

static void terminal_drawc_color(char c, uint32_t color)
{
    if (c == '\n') {
        terminal_newline();
        return;
    }

    if (c == '\r') {
        tx = INIT_TX;
        return;
    }

    if (c == '\b') {
        if (tx > INIT_TX) {
            tx -= CHAR_WIDTH;
            draw_rect(tx, ty,
                      CHAR_WIDTH, CHAR_HEIGHT, BLACK);
        }
        return;
    }

    if (c == '\t') {
        for (int i = 0; i < 4; i++)
            terminal_drawc_color(' ', color);
        return;
    }

    // Only printable ASCII for now.
    if ((unsigned char)c < 32 ||
        (unsigned char)c > 126)
        return;

    // Wrap when reaching the right edge.
    if ((uint64_t)tx + CHAR_WIDTH > fb_get_width())
        terminal_newline();

    draw_glyph(tx, ty, (unsigned char)c, color);
    tx += CHAR_WIDTH;
}

void terminal_drawchar(char c)
{
    terminal_drawc_color(c, WHITE);
}

void write_terminal(char *str, uint32_t color)
{
    if (!str)
        return;

    while (*str)
        terminal_drawc_color(*str++, color);
}
