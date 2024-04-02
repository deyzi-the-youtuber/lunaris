#include <stdint.h>
#include <lunaris/isr.h>
#include <fs/vfs.h>
#include <lunaris/syscall.h>
#include <lunaris/cpu.h>
#include <lunaris/task.h>
#include <lunaris/mm.h>
#include <lunaris/debug.h>
#include <limits.h>
#include <sys/utsname.h>
#include <errno.h>
#include <fs/devfs.h>
#include <fs/ext2.h>
#include <common.h>
#include <lunaris/elf.h>
#include <sys/types.h>
#include <lunaris/module.h>

MODULE("syscall")

#define SYSCALL_MAX 32

static void _trace_return(uint32_t result)
{
#ifdef CONFIG_KERNEL_TRACE
  DebugOutput("[KTRACE] PID %d ---- return: %d\n", getCurrentProcess()->pid, result);
#endif
}

void sys_exit(int error_code)
{
  DebugOutput("[SYSCALL] sys_exit (pid %d, error code %d)\n", getCurrentProcess()->pid, error_code);
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
  child->context.esp = child->stack = 0;
  child->context.eip = 0;
  asm volatile("mov %%cr3, %%eax" : "=a"(child->context.cr3));
  child->next = 0;
  uint32_t eip = dump_eip();
  if (getCurrentProcess() == parent)
  {
    uint32_t esp, ebp;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    asm volatile("mov %%ebp, %0" : "=r"(ebp));
    child->context.esp = esp;
    child->context.eip = eip;
    child->stack = esp;
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
  if (!uname)
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
  pid_t pid = getCurrentProcess()->pid;
  DebugOutput("[SYSCALL] get_pid (pid %d)\n", pid);
  return pid;
}

void sys_write(int fd, char * buf, uint32_t count)
{
  device_node_t tty;
  tty = devfs_get_device_node(devfs_get_device_byname("tty0"));
  CHECK_FD(fd);
  switch (fd)
  {
    case 1:
      char * str = (char *)malloc(count + 1);
      memcpy(str, buf, count);
      devfs_write((uint32_t)&tty, strlen(str), (uint8_t *)str);
      free(str);
      break;
    case 2:
      break;
    default:
      break;
  }
}

int sys_read(int fd, char * buf, uint32_t count)
{
  device_node_t tty;
  tty = devfs_get_device_node(devfs_get_device_byname("tty0"));
  DebugOutput("[SYSCALL] sys_read (pid %d, fd %d, count %d)\n", getCurrentProcess()->pid, fd, count);
  switch (fd)
  {
    case 1:
      break;
    case 2:
      devfs_read((uint32_t)&tty, count, (uint8_t *)buf);
      return strlen(buf);
      break;
    default:
      /* file descriptor handling logic */
      break;
  }
  return 0;
}

static int is_ext2(char file[])
{
  int i = 0;
  char * token = strtok((char *)file, "/");
  while (token)
  {
    /* checks if directory / file is absolute or relative */
    if (strcmp(token, "dev") == 0 && (i == 0 || i == 1))
    {
      return 0;
      break;
    }
    token = strtok(NULL, "/");
    i++;
  }
  return 1;
}

int sys_open(const char * file, int flags, uint16_t mode)
{
  char fn[255];
  int fd;
  strcpy(fn, (char *)file);
  DebugOutput("[SYSCALL] sys_open (pid %d, flags 0x%x, mode 0x%x)\n", getCurrentProcess()->pid, flags, mode);
  DebugOutput("[SYS_OPEN] %s\n", file);
  bool is_ext2_prefix = is_ext2(fn);
  vfs_node_t * file_node;
  if (is_ext2_prefix)
  {
    ext2_get_file((char *)file, file_node);
    if ((int)file_node == -ENOENT)
    {
      DebugOutput("[SYS_OPEN] %s: no such file or directory\n", file);
      return -1;
    }
    if (file_node->flags & VFS_DIRECTORY)
    {
      DebugOutput("[SYS_OPEN] %s: is a directory\n", file);
      return -1;
    }
    fd = find_fd(file_node);
  }
  else
  {
    int i = 0;
    char * token = strtok((char *)file, "/");
    while (token != NULL)
    {
      if (strcmp(token, "dev") != 0 && (i > 0 || i > 1))
      {
        break;
      }
      token = strtok(NULL, "/");
      i++;
    }
    fd = find_fd(0);
  }
  DebugOutput("[SYS_OPEN] %s: file descriptor: %d\n", file, fd);
  return fd + 2;
}

int sys_execve(const char * path, const char * argv[], const char * envp[])
{
  DebugOutput("[SYSCALL] sys_execve (pid %d, path %s)\n", getCurrentProcess()->pid, path);
  vfs_node_t * node;
  if (!ext2_get_file((char *)path, node))
  {
    DebugOutput("[EXECVE] %s: not found\n", path);
    return -ENOENT;
  }
  uint8_t * program_buffer;
  ext2_read(node, program_buffer);
  if (!elf_is_valid(program_buffer))
  {
    return -ENOEXEC;
  }
  int argc = 0;
  /* sizeof(argv) / sizeof(argv[0]) does not work for some odd reason */
  for(int i = 0; argv[i] != NULL; i++)
  {
    argc++;
  }
  elf_execute(path, program_buffer, argc, (char **)&argv);
  return 1;
}

static void SyscallHandler(REGISTERS * regs)
{
  int r = 0;
  switch (regs->eax)
  {
    case SYSEXIT:
      sys_exit((int)regs->ebx);
      break;
    case SYSFORK:
      r = sys_fork();
      break;
    case GETPID:
      r = sys_getpid();
      break;
    case SYSREAD:
      r = sys_read((int)regs->ebx, (char *)regs->ecx, regs->edx);
      break;
    case SYSWRITE:
      sys_write((int)regs->ebx, (char *)regs->ecx, regs->edx);
      break;
    case SYSOPEN:
      r = sys_open((const char *)regs->ebx, (int)regs->ecx, (uint16_t)regs->edx);
      break;
    case 0x0b:
      r = sys_execve((const char *)regs->ebx, (const char **)regs->ecx, (const char **)regs->edx);
      break;
    default:
      printm("received unknown or unimplemented syscall %d\n", regs->eax);
      DebugOutput("[SYSCALL] Unimplemented syscall 0x%x\n", regs->eax);
      r = -ENOSYS;
      break;
  }
  _trace_return(r);
}

void EnableSystemCalls()
{
  RegisterInterrupt(0x80, (uintptr_t)SyscallHandler);
}
