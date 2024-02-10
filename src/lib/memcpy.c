#include <common.h>

void * memcpy(void * dest, const void * src, int count)
{
  const char * sp = (const char *)src;
  char * dp = (char *)dest;
  for(; count != 0; count--) *dp++ = *sp++;
  return dest;
}