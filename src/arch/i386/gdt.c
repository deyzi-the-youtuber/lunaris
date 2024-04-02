#include <stdint.h>
#include <lunaris/gdt.h>
#include <lunaris/video.h>
#include <common.h>
#include <lunaris/tss.h>

TSS gTSS;
GDT gGDT[NUM_GDT_DESCRIPTORS];
PointerGDT pGDT = {
  .Limit = sizeof(gGDT) - 1,
  .BaseAddress = (uint32_t)gGDT,
};

extern void InstallGDT(uint32_t PointerGDT);

void WriteTSS(uint32_t num, uint16_t ss0, uint32_t esp0)
{
	uint32_t base = (uint32_t)&gTSS;
	uint32_t limit = base + sizeof(gTSS);

	SetGateGDT(num, base, limit, 0xE9, 0x00);

	memset(&gTSS, 0, sizeof(gTSS));

	gTSS.ss0		= ss0;		// kernel stack segment
	gTSS.esp0		= esp0;		// kernel stack pointer

	gTSS.cs		= 0x0b;
	gTSS.ss		= gTSS.ds = gTSS.es = gTSS.fs = gTSS.gs = 0x13;
}

/* Fills in GDT descriptors */
void SetGateGDT(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
  GDT * TemporaryGDT = &gGDT[index];

  TemporaryGDT->SegmentLimit = (limit & 0xFFFF);
  TemporaryGDT->BaseLo = (base & 0xFFFF);
  TemporaryGDT->BaseMid = (base >> 16) & 0xFF;
  TemporaryGDT->Access = access;

  TemporaryGDT->GranularityByte = (limit >> 16) & 0x0F;
  TemporaryGDT->GranularityByte |= (granularity & 0xF0);

  TemporaryGDT->BaseHigh = (base >> 24) & 0xFF;
}

void InitializeGDT()
{
  /* Null segment */
  SetGateGDT(0, 0, 0, 0, 0);
  /* Kernel code segment */
  SetGateGDT(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
  /* Kernel data segment */
  SetGateGDT(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

  /* User code segment */
  SetGateGDT(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
  /* User data segment */
  SetGateGDT(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
  WriteTSS(5, 0x10, 0);

  InstallGDT((uint32_t)&pGDT);
  LoadTSS();
}

void SetKernelStack(uint32_t esp0)
{
  gTSS.esp0 = esp0;
}
