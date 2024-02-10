#include <kernel/cpu.h>
#include <kernel/task.h>
#include <kernel/mm/malloc.h>
#include <kernel/mm/paging.h>
#include <kernel/syscall.h>
#include <kernel/signal.h>
#include <kernel/debug.h>
#include <kernel/tss.h>
#include <kernel/config.h>
#include <system.h>
#include <common.h>
#include <stdint.h>
#include <kernel/printk.h>

Process *CurrentProcess;
Process *KernelProcess;
uint32_t __cpid__ = 0;
bool tasking_enabled = false;

static void task_exit();

void kthread()
{
	DebugOutput("[TASKING] Multitasking online\n");
	tasking_enabled = true;
	KernelMainStage2();
	for(;;);
}

pid_t get_free_pid()
{
	DebugOutput("[TASKING] Getting free PID...\n");
	Process * p = KernelProcess;
	for(pid_t pid = 1; pid < MAX_PID_VALUE; pid++)
	{
		if (p->pid != pid)
		{
			DebugOutput("[TASKING] Found free PID %d\n", pid);
			return pid;
		}
		p = p->next;
	}
	return -1;
}

Process * createProcess(char *name, uint32_t addr, int argc, char * argv[])
{
	if (__cpid__ >= CONFIG_MAX_TASKS)
	{
		DebugOutput("[TASKING] Max amount of tasks has been reached!\n");
		return NULL;
	}
	Process * p = (Process *)malloc(sizeof(Process));
	memset(p, 0, sizeof(Process)); // make sure everything's clean

	p->name = name;
	p->pid = get_free_pid();
	p->state = PROCESS_ALIVE;
	p->notify = __notify__;
	p->eip = addr;
	p->esp = (uint32_t)malloc(4096);
	p->NotExecuted = true;
	asm volatile("mov %%cr3, %%eax" : "=a"(p->cr3));
	uint32_t * stack = (uint32_t *)(p->esp + 4096);
	p->stack = p->esp;
	*--stack = argc;
	for(int i = argc; i >= 0; i--)
	{
		*--stack = (uint32_t)argv[i];
	}
	*--stack = 0x202;					// eflags
	*--stack = 0x8;						// cs
	*--stack = addr;					// eip
	*--stack = 0;							// eax
	*--stack = 0;							// ebx
	*--stack = 0;							// ecx;
	*--stack = 0;							// edx
	*--stack = 0;							// esi
	*--stack = 0;							// edi
	*--stack = p->esp + 4096; // ebp
	*--stack = 0x10;					// ds
	*--stack = 0x10;					// fs
	*--stack = 0x10;					// es
	*--stack = 0x10;					// gs

	p->esp = (uint32_t)stack;
	DebugOutput("[TASKING] \"%s\" created with PID %d\n", p->name, p->pid);
	return p;
}

Process * getProcess(pid_t pid)
{
	Process *current = KernelProcess;
	do
	{
		if (current->pid == pid)
			return current;
		current = current->next;
	} while (current != KernelProcess);
	return NULL;
}

void reload_context()
{
	CurrentProcess->NotExecuted = false;
	asm volatile("mov %%eax, %%esp" : : "a"(CurrentProcess->esp));
	asm volatile("pop %gs");
	asm volatile("pop %fs");
	asm volatile("pop %es");
	asm volatile("pop %ds");
	asm volatile("pop %ebp");
	asm volatile("pop %edi");
	asm volatile("pop %esi");
	asm volatile("pop %edx");
	asm volatile("pop %ecx");
	asm volatile("pop %ebx");
	asm volatile("pop %eax");
	asm volatile("iret"); // FIX: Causes a general protection fault exception
}

void preempt_now()
{
	if (!tasking_enabled)
		return;
	asm volatile("int $0x81");
}

void wakeup(Process * p)
{
	DebugOutput("[TASKING] Waking up PID %d\n", p->pid);
	InterruptsLock();
	if (p->state == PROCESS_SLEEP)
  	p->state = PROCESS_ALIVE;
	InterruptsRelease();
}

void sleep(Process * p)
{
	DebugOutput("[TASKING] Putting PID %d to sleep\n", p->pid);
	InterruptsLock();
  p->state = PROCESS_SLEEP;
	InterruptsRelease();
  preempt_now();
}

