#include <stdint.h>
#include <common.h>

void * memset(void * dest, char val, int count)
{
  char * temp = (char *)dest;
  for( ; count != 0; count--) *temp++ = val;
  return dest;
}