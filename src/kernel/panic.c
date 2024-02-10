#include <common.h>
#include <stdarg.h>
#include <kernel/printk.h>
#include <kernel/trace.h>

void PANIC(char fmt[], ...)
{
  va_list ap;
  va_start(ap, fmt);
  vprintk(strcat("kernel panic: ", fmt), ap);
  va_end(ap);
  InterruptsLock();
  StackTrace(8);
  for(;;);
}