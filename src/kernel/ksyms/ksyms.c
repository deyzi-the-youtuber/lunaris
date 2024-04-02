#include <common.h>
#include <multiboot.h>
#include <stdint.h>
#include <system.h>
#include <lunaris/ksyms.h>

extern ksyms_t symlist[];

char * trace_address(uint32_t * off, uint32_t addr)
{
  for (uint32_t i = 0;; i++)
  {
    if (symlist[i].addr >= addr)
    {
      *off = addr - symlist[i - 1].addr;
      return symlist[i - 1].name;
    }
  }
}
