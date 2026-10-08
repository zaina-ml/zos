#include <char/stdout.h>

#include <graphics/graphics.h>
#include <utils/string.h>

static const uint32_t INIT_TX = 10;
static const uint32_t INIT_TY = 60;
static const uint32_t TY_INC = 15;

static uint32_t ty;
static uint32_t tx;

void init_stdout(void)
{
    ty = INIT_TY;
    tx = INIT_TX;
}

void stdout_write(char *str, uint32_t color)
{
    int length = strlen(str);
    
    if (length == 0)
        return;

    draw_chars(tx, ty, str, color);

    if (str[length - 1] == '\n')
    {
        ty += TY_INC;
        tx = INIT_TX;
    }
    else
    {
        tx += length * 8;
    }
}