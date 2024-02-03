#include <stdarg.h>

#ifndef __PRINTK_H
#define __PRINTK_H

void printk(char fmt[], ...);
void vprintk(char fmt[], va_list ap);

#endif