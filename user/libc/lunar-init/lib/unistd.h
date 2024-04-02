#include <stdint.h>

#ifndef __UNISTD_H
#define __UNISTD_H

#define O_RDONLY 0x1
#define O_WRONLY 0x2
#define O_NONBLOCK 0x4
#define O_APPEND 0x8

#define O_CREAT 0x200
#define O_TRUNC 0x400

#define O_RDWR (O_RDONLY | O_WRONLY)

typedef uint32_t pid_t;

void write(uint32_t fd, const char * buf, uint32_t count);
void read(uint32_t fd, const char * buf, uint32_t count);
void execve(const char * path, const char * argv[], const char * envp[]);
uint32_t open(char * filename, int flags, uint16_t mode);
pid_t getpid(void);

#endif