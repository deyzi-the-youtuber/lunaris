#include <stdint.h>

#ifndef __VIDEO_H
#define __VIDEO_H

struct video_ops
{
  void (*write)(uint8_t);
  void (*remap)(int, int);
};

struct video
{
  uintptr_t framebuffer;
  uint32_t  width;
  uint32_t  height;
  uint8_t   bpp;
  int       x, y;
  struct video_ops * video_ops;
};

void video_init(void);
void video_putc(const char c);
void video_puts(const char fmt[]);
void video_remap(int x, int y);
int video_getx(void);
int video_gety(void);
struct video * video_get_driver(void);

struct video * vga_generic_init(void);

#endif
