#include <errno.h>

#ifndef __SYSCALL_H
#define __SYSCALL_H

#define SYSEXIT 0x01
#define SYSFORK 0x02
#define SYSREAD 0x03
#define SYSWRITE 0x04
#define SYSOPEN 0x05
#define SYSCLOSE 0x06
#define GETPID 0x14

#define CHECK_FD(file_descriptor) \
if (file_descriptor == 0 || file_descriptor >= 64) \
{ \
  return -EBADFD; \
}

void EnableSystemCalls();

#endif
