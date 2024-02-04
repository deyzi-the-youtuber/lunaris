section .text
  global dump_eip

dump_eip:
  pop eax
  jmp eax