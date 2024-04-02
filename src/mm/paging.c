#include <lunaris/mm.h>
#include <sys/io.h>
#include <lunaris/isr.h>
#include <lunaris/debug.h>
#include <lunaris/task.h>
#include <lunaris/signal.h>
#include <common.h>
#include <lunaris/printk.h>

// 4/1/24: Kernel paging rewrite
// I feel like the paging code before was super messy.
// Because of that, I'm gonna make it cleaner.

static struct page_directory page_dir[1024] __attribute__((aligned(PAGE_SIZE)));
static struct page_table page_tables[1024] __attribute__((aligned(PAGE_SIZE)));

static void __page_irq(REGISTERS * regs)
{  
  uint32_t cr2;
  int present = !(regs->err_code & 0x1);
  int rw = regs->err_code & 0x2;
  int us = regs->err_code & 0x4;
  int reserved = regs->err_code & 0x8;
  int id = regs->err_code & 0x10;
  DebugOutput("[PAGING] Received page fault! Reason: ");
  asm volatile("mov %cr2, %eax");
  asm volatile("mov %%eax, %0" :: "m"(cr2));
	if (present)
		DebugOutput("Page not present\n");
  if (rw)
		DebugOutput("Write operation on a read-only page\n");
  if (us) 
		DebugOutput("User-Mode\n");
  if (reserved) 
		DebugOutput("Reserved\n");
  DebugOutput("[PAGING] Page fault error: 0x%x\n", cr2);
  notify(SIGSEGV);
}

static void load_page_directory(struct page_directory * pd)
{
  uint32_t cr0;
  asm volatile("mov %0, %%cr3" ::"r"((uint32_t)pd));
  // set bit in cr0 to enable paging
  asm volatile("mov %%cr0, %0": "=r"(cr0));
  cr0 = cr0 | 0x80000001;
  asm volatile("mov %0, %%cr0" ::"r"(cr0));
}

void __tlb_flush(void)
{
  asm volatile("movl %%cr3, %%eax; movl %%eax, %%cr3" ::: "eax", "memory");
}

void paging_init(void)
{
  int i;
  memset(page_dir, 0, sizeof(struct page_directory) * 1024);
  memset(page_tables, 0, sizeof(struct page_table) * 1024);
  for(i = 0; i < 1024; i++)
  {
    page_dir[i].read_write = 1;
    page_dir[i].user_mode = 1;
  }
  for(i = 0; i < 1024; i++)
  {
    page_tables[i].present = 1;
    page_tables[i].read_write = 1;
    page_tables[i].user_mode = 1;
    page_tables[i].frame = (i * PAGE_SIZE) >> 12;
  }
  page_dir[0].present = 1;
  page_dir[0].accessed = 0;
  page_dir[0].user_mode = 1;
  page_dir[0].frame = ((uint32_t)page_tables >> 12);

  load_page_directory(page_dir);
  RegisterInterrupt(14, __page_irq);
}

void paging_map_page(void * physaddr, void * virtualaddr)
{
  uint32_t pdindex = (uint32_t)virtualaddr >> 22;
  uint32_t ptindex = (uint32_t)virtualaddr >> 12 & 0x03FF;
 
  if(!page_dir[pdindex].present)
  {
    page_dir[pdindex].present = 1;
    page_dir[pdindex].read_write = 1;
    page_dir[pdindex].accessed = 1;
    page_dir[pdindex].user_mode = 1;
    page_dir[pdindex].frame = (uintptr_t)physaddr >> 12;
  }

  if(!page_tables[ptindex].present)
  {
    DebugOutput("[PAGING] Warning: page table entry does not exist!\n", ptindex);
    return;
  }
  //__tlb_flush();
}
