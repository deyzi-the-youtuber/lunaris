#include <stdint.h>

#ifndef __KSYMS_H
#define __KSYMS_H

typedef struct
{
  uint32_t addr;
  char * name;
}ksyms_t;

extern ksyms_t symlist[];

#endif