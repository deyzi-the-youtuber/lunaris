#include <common.h>

void substrr(int s, int e, char *src, char *dest)
{
  //substring exclusive range (end is exclusive, beginning is inclusive)
	memcpy(dest,&src[s],e-s);
	dest[e-s] = '\0';
}