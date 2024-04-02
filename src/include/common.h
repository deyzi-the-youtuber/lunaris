#include <stdint.h>
#include <stdarg.h>
#include <sys/types.h>

#ifndef __COMMON_H
#define __COMMON_H

#define NULL ((void *)0)
#define MAJOR(num) ((uint8_t)((num >> 8) & 0xFF))
#define MINOR(num) ((uint8_t)((num) & 0xFF))

typedef enum
{
  true = 1,
  false = 0
}bool;

char * itoa(int num, char *str, int radix);
void ftoa(char * buf, float f);
void reverse(char *str[], int length);
int strlen(const char *str);
void * memset(void * dest, char val, int count);
void * memcpy(void * dest, const void * src, int count);
int strcmp(const char * a, const char * b);
int indexOf(char c, char *str);
void substrr(int s, int e, char * src, char * dest);
void strcpy(char *src, char *dest);
void strncpy(char * dest, char * src, int n);
char *strcat(char *dest, const char *src);
char * strstr(const char *s1, const char *s2);
int memcmp(const void * str1, const void * str2, int count);
int strcspn(const char * str, const char * charset);
char * strchr(char *s, const char c);
uint32_t rand();
int strncmp(const char * s1, const char * s2, int n);
void PANIC(char fmt[], ...);
int strsplit(char * str, char delim);
char * strtok(char * str, const char * delimiters);

size_t vsprintf(const char fmt[], va_list arg);

#endif
