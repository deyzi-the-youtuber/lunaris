[org 0x7c00] ; where the bootsector loads the program
[bits 16] ; 16-bit real mode

_boot_start:
  mov ax, 0x0
  mov ds, ax
  mov es, ax

  mov ss, ax
  mov sp, 0x7c00

  mov si, msg
  call string_print
  jmp $


string_print:
  mov bx, 0x0
  lodsb ; load byte from the string given
  cmp al, 0x0 ; if null byte is reached, exit
  je _string_print_end
  mov ah, 0x0e ; screen print
  int 0x10 ; bios interrupt
  jmp string_print ; loop

_string_print_end:
  ret

msg: db "Error reading disk", 0

times 510-($-$$) db 0 ; padding
dw 0xAA55 ; boot sector magic (signature)