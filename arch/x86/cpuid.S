section .text
  global check_cpuid

check_cpuid:
  push ebx
  pushfd
  pop eax
  mov ebx, eax
  xor eax, 0x200000
  push eax
  popfd
  cmp eax, ebx
  jz .cpuid_no_support
  mov eax, 1
  pop ebx
  ret

.cpuid_no_support:
  xor eax, eax
  pop ebx
  ret

