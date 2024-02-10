#include <common.h>

void strncpy(char * dest, char * src, int n)
{
	memcpy(dest, src, n);
	dest[n] = '\0';
}