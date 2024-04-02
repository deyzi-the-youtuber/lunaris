#include <stdint.h>
#include <common.h>
#include <fs/vfs.h>
#include <sys/types.h>

#ifndef __TASK_H
#define __TASK_H

#define START(name, addr) addProcess(createProcess(name, (uint32_t)addr, 0, 0));
#define WAIT_FOR_PID(pid) while(is_pid_running(pid));

#define PROCESS_ALIVE 1
#define PROCESS_ZOMBIE 2
#define PROCESS_SLEEP 3
#define PROCESS_DEAD 4

/* Max value for PID */
#define MAX_PID_VALUE 32767

#define FOR_EACH_PROCESS(p) p = p->next ; while(p)

#define MAX_OPEN_FILES 20

// This is just the task's registers n stuff
// ! do not modify !
struct task_context
{
	// 32-bit
	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t eip;
	uint32_t cr3;
	uint32_t eflags;
	// 16-bit
	uint16_t cs;
	uint16_t ds;
	uint16_t fs;
	uint16_t es;
	uint16_t gs;
};

typedef struct _Process
{
	char * name;
	pid_t pid;
	uint32_t stack;
	uint32_t state;
	struct task_context context;
	vfs_node_t * open_files[MAX_OPEN_FILES];
	void (*notify)(uint32_t);
	bool not_ready;
	struct _Process * next, * prev;
}Process;

extern bool tasking_enabled;

void initTasking();
void DumpTaskInfo();
uint32_t addProcess(Process * p);
Process * getCurrentProcess();
void __init__();
void preempt_now();
void __kill__();
void __notify__(uint32_t sig);
Process * createProcess(char * name, uint32_t addr);
Process * createUserProcess(char * name, uint32_t addr, int argc, char * argv[]);
Process * getProcess(pid_t pid);
void preempt();
void notify(uint32_t sig);
void kill(Process *p);
int is_pid_running(pid_t pid);
void sleep(Process * p);
void wakeup(Process * p);

#endif
