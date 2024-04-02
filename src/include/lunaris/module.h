#include <stdint.h>

#ifndef __MODULE_H
#define __MODULE_H

void __printm(const char fmt[], ...);

#define MODULE(x) static const char * _module_name_ __attribute__((unused)) = x;
#define printm(...) \
  __printm(_module_name_, __VA_ARGS__);

#endif
