#include <kernel/isr.h>
#include <kernel/task.h>
#include <kernel/signal.h>
#include <stdint.h>

void divide_by_zero_handler(REGISTERS * regs)
{
  notify(SIGTERM);
}

void gpf_handler(REGISTERS * regs)
{
  notify(SIGTERM);
}

void invalid_opcode_handler(REGISTERS * regs)
{
  notify(SIGILL);
}

void install_exception_handlers(void)
{
  RegisterInterrupt(0, divide_by_zero_handler);
  RegisterInterrupt(6, invalid_opcode_handler);
  RegisterInterrupt(13, gpf_handler);
}