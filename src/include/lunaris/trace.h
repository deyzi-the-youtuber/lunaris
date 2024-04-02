#include <stdint.h>

#ifndef __TRACE_H
#define __TRACE_H

struct stackframe
{
  struct stackframe * ebp;
  uint32_t eip;
};

void StackTrace(uint32_t MaxFrames);
char * trace_address(uint32_t * off, uint32_t addr);

#endif