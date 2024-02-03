#include <kernel/sys/io.h>
#include <kernel/sys/serial.h>
#include <kernel/video/vga.h>
#include <kernel/debug.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/timer.h>
#include <common.h>
#include <multiboot.h>
#include <system.h>
#include <kernel/mm/paging.h>
#include <kernel/pci.h>
#include <kernel/mm/malloc.h>
#include <kernel/tss.h>
#include <kernel/task.h>
#include <kernel/sched.h>
#include <kernel/blk/ide.h>
#include <kernel/syscall.h>
#include <kernel/cpu.h>
#include <kernel/bios32.h>
#include <system.h>
#include <kernel/printk.h>

//extern MULTIBOOT_INFO * mbi;

void init_mem()
{
  InitializePaging();
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
  if (SerialInitialize())
  {
    printk("warning: could not initialize serial device\n");
  }
  init_mem();
  DumpPCI();
  /* Goodbye to tasking with a timer :) */
  EnableSystemCalls();
  SetGateIDT(0x81, (uint32_t)preempt, 0x8, 0x8E);
  initTasking();
  return 0;
}