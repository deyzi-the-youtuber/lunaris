#include <kernel/video/vga.h>
#include <kernel/trace.h>
#include <kernel/cpu.h>
#include <stdint.h>

#ifndef __COMMON_H
#define __COMMON_H

#define NULL ((void *)0)
#define MAJOR(num) ((uint8_t)((num>>8)&0xFF))
#define MINOR(num) ((uint8_t)((num)&0xFF))

typedef enum
{
  true = 1,
  false = 0
}bool;

char * itoa(int num, char *str, int radix);
void ftoa(char * buf, float f);
void reverse(char *str[], int length);
int strlen(const char *str);
void * memset(void * b, int c, int len);
void memcpy(void * dest, void * src, int n);
int strcmp(const char *a, const char *b);
int indexOf(char c, char *str);
void substrr(int s, int e, char *src, char *dest);
void strcpy(char *src, char *dest);
char *strcat(char *dest, const char *src);
char * strstr(const char *s1, const char *s2);
int memcmp(const void * str1, const void * str2, int count);
int strcspn(const char * str, const char * charset);
char * strchr(char *s, const char c);
uint32_t rand();
int strncmp(const char * s1, const char * s2, int n);
void PANIC(char fmt[], ...);

#endif