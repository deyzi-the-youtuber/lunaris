#include <common.h>

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