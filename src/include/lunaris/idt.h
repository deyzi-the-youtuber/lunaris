#ifndef __IDT_H
#define __IDT_H

#include <stdint.h>

#define NUM_IDT_DESCRIPTORS 256

typedef struct
{
  uint16_t BaseLo;
  uint16_t SegmentSelector;
  uint8_t Reserved;
  uint8_t Attribues;
  uint16_t BaseHigh;
} __attribute__((packed)) IDT;

typedef struct
{
  uint16_t Limit;
  uint32_t BaseAddress;
} __attribute__((packed)) PointerIDT;

void InitializeIDT();
void SetGateIDT(int descriptor, uint32_t base, uint16_t seg, uint8_t flags);

#endif