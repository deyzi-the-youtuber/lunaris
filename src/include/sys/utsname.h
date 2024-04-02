#ifndef __UTSNAME_H
#define __UTSNAME_H

#define MAX_LENGTH 64

#define UTSNAME_SYSNAME "Lunaris"
#define UTSNAME_NODENAME "lunaris"
#define UTSNAME_RELEASE "v0.1.0"
#define UTSNAME_VERSION (__DATE__ " " __TIME__)
#ifdef __i386__
#define UTSNAME_MACHINE "x86"
#else
#define UTSNAME_MACHINE "x86_64"
#endif

#if defined(__GNUC__)
#define __CONFIG_COMPILER__             "gcc"
#define __CONFIG_COMPILER_VERSION__     __VERSION__
#endif

struct utsname
{
  char sysname[MAX_LENGTH];
  char nodename[MAX_LENGTH];
  char release[MAX_LENGTH];
  char version[MAX_LENGTH];
  char machine[MAX_LENGTH];
};

#endif
