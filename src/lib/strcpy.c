#include <common.h>

void strcpy(char *dest, char *src)
{
	memcpy(dest, src, strlen(src));
	dest[strlen(src)] = '\0';
}