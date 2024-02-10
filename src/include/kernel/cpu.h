#ifndef __CPU_H
#define __CPU_H

#include <common.h>

void InterruptsRelease(void);
void InterruptsLock(void);
bool InterruptsEnabled(void);
extern uint32_t dump_eip();
extern bool check_cpuid();
void arch_get_brand_string(char brand_string[]);

#endif