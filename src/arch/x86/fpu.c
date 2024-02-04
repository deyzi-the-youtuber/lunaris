#include <kernel/sys/io.h>
#include <kernel/fpu.h>

void SetControlWordFPU(const uint16_t cw)
{
  asm volatile("fldcw %0" ::"m"(cw));
}

void EnableFPU()
{
  uint32_t cr4;
  asm volatile("mov %%cr4, %0" : "=r"(cr4));
  // set 9th bit to 1 in cr4
  cr4 |= 0x200;
  asm volatile("mov %0, %%cr4" ::"r"(cr4));
  SetControlWordFPU(0x37F); // fpu init
  SetControlWordFPU(0x37E); // invalid operand exceptions
  SetControlWordFPU(0x37A); // divide by zero
}