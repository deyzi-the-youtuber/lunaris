#include <common.h>

void memcpy(void * dest, void * src, int n)
{
  // Typecast src and dest addresses to (char *)
  char * csrc = (char *)src;
  char * cdest = (char *)dest;

  // Copy contents of src[] to dest[]
  for (int i = 0; i < n; i++)
    cdest[i] = csrc[i];
}