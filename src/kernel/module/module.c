#include <lunaris/module.h>
#include <lunaris/printk.h>
#include <common.h>

inline void __printm(const char fmt[], ...)
{
  va_list ap;
  va_start(ap, fmt);
  printk("%s: ", fmt);
  char * str = va_arg(ap, char *);
  vsprintf(str, ap);
  va_end(ap);
}
