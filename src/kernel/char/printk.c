#include <char/printk.h>
#include <char/stdout.h>
#include <graphics/colors.h>

void printk(char *str)
{
    stdout_write(str, WHITE);
}