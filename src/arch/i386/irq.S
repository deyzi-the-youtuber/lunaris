section .text
  extern IRQHandlerISR

HandlerIRQ:
  pusha
  mov ax, ds
  push eax

  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  push esp
  call IRQHandlerISR
  pop esp

  pop eax
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  popa
  add esp, 0x8

  sti
  iret


%macro IRQ 2
global irq_%1
irq_%1:
  cli
  push byte 0
  push byte %2
  jmp HandlerIRQ
%endmacro


IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47
