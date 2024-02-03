section .text
  global InstallGDT

InstallGDT:
  mov eax, [esp + 4]  ; Get the GDT pointer that we passed
  lgdt [eax]          ; Load the GDT into memory

  mov ax, 0x10        ; Kernel data segment
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax

  jmp 0x08 : far_jump   ; Do a far jump

far_jump:
  ret