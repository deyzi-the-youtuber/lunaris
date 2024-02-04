#include <common.h>
#include <stdint.h>

int memcmp(const void * str1, const void * str2, int count)
{
  const uint8_t * s1 = (const uint8_t *)str1;
  const uint8_t * s2 = (const uint8_t *)str2;

  while (count-- > 0)
  {
    if (*s1++ != *s2++)
      return s1[-1] < s2[-1] ? -1 : 1;
  }
  return 0;
}