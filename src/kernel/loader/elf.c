#include <lunaris/elf.h>
#include <lunaris/debug.h>
#include <lunaris/mm.h>
#include <lunaris/task.h>
#include <stdint.h>
#include <common.h>

/* work in progress */

int elf_is_valid(uint8_t * buffer)
{
	elf_header_t * header = (elf_header_t *)buffer;
	return (header->e_ident[0] == 0x7f && header->e_ident[1] == 'E' && header->e_ident[2] == 'L' && header->e_ident[3] == 'F');
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

void elf_execute(char * name, uint8_t * file, int argc, char * argv[])
{
  uint32_t load_addr;
	elf_header_t * hdr = (elf_header_t *)file;
	elf_print_info(hdr);
	elf_program_header_t * prghdr = (elf_program_header_t *)(file + hdr->e_phoff);
	for(int i = 0; i < hdr->e_phnum; i++, prghdr++)
	{
		switch (prghdr->p_type)
		{
			/* load */
			case 1:
				DebugOutput("[ELF] Program header:\n");
				DebugOutput(" [+] Load offest: 0x%x\n", prghdr->p_offset);
        load_addr = (uint32_t)(file + prghdr->p_offset);
        break;
			default:
				break;
		}
	}
  DebugOutput("[ELF] Executing ELF file...\n");
	addProcess(createProcess(name, load_addr + hdr->e_entry));
}
