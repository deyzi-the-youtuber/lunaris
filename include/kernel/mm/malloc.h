#include <stdint.h>

#ifndef __MALLOC_H
#define __MALLOC_H

typedef struct
{
	uint8_t status;
	uint32_t size;
} alloc_t;

void InitializeMemoryManagement(uint32_t * KernelEnd);
void pfree(void *mem);
char* pmalloc(uint32_t size);
char* malloc(uint32_t size);
void free(void *mem);

#endif