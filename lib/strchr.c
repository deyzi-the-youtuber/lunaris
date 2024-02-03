#include <common.h>

char * strchr(char *s, const char c)
{
   while(*s != c && *s != '\0') {
      s++;
   }
   if(*s == c) {
      return s;
   }else {
      return NULL;
   }
}
