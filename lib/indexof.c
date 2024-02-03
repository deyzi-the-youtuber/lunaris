#include <common.h>

int indexOf(char c, char *str)
{
	int i = 0;
	while(str[i] != '\0'){
		if(str[i] == c)
			return i;
		i++;
	}
	return strlen(str);
}