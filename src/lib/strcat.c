#include <common.h>
#include <lunaris/mm.h>

char * strcat(char * dest, const char * src)
{
  int dest_len = strlen(dest);
  int src_len = strlen(src);
  
  // Allocate memory for the concatenated string
  char * result = (char *)malloc(dest_len + src_len + 1);
  
  // Check if memory allocation was successful
  if (result == NULL) 
  {
    return NULL;
  }
  
  // Copy the destination string to the result string
  strcpy(result, dest);
  
  // Append the source string to the result string
  strcpy(result + dest_len, (char *)src);
  
  // Return the concatenated string
  return result;
}
