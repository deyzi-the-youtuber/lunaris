MAGIC equ 0xe85250d6
KERNEL_ARCH equ 0x0 ; i386 (also has x86_64 compatibility)
BOOTLOADER_MAGIC equ 0x36d76289
KERNEL_STACK_SIZE equ 0x100000

section .text
  global kernel_load
  extern KernelMain

align 8

MultibootHeader:
  dd MAGIC
  dd KERNEL_ARCH
  dd MultibootHeaderEnd - MultibootHeader
  dd -(MAGIC + KERNEL_ARCH + (MultibootHeaderEnd - MultibootHeader))
;FramebufferTag:
;  dw 5
;  dw 1
;  dd 20
;  dd 800
;  dd 600
;  dd 32
;FramebufferTagEnd:
; End tag
  dw 0
  dw 0
  dd 0
MultibootHeaderEnd:

kernel_load:
  mov esp, StackTop
  push dword 0
  popf
  push ebx
  push eax
  call KernelMain

SystemHalt:
  hlt
  jmp SystemHalt

StackBottom:
  resb 1048576
StackTop:
; End of file