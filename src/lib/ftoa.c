#include <common.h>
#include <stdint.h>

void ftoa(char *buf, float f)
{
  uint32_t count = 1;
  const uint32_t DEFAULT_DECIMAL_COUNT = 8;
  char int_part_buf[16];
  char *p;

  memset(int_part_buf, 0, sizeof(int_part_buf));

  // add integer part
  int x = (int)f;
  itoa(x, int_part_buf, 10);  // Use itoa correctly
  p = int_part_buf;

  while (*p != '\0')
  {
    *buf++ = *p++;
  }
  *buf++ = '.';

  // example decimal = 3.14159 - 3 = 0.14159
  float decimal = f - x;
  if (decimal == 0)
    *buf++ = '0';
  else
  {
    while (decimal > 0)
    {
      uint32_t y = decimal * 10;  // y = 0.14159 * 10 = 1
      *buf++ = y + '0';
      decimal = (decimal * 10) - y;  // decimal = (0.14159 * 10) - 1 = 0.4159
      count++;
      if (count == DEFAULT_DECIMAL_COUNT)
        break;
    }
  }
  *buf = '\0';
}