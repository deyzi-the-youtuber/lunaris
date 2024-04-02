#include <stdint.h>

#ifndef __SERIAL_H
#define __SERIAL_H

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8
#define COM5 0x5F8
#define COM6 0x4F8
#define COM7 0x5E8
#define COM8 0x4E8

int SerialInitialize();
void SerialRead(uint8_t * buffer, uint32_t count);
void SerialWrite(char a);

#endif
