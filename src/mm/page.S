section .text
  global LoadPageDirectory

LoadPageDirectory:
	push ebp
	mov ebp, esp
	push dword [ebp + 8]
	pop eax
	
	pusha
	mov cr3, eax

	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax
	popa
	
	mov esp, ebp
	pop ebp
	ret