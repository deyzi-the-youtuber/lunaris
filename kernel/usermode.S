section .text
  global usermode_test
  global move_to_usermode
  global usermode_start_init

section .data 
  file: db "/init", 0
  argv: db "/init", 0

usermode_start_init:
  ; because we are in usermode, we cant disable, or enable interrupts
  ; that's all up to the kernel at this point.
  mov eax, 0x0b ; sys_execve
  mov ebx, file ; file to execute
  mov ecx, argv ; unused
  mov edx, 0x00 ; unused
  int 0x80 ; pass info to the kernel

  ret

move_to_usermode:
  ; turn off interrupts
  cli
	; setup data segment
  mov ax, 0x23
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  mov eax, esp
  push dword 0x23
  push dword eax
  ; code segment selector, rpl = 3
  pushfd
  pop eax
  or eax, 0x200
  push eax
  push dword 0x1b
  push .usermode
  iretd
.usermode:
  ret


; addr2line -a -f -e lunaris
; ^ ^
; This is used for debugging purposes