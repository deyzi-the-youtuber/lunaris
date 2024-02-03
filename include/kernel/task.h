#include <stdint.h>
#include <common.h>

#ifndef __TASK_H
#define __TASK_H

#define START(name, addr) addProcess(createProcess(name, (uint32_t)addr));
#define WAIT_FOR_PID(pid) while(is_pid_running(pid));

#define PROCESS_ALIVE 1
#define PROCESS_ZOMBIE 2
#define PROCESS_SLEEP 3
#define PROCESS_DEAD 4

/* Max processes */
#define NR_PROCS 64
/* Max value for PID */
#define MAX_PID_VALUE 32767

#define FOR_EACH_PROCESS(p) p = p->next ; while(p)

static uint32_t MAX_OPEN_FILES = 16;

typedef struct _Process{
	char *name;
	uint32_t pid;
	uint32_t esp;
	uint32_t stack;
	uint32_t eip;
	uint32_t cr3;
	uint32_t state;
	void (*notify)(uint32_t);
	bool NotExecuted;
	struct _Process *next, *prev;
}Process;

typedef uint32_t pid_t;

void initTasking();
void DumpTaskInfo();
uint32_t addProcess(Process * p);
Process * getCurrentProcess();
void __init__();
void preempt_now();
void __kill__();
void __notify__(uint32_t sig);
Process *createProcess(char *name,  uint32_t loc);
Process *getProcess(uint32_t pid);
void preempt();
void notify(uint32_t sig);
void kill(Process *p);
int is_pid_running(pid_t pid);
void sleep(Process * p);
void wakeup(Process * p);

#endif