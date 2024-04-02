#include <stdarg.h>
#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"

static void __vsprintf(char * fmt, va_list ap)
{
  while(*fmt)
  {
    if (*fmt == '%')
    {
      *fmt++;
      switch (*fmt)
      {
        case 's':
          char * str = va_arg(ap, char *);
          write(1, str, strlen(str));
          break;
        case 'd':
          int num = va_arg(ap, int);
          char s_num[3];
          itoa(num, s_num, 10);
          write(1, s_num, strlen(s_num));
          break;
        default:
          break;
      }
    }
    else
    {
      write(1, fmt, 1);
    }
    fmt++;
  }
}

void printf(char * fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  __vsprintf(fmt, ap);
  va_end(ap);
}

int strlen(char * str)
{
  int i = 0;
  for(; str[i] != 0; i++);
  return i;
}

void reverse(char * str[], int length)
{
  int start = 0;
  int end = length - 1;
  while (start < end)
  {
    char * temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    end--;
    start++;
  }
}

char * itoa(int value, char * str, int radix)
{
  char * rc;
  char * ptr;
  char * low;
  // Check for supported radix.
  if ( radix < 2 || radix > 36 )
  {
    *str = '\0';
    return str;
  }
  rc = ptr = str;
  // Set '-' for negative decimals.
  if ( value < 0 && radix == 10 )
  {
    *ptr++ = '-';
  }
  // Remember where the numbers start.
  low = ptr;
  // The actual conversion.
  do
  {
    // Modulo is negative for negative value. This trick makes abs() unnecessary.
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % radix];
    value /= radix;
  } while ( value );
  // Terminating the string.
  *ptr-- = '\0';
  // Invert the numbers.
  while ( low < ptr )
  {
    char tmp = *low;
    *low++ = *ptr;
    *ptr-- = tmp;
  }
  return rc;
}

int strcmp(const char * a, const char * b)
{
  while (*a && *a == *b) { ++a; ++b; }
  return (int)(uint8_t)(*a) - (int)(uint8_t)(*b);
}

void * memset(void * dest, char val, int count)
{
  char * temp = (char *)dest;
  for( ; count != 0; count--) *temp++ = val;
  return dest;
}
