#include <stdint.h>
#include <stddef.h>

#include <limine.h>

#include <graphics.h>


__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t start_marker[4] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests")))
static volatile  uint64_t base_revision[3] = LIMINE_BASE_REVISION(6);

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request fr = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0,
    .response = NULL
};

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t end_marker[2] = LIMINE_REQUESTS_END_MARKER;


__attribute__((noreturn))
void kmain(void)
{
    if (fr.response == NULL)
        for (;;) {}

    if (fr.response->framebuffer_count == 0)
        for (;;) {}

    FrameBuffer fb = init_buff(&fr);

    draw_rect(fb, 0, 0, 100, 300, 0x00FF0000);

    for (;;) {}
}