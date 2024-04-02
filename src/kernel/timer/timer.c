#include <lunaris/timer.h>
#include <lunaris/isr.h>
#include <sys/io.h>
#include <lunaris/task.h>
#include <lunaris/sched.h>
#include <lunaris/pic.h>
#include <lunaris/config.h>
#include <stdint.h>
#include <lunaris/printk.h>
#include <lunaris/module.h>

MODULE("pit");

uint16_t TimerFrequency;
volatile uint32_t jiffies;

extern bool tasking_enabled;

void SetFrequencyTimer(uint16_t f)
{
  TimerFrequency = f;
  uint16_t divisor = TIMER_INPUT_CLOCK_FREQUENCY / f;
  // set Mode 3 - Square Wave Mode
  outb(TIMER_COMMAND_PORT, 0x36);
  // set low byte
  outb(TIMER_CHANNEL_0_DATA_PORT, (divisor & 0xFF));
  // set high byte
  outb(TIMER_CHANNEL_0_DATA_PORT, (divisor >> 8 & 0xFF));
  printm("initializing with %dhz\n", f);
}

void TimerHandler(REGISTERS *regs)
{
  jiffies++;
  if (tasking_enabled)
  {
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

void timer_sleep_ms(uint32_t ms)
{
	uint32_t wake_time = jiffies + ms;
	while (jiffies < wake_time)
		continue;
}

void InitializeTimer()
{
  SetFrequencyTimer(CONFIG_TIMER_FREQ);
  RegisterInterrupt(32, TimerHandler);
}
