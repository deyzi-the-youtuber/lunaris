/*
  Lunaris kernel "gop.c"
  Jan 31, 2024

  DESCRIPTION:
    [+] Generic UEFI graphics driver.
    [+] Incompatible with legacy BIOS.
*/

#include <stdint.h>
#include <kernel/video/gop.h>
#include <kernel/font/vga.h>

struct uefi_gop gop_display;
uint8_t * fb_addr;

void gop_putpx(int x, int y, int r, int g, int b)
{
  uint32_t offset =
      (x + y * gop_display.width) *
      4; // Finding the location of the pixel in the video array

  fb_addr[offset] = b; // Setting the color using BGR
  fb_addr[offset + 1] = g;
  fb_addr[offset + 2] = r;
  fb_addr[offset + 3] = 0;
}

static inline void set_bg(int r, int g, int b)
{
  for(int y = 0; y < 768; y++)
  {
    for (int x = 0; x < 1024; x++)
      gop_putpx(x, y, r, g, b);
  }
}

void uefi_gop_init(struct multiboot_tag_framebuffer_common * tag)
{
  gop_display.fb_addr = tag->framebuffer_addr;
  gop_display.width = tag->framebuffer_width;
  gop_display.height = tag->framebuffer_height;
  gop_display.pitch = tag->framebuffer_pitch;
  gop_display.bpp = tag->framebuffer_bpp;
  gop_display.type = tag->framebuffer_type;

  fb_addr = (uint8_t *)gop_display.fb_addr;
  set_bg(110, 95, 150);
  set_bg(150, 95, 95);
}