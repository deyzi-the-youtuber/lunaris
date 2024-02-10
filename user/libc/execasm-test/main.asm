[bits 32]

; first userspace program
; loaded from the disk!

jmp _start

section .data
  msg: db "Welcome to Lunaris!", 0
  msg_len: equ $-msg

_start:
  ; write string to stdout
  mov eax, 0x4
  mov ebx, 0x1
  mov ecx, msg
  mov edx, msg_len
  int 0x80

  ; exit
  mov eax, 0x1
  mov ebx, 0x0
  int 0x80

  ; wait for the program to exit
  jmp $