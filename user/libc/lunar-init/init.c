#include "lib/stdio.h"
#include "lib/unistd.h"

#define BUFFER_MAX 512

int main(int argc, char * argv[])
{
  printf("Lunar-init!\n");
  uint8_t * buf;
  memset((void *)buf, 0, 256);
  read(0, (const uint8_t *)buf, 256);
  printf("You said this: %s\n", buf);
  for(;;);
  return 0;
}
