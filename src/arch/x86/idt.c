#include <kernel/video/vga.h>
#include <stdint.h>
#include <kernel/idt.h>
#include <kernel/isr.h>
#include <kernel/pic.h>
#include <kernel/debug.h>
#include <kernel/apic.h>
#include <common.h>

IDT gIDT[NUM_IDT_DESCRIPTORS];
PointerIDT pIDT;

extern void InstallIDT(uint32_t PointerIDT);

void SetGateIDT(int descriptor, uint32_t base, uint16_t seg, uint8_t flags)
{
  IDT * Temporary = &gIDT[descriptor];

  Temporary->BaseLo = base & 0xFFFF;
  Temporary->SegmentSelector = seg;
  Temporary->Reserved = 0;
  Temporary->Attribues = flags | 0x60;
  Temporary->BaseHigh = (base >> 16) & 0xFFFF;
}

void InitializeIDT()
{
  pIDT.BaseAddress = (uint32_t)gIDT;
  pIDT.Limit = sizeof(gIDT) - 1;
  InitializePIC();

  SetGateIDT(0, (uint32_t)Exception_0, 0x08, 0x8E);
  SetGateIDT(1, (uint32_t)Exception_1, 0x08, 0x8E);
  SetGateIDT(2, (uint32_t)Exception_2, 0x08, 0x8E);
  SetGateIDT(3, (uint32_t)Exception_3, 0x08, 0x8E);
  SetGateIDT(4, (uint32_t)Exception_4, 0x08, 0x8E);
  SetGateIDT(5, (uint32_t)Exception_5, 0x08, 0x8E);
  SetGateIDT(6, (uint32_t)Exception_6, 0x08, 0x8E);
  SetGateIDT(7, (uint32_t)Exception_7, 0x08, 0x8E);
  SetGateIDT(8, (uint32_t)Exception_8, 0x08, 0x8E);
  SetGateIDT(9, (uint32_t)Exception_9, 0x08, 0x8E);
  SetGateIDT(10, (uint32_t)Exception_10, 0x08, 0x8E);
  SetGateIDT(11, (uint32_t)Exception_11, 0x08, 0x8E);
  SetGateIDT(12, (uint32_t)Exception_12, 0x08, 0x8E);
  SetGateIDT(13, (uint32_t)Exception_13, 0x08, 0x8E);
  SetGateIDT(14, (uint32_t)Exception_14, 0x08, 0x8E);
  SetGateIDT(15, (uint32_t)Exception_15, 0x08, 0x8E);
  SetGateIDT(16, (uint32_t)Exception_16, 0x08, 0x8E);
  SetGateIDT(17, (uint32_t)Exception_17, 0x08, 0x8E);
  SetGateIDT(18, (uint32_t)Exception_18, 0x08, 0x8E);
  SetGateIDT(19, (uint32_t)Exception_19, 0x08, 0x8E);
  SetGateIDT(20, (uint32_t)Exception_20, 0x08, 0x8E);
  SetGateIDT(21, (uint32_t)Exception_21, 0x08, 0x8E);
  SetGateIDT(22, (uint32_t)Exception_22, 0x08, 0x8E);
  SetGateIDT(23, (uint32_t)Exception_23, 0x08, 0x8E);
  SetGateIDT(24, (uint32_t)Exception_24, 0x08, 0x8E);
  SetGateIDT(25, (uint32_t)Exception_25, 0x08, 0x8E);
  SetGateIDT(26, (uint32_t)Exception_26, 0x08, 0x8E);
  SetGateIDT(27, (uint32_t)Exception_27, 0x08, 0x8E);
  SetGateIDT(28, (uint32_t)Exception_28, 0x08, 0x8E);
  SetGateIDT(29, (uint32_t)Exception_29, 0x08, 0x8E);
  SetGateIDT(30, (uint32_t)Exception_30, 0x08, 0x8E);
  SetGateIDT(31, (uint32_t)Exception_31, 0x08, 0x8E);
  SetGateIDT(32, (uint32_t)irq_0, 0x08, 0x8E);
  SetGateIDT(33, (uint32_t)irq_1, 0x08, 0x8E);
  SetGateIDT(34, (uint32_t)irq_2, 0x08, 0x8E);
  SetGateIDT(35, (uint32_t)irq_3, 0x08, 0x8E);
  SetGateIDT(36, (uint32_t)irq_4, 0x08, 0x8E);
  SetGateIDT(37, (uint32_t)irq_5, 0x08, 0x8E);
  SetGateIDT(38, (uint32_t)irq_6, 0x08, 0x8E);
  SetGateIDT(39, (uint32_t)irq_7, 0x08, 0x8E);
  SetGateIDT(40, (uint32_t)irq_8, 0x08, 0x8E);
  SetGateIDT(41, (uint32_t)irq_9, 0x08, 0x8E);
  SetGateIDT(42, (uint32_t)irq_10, 0x08, 0x8E);
  SetGateIDT(43, (uint32_t)irq_11, 0x08, 0x8E);
  SetGateIDT(44, (uint32_t)irq_12, 0x08, 0x8E);
  SetGateIDT(45, (uint32_t)irq_13, 0x08, 0x8E);
  SetGateIDT(46, (uint32_t)irq_14, 0x08, 0x8E);
  SetGateIDT(47, (uint32_t)irq_15, 0x08, 0x8E);
  SetGateIDT(128, (uint32_t)Exception_128, 0x08, 0x8E);

  InstallIDT((uint32_t)&pIDT);
  asm volatile("sti");
}