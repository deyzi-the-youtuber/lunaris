#include <common.h>
#include <stdarg.h>
#include <kernel/printk.h>

void PANIC(char fmt[], ...)
{
  va_list ap;
  va_start(ap, fmt);
  vprintk(fmt, ap);
  va_end(ap);
}