#include <lunaris/config.h>
#include <stdint.h>
#include <common.h>
#include <lunaris/video.h>
#include <lunaris/hal.h>
#include <lunaris/cpu.h>
#include <sys/io.h>
#include <lunaris/timer.h>
#include <multiboot.h>
#include <system.h>
#include <lunaris/debug.h>
#include <lunaris/mm.h>
#include <lunaris/task.h>
#include <lunaris/blk/mbr.h>
#include <lunaris/blk/ide.h>
#include <sys/utsname.h>
#include <sys/tty.h>
#include <lunaris/serial.h>
#include <fs/devfs.h>
#include <lunaris/kbd.h>
#include <lunaris/syscall.h>
#include <lunaris/tss.h>
#include <lunaris/acpi.h>
#include <lunaris/apic.h>
#include <lunaris/trace.h>
#include <lunaris/fcntl.h>
#include <lunaris/printk.h>
#include <fs/ext2.h>
#include <fs/devfs.h>
#include <lunaris/firmware/smbios.h>
#include <lunaris/boot.h>
#include <lunaris/video.h>

struct multiboot_header_tag * tag;
char * VendorString;
uintptr_t _mbi_addr;

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
  //devfs_create_device(VFS_CHARACTER_DEVICE, true, false, "ttyS0", SerialRead, SerialWrite, DEVFS_ROOT_DIR);
}

static void bss_init()
{
  memset(&bss_start, 0, &bss_end - &bss_end);
}

void KernelMain(struct mboot_info info)
{
  _mbi_addr = info.addr;
  video_init();
  bss_init();
  /* Multiboot check */
  if(!mboot_info_verify(&info))
  {
    printk("%d: invalid boot info: 0x%x, 0x%x", __LINE__, info.magic, info.addr);
    return;
  }

  uint32_t mbi_size = *(uint32_t *)info.addr;
  char * cmdline = get_cmdline(tag, info.addr);
  printk("%s version %s-%s (%s version %s)\n", UTSNAME_SYSNAME, UTSNAME_RELEASE, UTSNAME_MACHINE, __CONFIG_COMPILER__, __CONFIG_COMPILER_VERSION__);
  printk("kernel: cmdline: %s\n", cmdline);
  parse_cmdline(cmdline);
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
    cpu_get_brand_string(brand);
    printk("cpu: %s\n", brand);
  }
  InitializeHALStage1();
  PANIC("reached end of kernel stage 1");
}

extern void start_init();

void KernelMainStage2()
{
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
  SetKernelStack(getCurrentProcess()->context.esp + 0x100000);
  /* at this point, we are ready for user mode. switch to it */
  DebugOutput("[KERNEL] Kernel is switching to user-mode. Goodbye kernel-mode!\n");
  printk("executing /init\n");
  /* switch to user mode and execute a program */
  move_to_usermode();
  for(;;);
}
