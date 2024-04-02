#include <stdint.h>
#include <sys/io.h>
#include <lunaris/cpu.h>
#include <common.h>
#include <cpuid.h>

void cpu_get_brand_string(char brand_string[])
{
  memset(brand_string, 0, strlen(brand_string));
  uint32_t regs[12];
  char str[sizeof(regs)+1];

  __cpuid(0x80000000, regs[0], regs[1], regs[2], regs[3]);

  if (regs[0] < 0x80000004)
    return;

  __cpuid(0x80000002, regs[0], regs[1], regs[2], regs[3]);
  __cpuid(0x80000003, regs[4], regs[5], regs[6], regs[7]);
  __cpuid(0x80000004, regs[8], regs[9], regs[10], regs[11]);

  memcpy(str, regs, sizeof(regs));
  str[sizeof(regs)] = '\0';
  strcpy(brand_string, str);
}

bool InterruptsEnabled(void)
{
  uint16_t Flags;
  asm volatile("pushf; pop %0" : "=g"(Flags));
  return Flags & (1 << 9);
}

void InterruptsRelease(void)
{
  asm volatile("sti");
}

void InterruptsLock(void)
{
  asm volatile("cli");
}

void outb(uint16_t Port, uint8_t Value)
{
  asm volatile("outb %0, %1" : : "a"(Value), "Nd"(Port) : "memory");
}

void outw(uint16_t Port, uint16_t Value)
{
  asm volatile("outw %0, %1" : : "a"(Value), "Nd"(Port) : "memory");
}

void outl(uint16_t Port, uint32_t Value)
{
  asm volatile("outl %0, %1" : : "a"(Value), "Nd"(Port) : "memory");
}

uint8_t inb(uint16_t Port)
{
  uint8_t Value;
  asm volatile("inb %1, %0" : "=a"(Value) : "Nd"(Port) : "memory");
  return Value;
}

uint16_t inw(uint16_t Port)
{
  uint16_t Value;
  asm volatile("inw %1, %0" : "=a"(Value) : "Nd"(Port) : "memory");
  return Value;
}

uint32_t inl(uint16_t Port)
{
  uint32_t Value;
  asm volatile("inl %1, %0" : "=a"(Value) : "Nd"(Port) : "memory");
  return Value;
}
