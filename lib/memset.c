#include <stdint.h>
#include <common.h>

void * memset(void * b, int c, int len)
{
  int i;
  uint8_t * p = b;
  i = 0;
  while (len > 0)
  {
    *p = c;
    p++;
    len--;
  }
  return (b);
}