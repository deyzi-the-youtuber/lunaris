%define SYSCALL(syscall_number, function_name) \
global function_name \
function_name: \
  mov eax, syscall_number \
  int 0x80 \
  ret \

section .text
  SYSCALL(1, exit)
  SYSCALL(2, fork)
  SYSCALL(3, read)
  SYSCALL(4, write)
  SYSCALL(5, open)
  SYSCALL(20, getpid)