/*
  Lunaris kernel "vga.c"
  Jan 28, 2024

  DESCRIPTION:
    [+] Generic VGA BIOS graphics driver.
    [+] Usable on x86_64 systems (without UEFI).
    [+] Incompatible with UEFI.
*/

// Rewritten on April 1st, 2024 

#include <lunaris/video.h>
#include <sys/io.h>
#include <common.h>
#include <stdint.h>
#include <lunaris/module.h>

MODULE("vga-generic")

void __vga_generic_write(uint8_t ch);
void __vga_generic_remap(int x, int y);

struct video_ops vga_generic_ops = {
  .write = __vga_generic_write,
  .remap = __vga_generic_remap
};

struct video vga_generic = {
  .framebuffer = (uintptr_t)0xb8000,
  .width  = 80,
  .height = 25,
  .bpp    = 16,
  .x      = 0,
  .y      = 0,
  .video_ops = &vga_generic_ops
};

static void __vga_generic_scroll(void)
{
  // Check if the cursor is at the last row of the VGA buffer
  if (vga_generic.y >= vga_generic.height)
  {
      // Move all rows up by one (excluding the first row)
    for (int i = 1; i < vga_generic.height; i++)
    {
      memcpy((void *)vga_generic.framebuffer + (i - 1) * vga_generic.width * 2, (void *)vga_generic.framebuffer + i * vga_generic.width * 2, vga_generic.width * 2);
    }

    // Clear the last row
    char * last_row = (void *)vga_generic.framebuffer + 24 * vga_generic.width * 2;
    memset(last_row, 0, vga_generic.width * 2);

    // Move the cursor up by one row
    vga_generic.y--;
  }
}

void __vga_generic_write(uint8_t ch)
{
  uint16_t position = vga_generic.y * vga_generic.width + vga_generic.x;
  uint16_t * vmem = (uint16_t *)(vga_generic.framebuffer + position * 2);
  switch(ch)
  {
    case '\n':
    {
      vga_generic.x = 0;
      vga_generic.y++;
      goto remap;
      break;
    }
    default:
    {
      *vmem = (uint16_t)ch | (0x7 << 0x8);
    }
  }
  vga_generic.x++;
remap:;
  vga_generic.video_ops->remap(vga_generic.x, vga_generic.y);
}

void __vga_generic_remap(int x, int y)
{
  uint16_t position = y * vga_generic.width + x;
  outb(0x3D4, 0xf);
  outb(0x3D5, (uint8_t)(position & 0xff));
  outb(0x3D4, 0xe);
  outb(0x3D5, (uint8_t)(position >> 8) & 0xff);
  __vga_generic_scroll();
}

struct video * vga_generic_init(void)
{
  return &vga_generic;
}
