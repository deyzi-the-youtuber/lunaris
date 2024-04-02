#include <lunaris/boot.h>
#include <lunaris/errno.h>
#include <multiboot.h>
#include <stdint.h>

int mboot_info_verify(struct mboot_info * info)
{
  return (info->magic == MULTIBOOT2_BOOTLOADER_MAGIC || !(info->addr & 7));
}
