#include "stdio.h"

void printf(char * fmt, ...)
{
    asm volatile (
        "mov $4, %%eax \n"
        "mov $1, %%ebx \n"
        "mov %0, %%ecx \n"
        "mov %1, %%edx \n"
        "int $0x80 \n"
        :
        : "r"(fmt), "r"(strlen(fmt))
        : "%eax", "%ebx", "%ecx", "%edx"
    );
}

int strlen(char * str)
{
  int i = 0;
  for(; str[i] != 0; i++);
  return i;
}