#include <kernel/mm/paging.h>
#include <kernel/video/vga.h>
#include <kernel/sys/io.h>
#include <kernel/isr.h>
#include <kernel/debug.h>
#include <kernel/task.h>
#include <kernel/signal.h>
#include <common.h>
#include <kernel/printk.h>

#define INDEX(virt) virt >> 22

#define PAGE_PRESENT 0x1
#define PAGE_WRITE 0x2
#define PAGE_USER 0x4

__attribute__((aligned(PAGE_SIZE))) uint32_t page_directory[1024];
__attribute__((aligned(PAGE_SIZE))) uint32_t exec_page_table[1024];
static uint32_t page_dir_loc;
static uint32_t *current_directory;
static uint32_t *last_page;

static void invalidate(int vaddr)
{
	asm volatile("invlpg %0" ::"m"(vaddr));
}

void MapVirtualToPhys(uint32_t virt, uint32_t phys)
{
	uint16_t index = INDEX(virt);
	for (int i = 0; i < 1024; i++)
	{
		last_page[i] = phys | PAGE_PRESENT | PAGE_USER | PAGE_WRITE;
		phys += 4096;
	}
	page_directory[index] = ((uint32_t)last_page) | PAGE_PRESENT | PAGE_USER | PAGE_WRITE;
	last_page = (uint32_t *)(((uint32_t)last_page) + 4096);
}

void PageFaultHandler(REGISTERS * regs)
{
	/* By "core dumped", I dont mean dumped to a file, I mean dumped to the debug console :) */
	Process * p = getCurrentProcess();
	printk("%s: segmentation fault at eip 0x%x esp 0x%x error %d\n", p->name, regs->eip, regs->esp, regs->err_code);
	uint32_t fault;
	asm volatile("mov %%cr2, %0" : "=r" (fault));
	DebugOutput("Fault address: 0x%x\n", fault);
  int present   = !(regs->err_code & 0x1); // Page not present
  int rw = regs->err_code & 0x2;           // Write operation?
  int us = regs->err_code & 0x4;           // Processor was in user-mode?
  int reserved = regs->err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
  int id = regs->err_code & 0x10;          // Caused by an instruction fetch?
	if (present)
	{
		DebugOutput("Page not present\n");
	}
  if (rw)
	{
		DebugOutput("Write operation on a read-only page\n");
	}
  if (us) 
	{
		DebugOutput("User-Mode\n");
	}
  if (reserved) 
	{
		DebugOutput("Reserved\n");
	}
	notify(SIGSEGV);
}

void InitializePaging()
{
	page_directory[0] = 0;
	invalidate(0);
	page_directory[0] = (uint32_t)0x400000;
	page_dir_loc = (uint32_t)page_directory;
	last_page = (uint32_t *)0x404000;
	for (int i = 0; i < 1024; i += 0x1000)
	{
		page_directory[i] = ((uint32_t)last_page) | PAGE_PRESENT | PAGE_USER | PAGE_WRITE;
	}
	
	MapVirtualToPhys(0, 0);
	MapVirtualToPhys(0x400000, 0x400000);
	RegisterInterrupt(14, &PageFaultHandler);
	LoadPageDirectory((uint32_t *)page_dir_loc);
}