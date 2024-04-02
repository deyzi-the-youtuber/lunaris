#include <stdint.h>

#ifndef __BOOT_H
#define __BOOT_H

struct mboot_info
{
  uint32_t magic;
  uintptr_t addr;
};

int mboot_info_verify(struct mboot_info * info);

#endif
