#include <lunaris/video.h>
#include <stdint.h>
#include <common.h>

struct video * display_driver = {0};

void video_init(void)
{
  display_driver = vga_generic_init();
}

void video_putc(const char c)
{
  display_driver->video_ops->write(c);
}

void video_puts(const char fmt[])
{
  for(int i = 0; fmt[i]; i++)
    video_putc(fmt[i]);
}

int video_getx(void)
{
  return display_driver->x;
}

int video_gety(void)
{
  return display_driver->y;
}

struct video * video_get_driver(void)
{
  return display_driver;
}

void video_remap(int x, int y)
{
  display_driver->video_ops->remap(x, y);
}
