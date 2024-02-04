#include <kernel/video/vga.h>
#include <kernel/timer.h>
#include <kernel/isr.h>
#include <kernel/sys/io.h>
#include <kernel/task.h>
#include <kernel/sched.h>
#include <kernel/pic.h>
#include <kernel/config.h>
#include <stdint.h>
#include <kernel/printk.h>

uint16_t TimerFrequency;
volatile uint8_t jiffies;

extern bool tasking_enabled;

void SetFrequencyTimer(uint16_t f)
{
  TimerFrequency = f;
  uint16_t divisor = TIMER_INPUT_CLOCK_FREQUENCY / f;
  // set Mode 3 - Square Wave Mode
  IoPortByteWrite(TIMER_COMMAND_PORT, 0x36);
  // set low byte
  IoPortByteWrite(TIMER_CHANNEL_0_DATA_PORT, (divisor & 0xFF));
  // set high byte
  IoPortByteWrite(TIMER_CHANNEL_0_DATA_PORT, (divisor >> 8 & 0xFF));
  printk("clocksource: pit: initializing with %dhz\n", f);
}

void TimerHandler(REGISTERS *regs)
{
  jiffies++;
  if (tasking_enabled) // if (!tasking_enabled)
  {
  /*
	  asm volatile("pusha");
	  EndOfInterruptPIC(32);
	  asm volatile("popa");
  }
  else
  {
  */
    EndOfInterruptPIC(32);
    preempt();
  }
}

uint32_t get_uptime_seconds()
{
  return ((uint32_t)jiffies) / CONFIG_TIMER_FREQ;
}

uint32_t get_uptime_milliseconds()
{
  return (uint32_t)jiffies;
}

void InitializeTimer()
{
  SetFrequencyTimer(CONFIG_TIMER_FREQ);
  RegisterInterrupt(32, TimerHandler);
}