#include <stdint.h>

#ifndef __FPU_H
#define __FPU_H

void SetControlWordFPU(const uint16_t cw);
void EnableFPU();

#endif