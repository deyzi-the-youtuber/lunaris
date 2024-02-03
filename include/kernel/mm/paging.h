#include <stdint.h>
#include <kernel/isr.h>

#ifndef __PAGING_H
#define __PAGING_H

#define PAGE_SIZE 4096

typedef struct
{
  uint32_t Present : 1;
  uint32_t RW : 1;
  uint32_t User : 1;
  uint32_t WriteThrough : 1;
  uint32_t Cache : 1;
  uint32_t Accessed : 1;
  uint32_t Dirty : 1;
  uint32_t PageSize : 1;
  uint32_t Global : 1;
  uint32_t Available : 3;
  uint32_t Frame : 20;
}PageDirectory;

typedef struct
{
  uint32_t Present : 1;
  uint32_t RW : 1;
  uint32_t User : 1;
  uint32_t WriteThrough : 1;
  uint32_t Cache : 1;
  uint32_t Accessed : 1;
  uint32_t Dirty : 1;
  uint32_t PageSize : 1;
  uint32_t Global : 1;
  uint32_t Available : 3;
  uint32_t Frame : 20;
}PageTable;

//void PagingFreePage(void * virtual_addr);
//void PagingAllocatePage(void * virtual_addr);
//void * PagingGetPhysAddr(void * virtual_addr);

void InitializePaging();
void PageFaultHandler(REGISTERS * r);


//void PagingSwitchPageDirectory(PageDirectory * dir);
//uint32_t * PagingGetPageDirectory();
//void PagingVirtualUnmap(void * virtaddr);
//void PagingVirtualMap(void * physaddr, void * virtualaddr, uint32_t flags);
void exec(uint8_t * prog);
extern void LoadPageDirectory(uint32_t * pd);

#endif