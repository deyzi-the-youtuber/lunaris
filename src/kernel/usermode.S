section .text
  global move_to_usermode
  global start_init

section .data 
  arg1: db "/init", 0
  argv: dd arg1, 0

start_init:
  push ebp
  mov ebp, esp

  mov eax, 11
  mov ebx, arg1
  mov ecx, argv
  mov edx, 0
  int 0x80

  pop ebp
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
  call start_init
  ret


; addr2line -a -f -e lunaris
; ^ ^
; This is used for debugging purposes
