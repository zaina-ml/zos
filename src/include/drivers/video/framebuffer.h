#ifndef DRIVERS_VIDEO_FRAMEBUFFER_H
#define DRIVERS_VIDEO_FRAMEBUFFER_H

#include <stdint.h>

int init_framebuffer(void);
uint64_t fb_get_width(void);
uint64_t fb_get_height(void);
void fb_draw_pixel(uint64_t x, uint64_t y, uint32_t color);

#endif
