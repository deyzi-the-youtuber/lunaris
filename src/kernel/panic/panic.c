#include <common.h>
#include <stdarg.h>
#include <lunaris/printk.h>
#include <lunaris/trace.h>

void PANIC(char fmt[], ...)
{
  va_list ap;
  va_start(ap, fmt);
  vsprintf(strcat("kernel panic: ", fmt), ap);
  va_end(ap);
  InterruptsLock();
  StackTrace(16);
  asm volatile(
    "cli\n"
    "hlt\n"
  );
  __builtin_unreachable();
}
