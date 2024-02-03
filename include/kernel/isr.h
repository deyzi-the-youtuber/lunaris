#include <stdint.h>

#ifndef __ISR_H
#define __ISR_H

#define NUM_INTERRUPT_HANDLERS 256

typedef struct
{
  uint32_t ds;
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // pushed by pusha
  uint32_t int_no, err_code;                       // interrupt number and error code
  uint32_t eip, cs, eflags, useresp, ss;           // pushed by the processor automatically
}REGISTERS;

typedef struct
{
  uint16_t di;
  uint16_t si;
  uint16_t bp;
  uint16_t sp;
  uint16_t bx;
  uint16_t dx;
  uint16_t cx;
  uint16_t ax;
  // segments
  uint16_t ds;
  uint16_t es;
  uint16_t fs;
  uint16_t gs;
  uint16_t ss;
  uint16_t eflags;
}REGISTERS16;

typedef void (*ISR)(REGISTERS *);

void RegisterInterrupt(int num, ISR handler);

void ExceptionHandlerISR(REGISTERS reg);

void IRQHandlerISR(REGISTERS * reg);

extern void Exception_0();
extern void Exception_1();
extern void Exception_2();
extern void Exception_3();
extern void Exception_4();
extern void Exception_5();
extern void Exception_6();
extern void Exception_7();
extern void Exception_8();
extern void Exception_9();
extern void Exception_10();
extern void Exception_11();
extern void Exception_12();
extern void Exception_13();
extern void Exception_14();
extern void Exception_15();
extern void Exception_16();
extern void Exception_17();
extern void Exception_18();
extern void Exception_19();
extern void Exception_20();
extern void Exception_21();
extern void Exception_22();
extern void Exception_23();
extern void Exception_24();
extern void Exception_25();
extern void Exception_26();
extern void Exception_27();
extern void Exception_28();
extern void Exception_29();
extern void Exception_30();
extern void Exception_31();
extern void Exception_128();

extern void irq_0();
extern void irq_1();
extern void irq_2();
extern void irq_3();
extern void irq_4();
extern void irq_5();
extern void irq_6();
extern void irq_7();
extern void irq_8();
extern void irq_9();
extern void irq_10();
extern void irq_11();
extern void irq_12();
extern void irq_13();
extern void irq_14();
extern void irq_15();

#define IRQ_BASE 0x20
#define IRQ0_TIMER 0x00
#define IRQ1_KEYBOARD 0x01
#define IRQ2_CASCADE 0x02
#define IRQ3_SERIAL_PORT2 0x03
#define IRQ4_SERIAL_PORT1 0x04
#define IRQ5_RESERVED 0x05
#define IRQ6_DISKETTE_DRIVE 0x06
#define IRQ7_PARALLEL_PORT 0x07
#define IRQ8_CMOS_CLOCK 0x08
#define IRQ9_CGA 0x09
#define IRQ10_RESERVED 0x0A
#define IRQ11_RESERVED 0x0B
#define IRQ12_AUXILIARY 0x0C
#define IRQ13_FPU 0x0D
#define IRQ14_HARD_DISK 0x0E
#define IRQ15_RESERVED 0x0F

#endif