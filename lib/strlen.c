#include <common.h>

int strlen(const char * str)
{
  int i = 0;
  while (*str)
  {
    str++;
    i++;
  }
  return i;
}