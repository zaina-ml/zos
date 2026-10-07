#include <stdint.h>
#include <stddef.h>

#include <limine/limine.h>
#include <limine/request.h>

#include <graphics/colors.h>
#include <graphics/graphics.h>
#include <graphics/font.h>


__attribute__((noreturn))
void kmain(void)
{
    struct limine_framebuffer_request *fr = get_framebuffer_request();

    if (validate_request(fr)) 
    {
        for (;;) {} // add panic
    }

    FrameBuffer fb = init_buff(fr);

    draw_menu(fb);

    _printk(fb, 100, 100, "Hello World!", WHITE);

    for (;;) {}
}