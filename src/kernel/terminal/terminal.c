#include <stddef.h>
#include <terminal/terminal.h>

#include <graphics/graphics.h>
#include <lib/string.h>

static const uint32_t INIT_TX = 10;
static const uint32_t INIT_TY = 60;
static const uint32_t TY_INC = 15;

static uint32_t ty;
static uint32_t tx;

void init_terminal(void)
{
    ty = INIT_TY;
    tx = INIT_TX;
}

void write_terminal(char *str, uint32_t color) {
    if (str == NULL || *str == '\0') {
        return;
    }

    int current_line_len = 0;
    char *start_ptr = str;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {
            if (current_line_len > 0) {
                str[i] = '\0';
                draw_chars(tx, ty, start_ptr, color);
                str[i] = '\n';
                
                tx += current_line_len * 8;
            }

            ty += TY_INC;
            tx = INIT_TX;

            start_ptr = &str[i + 1];
            current_line_len = 0;
        } else {
            current_line_len++;
        }
    }
    
    if (current_line_len > 0) {
        draw_chars(tx, ty, start_ptr, color);
        tx += current_line_len * 8;
    }
}
