#include <kernel/isr.h>
#include <kernel/idt.h>
#include <common.h>
#include <kernel/pic.h>
#include <kernel/debug.h>
#include <stdint.h>
#include <kernel/signal.h>
#include <kernel/cpu.h>

ISR InterruptHandlers[NUM_INTERRUPT_HANDLERS];

// for more details, see Intel manual -> Interrupt & Exception Handling
const char * ExceptionMessages[32] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "BOUND Range Exceeded",
    "Invalid Opcode",
    "Device Not Available (No Math Coprocessor)",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection",
    "Page Fault",
    "Unknown Interrupt (intel reserved)",
    "x87 FPU Floating-Point Error (Math Fault)",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void RegisterInterrupt(int interrupt, ISR handler)
{
  if (interrupt < NUM_INTERRUPT_HANDLERS)
    InterruptHandlers[interrupt] = handler;
}

void IRQHandlerISR(REGISTERS * reg)
{
  if (InterruptHandlers[reg->int_no] != NULL)
  {
    ISR handler = InterruptHandlers[reg->int_no];
    handler(reg);
  }
  EndOfInterruptPIC(reg->int_no);
}

static void DumpRegisters(REGISTERS * reg)
{
  DebugOutput(" [+] error code: %d\n", reg->err_code);
  DebugOutput(" [+] eax: %x, ebx: %x, ecx: %x, edx: %x\n", reg->eax, reg->ebx, reg->ecx, reg->edx);
  DebugOutput(" [+] edi: %x, esi: %x, ebp: %x, esp: %x\n", reg->edi, reg->esi, reg->ebp, reg->esp);
  DebugOutput(" [+] eip: %x, cs: %x, ss: %x, eflags: %x, useresp: %x\n", reg->eip, reg->ss, reg->eflags, reg->useresp);
}

void ExceptionHandlerISR(REGISTERS reg)
{
  if (reg.int_no < 32) 
  {
    DebugOutput("[EXCEPTION] %s\n", ExceptionMessages[reg.int_no]);
    DumpRegisters(&reg);
    if (InterruptHandlers[reg.int_no] != NULL)
    {
      ISR handler = InterruptHandlers[reg.int_no];
      handler(&reg);
    }
    else
    {
      DebugOutput("[EXCEPTION] Unhandled exception 0x%x\n", reg.int_no);
      PANIC("Unhandled exception 0x%x\n", reg.int_no);
    }
  }
  if (InterruptHandlers[reg.int_no] != NULL)
  {
    ISR handler = InterruptHandlers[reg.int_no];
    handler(&reg);
  }
}