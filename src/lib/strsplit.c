#include <common.h>
#include <stdint.h>

int strsplit(char * str, char delim)
{
	int n = 0;
	int i = 0;
	while(str[i])
	{
		if(str[i] == delim)
		{
			str[i] = 0;
			n++;
		}
		i++;
	}
	n++;
	return n;
}