void __kill__()
{
	DebugOutput("[TASKING] Killing PID %d (\"%s\")\n", CurrentProcess->pid, CurrentProcess->name);
	if (CurrentProcess->pid != 1)
	{
		tasking_enabled = false;
		free((uint8_t *)CurrentProcess->stack);
		free(CurrentProcess);
		CurrentProcess->prev->next = CurrentProcess->next;
		CurrentProcess->next->prev = CurrentProcess->prev;
		CurrentProcess->state = PROCESS_DEAD;
		tasking_enabled = true;
		preempt_now();
	}
	else
	{
		PANIC("attempted to kill kernel\n");
		for(;;);
	}
}

static void task_exit()
{
	register uint32_t val __asm__ ("eax");
	printk("PID %d exited with return code %d", getCurrentProcess()->pid, val);
	__kill__();
	asm("hlt");
}

void __notify__(uint32_t sig)
{
	switch (sig)
	{
	case SIGTERM:
		printk("notifying process with SIGTERM...\n");
		__kill__();
		break;
	case SIGSEGV:
		printk("notifying process with SIGSEGV...\n");
		__kill__();
		break;
	case SIGILL:
		printk("notifying process with SIGILL\n", CurrentProcess->pid);
		__kill__();
		break;
	case SIGINT:
		DebugOutput("[TASKING] Task received SIGINT\n");
		break;
	}
}

void initTasking()
{
	DebugOutput("[TASKING] Max amount of tasks: %d\n", CONFIG_MAX_TASKS);
	KernelProcess = createProcess("lunaridle", (uint32_t)kthread, 0, 0);
	KernelProcess->next = KernelProcess;
	KernelProcess->prev = KernelProcess;
	CurrentProcess = KernelProcess;
	reload_context();
	// pop all of the registers off of the stack and get started
}

Process * getCurrentProcess()
{
	return CurrentProcess;
}

uint32_t addProcess(Process * p)
{
	if (__cpid__ >= CONFIG_MAX_TASKS)
	{
		DebugOutput("[TASKING] Max amount of tasks has been reached!\n");
	}
	bool en = tasking_enabled;
	tasking_enabled = false;
	p->next = CurrentProcess->next;
	p->next->prev = p;
	p->prev = CurrentProcess;
	CurrentProcess->next = p;
	tasking_enabled = en;
	return p->pid;
}

int is_pid_running(pid_t pid)
{
	Process * p = KernelProcess;
	Process * orig = KernelProcess;
	while(1)
	{
		if(p->pid == pid)
		{
			return 1;
		}
		p = p->next;
		if(p == orig) 
			break;
	}
	return 0;
}

void kill(Process * p)
{
	if (getProcess(p->pid) != NULL)
	{
		DebugOutput("[TASKING] Killing PID %d (\"%s\")\n", p->pid, p->name);
		tasking_enabled = false;
		free((void *)p->stack);
		free(p);
		p->prev->next = p->next;
		p->next->prev = p->prev;
		p->state = PROCESS_DEAD;
		tasking_enabled = true;
	}
}

void preempt()
{
	InterruptsLock();
	// push CurrentProcess process' registers on to its stack
	asm volatile("push %eax");
	asm volatile("push %ebx");
	asm volatile("push %ecx");
	asm volatile("push %edx");
	asm volatile("push %esi");
	asm volatile("push %edi");
	asm volatile("push %ebp");
	asm volatile("push %ds");
	asm volatile("push %es");
	asm volatile("push %fs");
	asm volatile("push %gs");
	asm volatile("mov %%esp, %%eax" : "=a"(CurrentProcess->esp));
	CurrentProcess = CurrentProcess->next;
	if (CurrentProcess->NotExecuted)
	{
		reload_context();
		return;
	}
	// pop all of next process' registers off of its stack
	asm volatile("mov %%eax, %%cr3" : : "a"(CurrentProcess->cr3));
	asm volatile("mov %%eax, %%esp" : : "a"(CurrentProcess->esp));
	asm volatile("pop %gs");
	asm volatile("pop %fs");
	asm volatile("pop %es");
	asm volatile("pop %ds");
	asm volatile("pop %ebp");
	asm volatile("pop %edi");
	asm volatile("pop %esi");
	asm volatile("pop %edx");
	asm volatile("pop %ecx");
	asm volatile("pop %ebx");
	asm volatile("pop %eax");
	InterruptsRelease();
}
