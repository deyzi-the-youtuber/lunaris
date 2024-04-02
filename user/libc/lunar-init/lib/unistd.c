#include <stdint.h>
#include "unistd.h"

void write(uint32_t fd, const char * buf, uint32_t count)
{
  asm volatile(
      "movl $4, %%eax \n"
      "movl $1, %%ebx \n"
      "movl %0, %%ecx \n"
      "movl %1, %%edx \n"
      "int $0x80 \n"
      :
      : "r"(buf), "r"(count)
      : "%eax", "%ebx", "%ecx", "%edx");
}

void read(uint32_t fd, const char * buf, uint32_t count)
{
  asm volatile(
      "movl $3, %%eax \n"
      "movl $2, %%ebx \n"
      "movl %0, %%ecx \n"
      "movl %1, %%edx \n"
      "int $0x80 \n"
      :
      : "r"(buf), "r"(count)
      : "%eax", "%ebx", "%ecx", "%edx");
}

void execve(const char * path, const char * argv[], const char * envp[])
{
  asm volatile(
    "movl $0x0b, %%eax \n"
    "movl %0, %%ebx \n"
    "movl %1, %%ecx \n"
    "movl %2, %%edx \n"
    "int $0x80 \n"
    :
    : "m"(path), "r"(argv), "r"(envp)
    : "%eax", "%ebx", "%ecx", "%edx"
  );
}

pid_t getpid(void)
{
  pid_t pid;
  asm volatile("mov $0x14, %eax");
  asm volatile("int $0x80");
  asm volatile("movl %%ebx, %%eax": :"b"(pid));
  return pid;
}

uint32_t open(char * filename, int flags, uint16_t mode)
{
  uint32_t fd;
  asm volatile(
    "movl $5, %%eax\n"
    "movl %0, %%ebx\n"
    "movl %1, %%ecx\n"
    "movl %2, %%edx\n"
    "int $0x80\n"
    :
    : "r"(filename), "r"(flags), "m"(mode)
    : "%eax", "%ebx", "%ecx", "%edx");
  asm volatile("movl %%ebx, %%eax": :"b"(fd));
  return fd;
}