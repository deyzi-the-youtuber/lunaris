#ifndef __BIOS_32_H
#define __BIOS_32_H

#include <stdint.h>
#include <kernel/isr.h>

// defined in bios32_call.asm
extern void BIOS32_START();
extern void BIOS32_END();
extern void * bios32_gdt_ptr;
extern void * bios32_gdt_entries;
extern void * bios32_idt_ptr;
extern void * bios32_in_reg16_ptr;
extern void * bios32_out_reg16_ptr;
extern void * bios32_int_number_ptr;

#define REBASE_ADDRESS(x)  (void*)(0x7c00 + (void*)x - (uint32_t)BIOS32_START)

#define BIOS_CONVENTIONAL_MEMORY 0x7e00

void bios32_init();
// bios interrupt call
void int86(uint8_t interrupt, REGISTERS16 * in, REGISTERS16 * out);

#endif