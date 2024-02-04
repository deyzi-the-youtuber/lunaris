#include <stdint.h>

#ifndef __IO_H
#define __IO_H

void IoPortByteWrite(uint16_t Port, uint8_t Value);
void IoPortWordWrite(uint16_t Port, uint16_t Value);
void IoPortDwordWrite(uint16_t Port, uint32_t Value);

uint8_t IoPortByteRead(uint16_t Port);
uint16_t IoPortWordRead(uint16_t Port);
uint32_t IoPortDwordRead(uint16_t Port);

#endif