#include <common.h>
#include <stdint.h>

int strncmp(const char * s1, const char * s2, int n)
{
  while (n && *s1 && (*s1 == *s2))
  {
    ++s1;
    ++s2;
    --n;
  }
  if (n == 0)
  {
    return 0;
  }
  else
  {
    return (*(uint8_t *)s1 - *(uint8_t *)s2);
  }
}