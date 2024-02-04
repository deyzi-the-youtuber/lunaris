#include <stdint.h>

#ifndef __CMOS_H
#define __CMOS_H

#define CMOS_ACCESS         0x70
#define CMOS_DATA           0x71
#define CMOS_STATUS_REG_A   0x0A

uint8_t cmos_get_register(uint8_t reg);
uint8_t cmos_get_update_flag(void);

#endif