#ifndef __TIMER_H
#define __TIMER_H

#define TIMER_INPUT_CLOCK_FREQUENCY 1193182
#define TIMER_CHANNEL_0_DATA_PORT 0x40
#define TIMER_CHANNEL_1_DATA_PORT 0x41
#define TIMER_CHANNEL_2_DATA_PORT 0x42
#define TIMER_COMMAND_PORT 0x43

void InitializeTimer();
int GetUptime();

#endif