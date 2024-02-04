section .text
  global LoadTSS

LoadTSS:
  mov ax, (5 * 8) | 0
  ltr ax
  ret