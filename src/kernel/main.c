#include <kernel/config.h>
#include <stdint.h>
#include <common.h>
#include <kernel/video/vga.h>
#include <kernel/hal.h>
#include <kernel/cpu.h>
#include <kernel/sys/io.h>
#include <kernel/timer.h>
#include <multiboot.h>
#include <system.h>
#include <kernel/debug.h>
#include <kernel/mm/paging.h>
#include <kernel/mm/malloc.h>
#include <kernel/task.h>
#include <kernel/blk/mbr.h>
#include <kernel/blk/ide.h>
#include <kernel/sys/utsname.h>
#include <kernel/sys/tty.h>
#include <kernel/sys/serial.h>
#include <kernel/fs/devfs.h>
#include <kernel/kbd.h>
#include <kernel/syscall.h>
#include <kernel/tss.h>
#include <kernel/acpi.h>
#include <kernel/apic.h>
#include <kernel/trace.h>
#include <kernel/fcntl.h>
#include <kernel/printk.h>
#include <kernel/fs/ext2.h>
#include <kernel/fs/devfs.h>
#include <kernel/sound/pcspk.h>
#include <kernel/firmware/smbios.h>

struct multiboot_header_tag * tag;

/* Will be used later for tasking */
pid_t pid;
/* For getting multiboot info */
SysInfo kinfo;
char * VendorString;
extern void syscall_test();
uintptr_t _mbi_addr;

static char * get_cmdline(struct multiboot_tag_string * tag, uintptr_t address)
{
  tag = (struct multiboot_header_tag *)(address + 8);
  while(tag->type != MULTIBOOT_TAG_TYPE_END)
  {
    switch (tag->type)
    {
      case MULTIBOOT_TAG_TYPE_CMDLINE:
        return (char *)tag->string;
        break;
    
      default:
        break;
    }
    tag = (struct multiboot_header_tag *) ((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7));
  }
}

static void register_devices(void)
{
  DebugOutput("[KERNEL] Registering devices...\n");
  devfs_create_device(VFS_FILE, true, false, "timer", 0, 0, DEVFS_ROOT_DIR);
  devfs_create_device(VFS_BLOCK_DEVICE, true, true, "sda", 0, 0, DEVFS_ROOT_DIR);
  int partition_num = get_num_partitions(0);
  for (int i = 1; i < partition_num; i++)
  {
    char num[3];
    char * name = strcat("sda", itoa(i, num, 10));
    devfs_create_device(VFS_BLOCK_DEVICE, true, true, name, 0, 0, DEVFS_ROOT_DIR);
  }
  devfs_create_device(VFS_FILE, true, true, "stdout", 0, 0, DEVFS_ROOT_DIR);
  devfs_create_device(VFS_FILE, true, true, "stdin", 0, 0, DEVFS_ROOT_DIR);
  devfs_create_device(VFS_FILE, true, true, "stderr", 0, 0, DEVFS_ROOT_DIR);
  devfs_create_device(VFS_CHARACTER_DEVICE, true, false, "ttyS0", SerialRead, SerialWrite, DEVFS_ROOT_DIR);
}

static int is_usb_boot()
{
  uint8_t sect[512];
  readSector(0, 0, sect);
  return sect[1] == 0xff;
}

static void bss_init()
{
  memset(&bss_start, 0, &bss_end - &bss_end);
}

void KernelMain(uint32_t magic, uintptr_t address)
{
  _mbi_addr = address;
  VideoClearVGA();
  bss_init();
  uint32_t kernel_size = kernel_base - kernel_end;
  /* Multiboot check */
  if (magic != MULTIBOOT2_BOOTLOADER_MAGIC || address & 7)
  {
    return;
  }

  uint32_t mbi_size = *(uint32_t *)address;
  char * cmdline = get_cmdline(tag, address);
  printk("%s version %s-%s (%s version %s)\n", UTSNAME_SYSNAME, UTSNAME_RELEASE, UTSNAME_MACHINE, __CONFIG_COMPILER__, __CONFIG_COMPILER_VERSION__);
  printk("kernel: cmdline: %s\n", cmdline);
  parse_cmdline(cmdline, &kinfo);
  printk("cpu: getting cpu info...\n");
  if (!check_cpuid())
  {
    printk("cpu: cpuid is not supported\n");
    printk("cpu: some features may be disabled\n");
  }
  else
  {
    printk("cpu: cpuid is supported\n");
    char brand[255];
    arch_get_brand_string(brand);
    printk("cpu: %s\n", brand);
  }
  InitializeHALStage1();
  PANIC("reached end of kernel stage 1");
}

void KernelMainStage2()
{
  smbios_init();
#ifdef CONFIG_ACPI
  //acpi_init();
#endif
  if (!is_usb_boot())
  {
    printk("ata: drive has %d partitions\n", get_num_partitions(0));
  }
  else
  {
    printk("kernel was booted from USB or other unknown device\n");
  }
  devfs_init(true, true);
  int fp = getFirstPartition(0);
  int mount = ext2_mount(fp);
  if (mount < 0)
  {
    PANIC("unable to mount rootfs\n");
  }
  register_devices();
  tty_init();
  keyboard_init();
  /* prepare for user-mode */
  SetKernelStack(getCurrentProcess()->stack + 0x100000);
  /* at this point, we are ready for user mode. switch to it */
  DebugOutput("[KERNEL] Kernel is switching to user-mode. Goodbye kernel-mode!\n");
  printk("executing /init\n");
  /* switch to user mode and execute a program */
  move_to_usermode();
  for(;;);
}