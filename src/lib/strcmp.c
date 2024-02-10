#include <stdint.h>
#include <common.h>

int strcmp(const char * a, const char * b)
{
    while (*a && *a == *b) { ++a; ++b; }
    return (int)(uint8_t)(*a) - (int)(uint8_t)(*b);
}