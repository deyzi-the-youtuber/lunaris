#include <stdint.h>

#ifndef __MEMORY_H
#define __MEMORY_H

// kernel heap stuff

typedef struct
{
	uint8_t status;
	uint32_t size;
}alloc_t;

void InitializeMemoryManagement(uint32_t * KernelEnd);
void pfree(void * mem);
char * pmalloc(uint32_t size);
char * malloc(uint32_t size);
void free(void * mem);

// paging

#define PAGE_SIZE 4096

struct page_directory
{
  uint32_t present : 1;
  uint32_t read_write : 1;
  uint32_t user_mode : 1;
  uint32_t write_through : 1;
  uint32_t cache : 1;
  uint32_t accessed : 1;
  uint32_t dirty : 1;
  uint32_t page_size : 1;
  uint32_t global : 1;
  uint32_t available : 3;
  uint32_t frame : 20;
};

struct page_table
{
  uint32_t present : 1;
  uint32_t read_write : 1;
  uint32_t user_mode : 1;
  uint32_t write_through : 1;
  uint32_t cache : 1;
  uint32_t accessed : 1;
  uint32_t dirty : 1;
  uint32_t page_size : 1;
  uint32_t global : 1;
  uint32_t available : 3;
  uint32_t frame : 20;
};

void paging_init();
void paging_map_page(void * physaddr, void * virtualaddr);

#endif
