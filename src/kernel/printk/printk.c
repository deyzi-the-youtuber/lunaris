#include <lunaris/printk.h>
#include <lunaris/config.h>
#include <common.h>
#include <stdarg.h>
#include <lunaris/spinlock.h>
#include <system.h>

DEFINE_SPINLOCK(printk_lock);
extern volatile uint8_t jiffies;

/* FIXED: Computer up-time in printk causes CPU exceptions */

void printk(char fmt[], ...)
{
  /*
    TODO:
    Store kernel messages into a buffer
  */
  va_list ap;
  va_start(ap, fmt);
  vsprintf(fmt, ap);
  va_end(ap);
}
