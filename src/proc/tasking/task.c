#include <lunaris/cpu.h>
#include <lunaris/task.h>
#include <lunaris/mm.h>
#include <lunaris/syscall.h>
#include <lunaris/signal.h>
#include <lunaris/debug.h>
#include <lunaris/tss.h>
#include <lunaris/config.h>
#include <system.h>
#include <common.h>
#include <stdint.h>
#include <lunaris/module.h>
#include <sys/types.h>

MODULE("scheduler");

Process * CurrentProcess;
Process * KernelProcess;
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

Process * createProcess(char * name, uint32_t addr)
{
	if (__cpid__ >= CONFIG_NPROC)
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
	p->not_ready = true;
	// now time for the register stuff :)
	p->context.eax = 0;
	p->context.ebx = 0;
	p->context.ecx = 0;
	p->context.edx = 0;
	p->context.esp = (uint32_t)malloc(PAGE_SIZE);
	p->context.ebp = p->context.esp;
	p->context.esi = 0;
	p->context.edi = 0;
	p->context.eip = (uint32_t)addr;
	p->context.eflags = 0x00000202;

	p->context.cs = 0x0008;
	p->context.ds = 0x0010;
	p->context.fs = 0x0010;
	p->context.es = 0x0010;
	p->context.gs = 0x0010;


	asm volatile("mov %%cr3, %%eax" : "=a"(p->context.cr3));
	uint32_t * stack = (uint32_t *)(p->context.esp + PAGE_SIZE);
	p->stack = p->context.esp;
	*--stack = (uint32_t)p->context.eflags;					// eflags
	*--stack = (uint32_t)p->context.cs;						// cs
	*--stack = (uint32_t)p->context.eip;					// eip
	*--stack = (uint32_t)p->context.eax;							// eax
	*--stack = (uint32_t)p->context.ebx;							// ebx
	*--stack = (uint32_t)p->context.ecx;							// ecx;
	*--stack = (uint32_t)p->context.edx;							// edx
	*--stack = (uint32_t)p->context.esi;							// esi
	*--stack = (uint32_t)p->context.edi;							// edi
	*--stack = (uint32_t)p->context.ebp; // ebp
	*--stack = (uint32_t)p->context.ds;					// ds
	*--stack = (uint32_t)p->context.fs;					// fs
	*--stack = (uint32_t)p->context.es;					// es
	*--stack = (uint32_t)p->context.gs;					// gs
	p->context.esp = (uint32_t)stack;
	DebugOutput("[TASKING] \"%s\" created with PID %d\n", p->name, p->pid);
	return p;
}

Process * createUserProcess(char * name, uint32_t addr, int argc, char * argv[])
{
	if (__cpid__ >= CONFIG_NPROC)
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
	p->not_ready = true;
	// now time for the register stuff :)
	p->context.eax = 0;
	p->context.ebx = 0;
	p->context.ecx = 0;
	p->context.edx = 0;
	p->context.esp = (uint32_t)malloc(PAGE_SIZE);
	p->context.ebp = p->context.esp;
	p->context.esi = 0;
	p->context.edi = 0;
	p->context.eip = (uint32_t)addr;
	p->context.eflags = 0x00000202;

	p->context.cs = 0x0018;
	p->context.ds = 0x0020;
	p->context.fs = 0x0020;
	p->context.es = 0x0020;
	p->context.gs = 0x0020;


	asm volatile("mov %%cr3, %%eax" : "=a"(p->context.cr3));
	uint32_t * stack = (uint32_t *)(p->context.esp + PAGE_SIZE);
	p->stack = p->context.esp;
	*--stack = argc;
	for(int i = argc; i == 0; i--)
	{
		*--stack = (uint32_t)argv[i];
	}
	*--stack = (uint32_t)p->context.eflags;				// eflags
	*--stack = (uint32_t)p->context.cs;						// cs
	*--stack = (uint32_t)p->context.eip;					// eip
	*--stack = (uint32_t)p->context.eax;					// eax
	*--stack = (uint32_t)p->context.ebx;				  // ebx
	*--stack = (uint32_t)p->context.ecx;					// ecx;
	*--stack = (uint32_t)p->context.edx;					// edx
	*--stack = (uint32_t)p->context.esi;					// esi
	*--stack = (uint32_t)p->context.edi;					// edi
	*--stack = (uint32_t)p->context.ebp;          // ebp
	*--stack = (uint32_t)p->context.ds;					  // ds
	*--stack = (uint32_t)p->context.fs;					  // fs
	*--stack = (uint32_t)p->context.es;					  // es
	*--stack = (uint32_t)p->context.gs;					  // gs
	p->context.esp = (uint32_t)stack;
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
	CurrentProcess->not_ready = false;
	asm volatile("mov %%eax, %%esp" :: "a"(CurrentProcess->context.esp));
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
	asm volatile("iret");
}

void preempt_now()
{
	if (!tasking_enabled)
		return;
	//asm volatile("int $0x81");
  preempt();
}

void wakeup(Process * p)
{
	if (p->pid > 1)
	{
		InterruptsLock();
		DebugOutput("[TASKING] Waking up PID %d\n", p->pid);
  	p->state = PROCESS_ALIVE;
		InterruptsRelease();
	}
}

void sleep(Process * p)
{
	if (p->pid > 1)
	{
		InterruptsLock();
		DebugOutput("[TASKING] Putting PID %d to sleep\n", p->pid);
  	p->state = PROCESS_SLEEP;
		p->not_ready = true;
		InterruptsRelease();
		preempt_now();
	}
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

/*
static void task_exit()
{
	register uint32_t val __asm__ ("eax");
	printm("PID %d exited with return code %d", getCurrentProcess()->pid, val);
	__kill__();
	asm("hlt");
}*/

void __notify__(uint32_t sig)
{
	switch (sig)
	{
	case SIGTERM:
		printm("notifying process with SIGTERM...\n");
		__kill__();
		break;
	case SIGSEGV:
		printm("notifying process with SIGSEGV...\n");
		__kill__();
		break;
	case SIGILL:
		printm("notifying process with SIGILL\n", CurrentProcess->pid);
		__kill__();
		break;
	case SIGINT:
		DebugOutput("[TASKING] Task received SIGINT\n");
		break;
	}
}

void initTasking()
{
	DebugOutput("[TASKING] Max amount of tasks: %d\n", CONFIG_NPROC);
	KernelProcess = createProcess("lunaridle", (uint32_t)kthread);
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
	if (__cpid__ >= CONFIG_NPROC)
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
	asm volatile("mov %%esp, %%eax" : "=a"(CurrentProcess->context.esp));
	CurrentProcess = CurrentProcess->next;
	if (CurrentProcess->not_ready)
	{
		/*
			if the process is sleeping, we move on to the next task
		*/
		if (CurrentProcess->state != PROCESS_SLEEP)
		{
			reload_context();
			return;
		}
		else
		{
			CurrentProcess = CurrentProcess->next;
    }
  }
	// pop all of next process' registers off of its stack
	asm volatile("mov %%eax, %%cr3" :: "a"(CurrentProcess->context.cr3));
	asm volatile("mov %%eax, %%esp" :: "a"(CurrentProcess->context.esp));
	asm volatile("pop %gs");
	asm volatile("pop %fs");
	asm volatile("pop %es");
	asm volatile("pop %ds");
	asm volatile("pop %ebp");
	asm volatile("pop %edi");
	asm volatile("pop %esi");
	asm volatile("pop %ebx");
	InterruptsRelease();
}
