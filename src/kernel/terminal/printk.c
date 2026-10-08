#include <terminal/printk.h>
#include <terminal/terminal.h>
#include <graphics/colors.h>

void printk(char *str)
{
    write_terminal(str, WHITE);
}