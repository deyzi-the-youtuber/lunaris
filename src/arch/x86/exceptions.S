section .text
  extern ExceptionHandlerISR
  global Exception_0
  global Exception_1
  global Exception_2
  global Exception_3
  global Exception_4
  global Exception_5
  global Exception_6
  global Exception_7
  global Exception_8
  global Exception_9
  global Exception_10
  global Exception_11
  global Exception_12
  global Exception_13
  global Exception_14
  global Exception_15
  global Exception_16
  global Exception_17
  global Exception_18
  global Exception_19
  global Exception_20
  global Exception_21
  global Exception_22
  global Exception_23
  global Exception_24
  global Exception_25
  global Exception_26
  global Exception_27
  global Exception_28
  global Exception_29
  global Exception_30
  global Exception_31
  global Exception_128


ExceptionHandler:
  pusha
  mov ax, ds
  push eax
  
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  call ExceptionHandlerISR

  pop ebx
  mov ds, bx
  mov es, bx
  mov fs, bx
  mov gs, bx

  popa
  add esp, 0x8

  sti
  iret


Exception_0:
  cli
  push byte 0
  push 0
  jmp ExceptionHandler

Exception_1:
  cli
  push byte 0
  push 1
  jmp ExceptionHandler

Exception_2:
  cli
  push byte 0
  push 2
  jmp ExceptionHandler

Exception_3:
  cli
  push byte 0
  push 3
  jmp ExceptionHandler

Exception_4:
  cli
  push byte 0
  push 4
  jmp ExceptionHandler

Exception_5:
  cli
  push byte 0
  push 5
  jmp ExceptionHandler

Exception_6:
  cli
  push byte 0
  push 6
  jmp ExceptionHandler

Exception_7:
  cli
  push byte 0
  push 7
  jmp ExceptionHandler

Exception_8:
  cli
  push 8
  jmp ExceptionHandler

Exception_9:
  cli
  push byte 0
  push 9
  jmp ExceptionHandler

Exception_10:
  cli
  push 10
  jmp ExceptionHandler

Exception_11:
  cli
  push 11
  jmp ExceptionHandler

Exception_12:
  cli
  push 12
  jmp ExceptionHandler

Exception_13:
  cli
  push 13
  jmp ExceptionHandler

Exception_14:
  cli
  push 14
  jmp ExceptionHandler

Exception_15:
  cli
  push byte 0
  push 15
  jmp ExceptionHandler

Exception_16:
  cli
  push byte 0
  push 16
  jmp ExceptionHandler

Exception_17:
  cli
  push byte 0
  push 17
  jmp ExceptionHandler

Exception_18:
  cli
  push byte 0
  push 18
  jmp ExceptionHandler

Exception_19:
  cli
  push byte 0
  push 19
  jmp ExceptionHandler

Exception_20:
  cli
  push byte 0
  push 20
  jmp ExceptionHandler

Exception_21:
  cli
  push byte 0
  push 21
  jmp ExceptionHandler

Exception_22:
  cli
  push byte 0
  push 22
  jmp ExceptionHandler

Exception_23:
  cli
  push byte 0
  push 23
  jmp ExceptionHandler

Exception_24:
  cli
  push byte 0
  push 24
  jmp ExceptionHandler

Exception_25:
  cli
  push byte 0
  push 25
  jmp ExceptionHandler

Exception_26:
  cli
  push byte 0
  push 26
  jmp ExceptionHandler

Exception_27:
  cli
  push byte 0
  push 27
  jmp ExceptionHandler

Exception_28:
  cli
  push byte 0
  push 28
  jmp ExceptionHandler

Exception_29:
  cli
  push byte 0
  push 29
  jmp ExceptionHandler

Exception_30:
  cli
  push byte 0
  push 30
  jmp ExceptionHandler

Exception_31:
  cli
  push byte 0
  push 31
  jmp ExceptionHandler

Exception_128:
  cli
  push byte 0
  push 128
  jmp ExceptionHandler