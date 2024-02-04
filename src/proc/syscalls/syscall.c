#include <stdint.h>
#include <kernel/isr.h>
#include <kernel/fs/vfs.h>
#include <kernel/syscall.h>
#include <kernel/cpu.h>
#include <kernel/task.h>
#include <kernel/mm/malloc.h>
#include <kernel/debug.h>
#include <limits.h>
#include <kernel/sys/utsname.h>
#include <kernel/errno.h>
#include <kernel/fs/devfs.h>
#include <kernel/mm/paging.h>
#include <common.h>
#include <kernel/elf.h>

#define SYSCALL_MAX 32

void sys_exit(int error_code)
{
  DebugOutput("[SYSCALL] sys_exit (pid %d, error code %d)\n", getCurrentProcess()->pid, error_code);
  /* First, get the current process */
  Process * this = getCurrentProcess();
  __kill__();
}

int sys_fork()
{
  DebugOutput("[SYSCALL] sys_fork (pid %d)\n", getCurrentProcess()->pid);
  extern uint32_t __cpid__;
  if (InterruptsEnabled() == true)
    InterruptsLock();
  Process * parent = getCurrentProcess();
  Process * child = (Process *)malloc(sizeof(Process));
  child->pid =  parent->pid + 1;
  child->esp = child->stack = 0;
  child->eip = 0;
  asm volatile("mov %%cr3, %%eax" : "=a"(child->cr3));
  child->next = 0;
  uint32_t eip = dump_eip();
  if (getCurrentProcess() == parent)
  {
    uint32_t esp, ebp;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    asm volatile("mov %%ebp, %0" : "=r"(ebp));
    child->esp = esp;
    child->eip = eip;
    child->stack = ebp;
    DebugOutput("[SYS_FORK] Forked new process with PID %d\n", child->pid);
    return child->pid;
  }
  else
  {
    return 0;
  }
}

int sys_uname(struct utsname * uname)
{
  struct utsname dummy;
  DebugOutput("[SYSCALL] sys_uname (pid %d)\n", getCurrentProcess()->pid);
  if (uname == NULL)
  {
    DebugOutput("[SYS_UNAME] Error: did not give utsname structure!\n");
    return -1;
  }
  memcpy(&uname->sysname, UTSNAME_SYSNAME, sizeof(dummy.sysname));
  memcpy(&uname->nodename, UTSNAME_NODENAME, sizeof(dummy.nodename));
  memcpy(&uname->release, UTSNAME_RELEASE, sizeof(dummy.release));
  memcpy(&uname->version, UTSNAME_VERSION, sizeof(dummy.version));
  memcpy(&uname->machine, UTSNAME_MACHINE, sizeof(dummy.machine));
  return 0;
}

pid_t sys_getpid()
{
  /* 
    Get the current running process and get its PID
    It will always be the one doing the syscall.
  */
  DebugOutput("[SYSCALL] sys_getpid (pid %d)\n", getCurrentProcess()->pid);
  return getCurrentProcess()->pid;
}

int sys_write(int fd, char * buf, uint32_t count)
{
  device_node_t tty;
  tty = devfs_get_device_node(devfs_get_device_byname("tty0"));
  DebugOutput("[SYSCALL] sys_write (pid %d, fd %d, count %d)\n", getCurrentProcess()->pid, fd, count);
  CHECK_FD(fd);
  switch (fd)
  {
    case 1:
      char * str = (char *)malloc(count + 1);
      memcpy(str, buf, count);
      devfs_write(&tty, 0, strlen(str), str);
      free(str);
      break;
    case 2:
      break;
    default:
      break;
  }
  return 0;
}

int sys_read(int fd, char * buf, uint32_t count)
{
  device_node_t tty;
  tty = devfs_get_device_node(devfs_get_device_byname("tty0"));
  DebugOutput("[SYSCALL] sys_read (pid %d, fd %d, count %d)\n", getCurrentProcess()->pid, fd, count);
  CHECK_FD(fd);
  switch (fd)
  {
    case 1:
      break;
    case 2:
      devfs_read(&tty, 0, count, (uint8_t)buf);
      break;
    default:
      break;
  }
  return 0;
}

void sys_open(const char * file, int flags, int mode)
{
  
}

int sys_execve(const char * path, const char * argv[], const char * envp[])
{
  return -ENOSYS;
}

static void SyscallHandler(REGISTERS * regs)
{
  InterruptsLock();
  switch (regs->eax)
  {
    case SYSEXIT:
      sys_exit((int)regs->ebx);
      break;
    case SYSFORK:
      sys_fork();
      break;
    case GETPID:
      sys_getpid();
      break;
    case SYSREAD:
      sys_read((int)regs->ebx, (char *)regs->ecx, regs->edx);
      break;
    case SYSWRITE:
      sys_write((int)regs->ebx, (char *)regs->ecx, regs->edx);
      break;
    case SYSOPEN:
      break;
    case 0x0b:
      sys_execve((const char *)regs->ebx, (const char **)regs->ecx, (const char **)regs->edx);
      break;
    default:
      DebugOutput("[SYSCALL] Unknown syscall received\n");
      DebugOutput("[SYSCALL] EAX: 0x%x\n", regs->eax);
      break;
  }
  InterruptsRelease();
}

void EnableSystemCalls()
{
  RegisterInterrupt(0x80, SyscallHandler);
}