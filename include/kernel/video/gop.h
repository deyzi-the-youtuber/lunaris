#include <multiboot.h>
#include <stdint.h>

#ifndef __GOP_H
#define __GOP_H

struct uefi_gop
{
  uint32_t fb_addr;
  uint32_t width;
  uint32_t height;
  uint32_t pitch;
  uint8_t bpp;
  uint8_t type;
};

void uefi_gop_init(struct multiboot_tag_framebuffer_common * tag);

#endif