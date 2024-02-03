section .text
  global InstallIDT

InstallIDT:
  mov eax, [esp + 4]
  lidt [eax]
  ret