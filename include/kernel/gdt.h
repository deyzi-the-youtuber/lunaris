#include <stdint.h>

#ifndef __GDT_H
#define __GDT_H

#define NUM_GDT_DESCRIPTORS 8

typedef struct
{
  uint16_t SegmentLimit;
  uint16_t BaseLo;
  uint8_t BaseMid;
  uint8_t Access;
  uint8_t GranularityByte;
  uint8_t BaseHigh;
} __attribute__((packed)) GDT;

typedef struct
{
  uint16_t Limit;
  uint32_t BaseAddress;
} __attribute__((packed)) PointerGDT;

void SetGateGDT(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);
void InitializeGDT();

#endif