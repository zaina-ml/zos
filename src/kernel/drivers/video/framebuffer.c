#include <drivers/video/framebuffer.h>

#include <stddef.h>
#include <drivers/video/limine.h>

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t start_marker[4] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests")))
static volatile uint64_t base_revision[3] = LIMINE_BASE_REVISION(6);

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request fb_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0,
    .response = NULL
};

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t end_marker[2] = LIMINE_REQUESTS_END_MARKER;

typedef struct
{
    void *address;
    uint64_t width;
    uint64_t height;
    uint64_t pitch;
    uint16_t bpp;
} Fb;

static Fb fb;

int init_framebuffer(void)
{
    volatile struct limine_framebuffer_request *request = &fb_request;
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

    fb.address = fr->address;
    fb.width = fr->width;
    fb.height = fr->height;
    fb.pitch = fr->pitch;
    fb.bpp = fr->bpp;

    return 0;
}

uint64_t fb_get_width(void)
{
    return fb.width;
}

uint64_t fb_get_height(void)
{
    return fb.height;
}

void fb_draw_pixel(uint64_t x, uint64_t y, uint32_t color)
{
    if (x >= fb.width || y >= fb.height)
        return;

    uint8_t *base = (uint8_t *)fb.address;
    volatile uint32_t *pixel =
        (volatile uint32_t *)
        (base + y * fb.pitch + x * (fb.bpp / 8));

    *pixel = color;
}
