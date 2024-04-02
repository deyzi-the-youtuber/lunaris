#include <sys/io.h>
#include <lunaris/serial.h>
#include <lunaris/video.h>
#include <lunaris/debug.h>
#include <lunaris/gdt.h>
#include <lunaris/idt.h>
#include <lunaris/timer.h>
#include <common.h>
#include <multiboot.h>
#include <system.h>
#include <lunaris/pci.h>
#include <lunaris/mm.h>
#include <lunaris/tss.h>
#include <lunaris/task.h>
#include <lunaris/sched.h>
#include <lunaris/blk/ide.h>
#include <lunaris/syscall.h>
#include <lunaris/cpu.h>
#include <system.h>
#include <lunaris/printk.h>

//extern MULTIBOOT_INFO * mbi;

void init_mem()
{
  paging_init();
  InitializeMemoryManagement(&kernel_end);
}

void arch_init(void)
{
  /* i386 specific things */
  InitializeGDT();
  InitializeIDT();
  InitializeTimer();
  install_exception_handlers();
}

int InitializeHALStage1(void)
{
  /*
    We're almost getting ready for the i/o debugging console.
    We just need a PIT, TSS, and a Multitasking handler.
    These will be implemented in stage 2
  */
  arch_init();
  if (!SerialInitialize())
  {
    printk("warning: could not initialize serial device\n");
  }
  init_mem();
  pci_init();
  /* Goodbye to tasking with a timer :) */
  EnableSystemCalls();
  SetGateIDT(0x81, (uint32_t)preempt, 0x8, 0x8E);
  initTasking();
  return 0;
}
