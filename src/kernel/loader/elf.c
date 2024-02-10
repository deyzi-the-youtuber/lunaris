#include <kernel/elf.h>
#include <kernel/debug.h>
#include <kernel/mm/malloc.h>
#include <kernel/mm/paging.h>
#include <kernel/task.h>
#include <stdint.h>
#include <common.h>

uint32_t load_addr = 0x400000;

/* work in progress */

int elf_is_valid(uint8_t * buffer)
{
  DebugOutput("[ELF] Searching for signature...\n");
	elf_header_t * header = (elf_header_t *)buffer;
	if(header->e_ident[0] == 0x7f && header->e_ident[1] == 'E' && header->e_ident[2] == 'L' && header->e_ident[3] == 'F')
	{
		/* ELF file is valid */
		DebugOutput("[ELF] Signature is valid!\n");
		return 1;
	}
	DebugOutput("[ELF] Invalid signature!\n");
	return 0;
}

static void elf_print_info(elf_header_t * header)
{
	DebugOutput("[ELF] File information:\n");
	DebugOutput(" [+] Format: %s\n", header->e_ident[4] ? "32-Bit" : "64-Bit");
	DebugOutput(" [+] Endianness: %s\n", header->e_ident[5] ? "Little Endian" : "Big Endian");
	DebugOutput(" [+] ELF Version: %d\n", header->e_ident[6]);
	DebugOutput(" [+] OS ABI: 0x%x\n", header->e_ident[7]);
	DebugOutput(" [+] Object file type: 0x%x\n", header->e_type);
	DebugOutput(" [+] Machine: 0x%x\n", header->e_machine);
	DebugOutput(" [+] Entry point: 0x%x\n", header->e_entry);
}

uint32_t elf_get_unused_load_addr()
{
	load_addr += 0x400000;
	return load_addr;
}

void elf_execute(char * name, uint8_t * file, int argc, char * argv[])
{
	elf_header_t * hdr = (elf_header_t *)file;
	elf_print_info(hdr);
	elf_program_header_t * prghdr = (elf_program_header_t *)(file + hdr->e_phoff);
	uint32_t load_location;
	for(int i = 0; i < hdr->e_phnum; i++, prghdr++)
	{
		switch (prghdr->p_type)
		{
			/* load */
			case 1:
				DebugOutput("[ELF] Program header:\n");
				DebugOutput(" [+] Load offest: 0x%x\n", prghdr->p_offset);
				load_location = file + prghdr->p_offset;
				break;
			default:
				break;
		}
	}
	DebugOutput("[ELF] Loading file at 0x%x\n", load_location + hdr->e_entry);
	addProcess(createProcess(name, load_location + hdr->e_entry, argc, argv));
}