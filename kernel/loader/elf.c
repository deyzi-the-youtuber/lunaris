#include <kernel/elf.h>
#include <kernel/debug.h>
#include <kernel/mm/malloc.h>
#include <kernel/mm/paging.h>
#include <kernel/task.h>
#include <stdint.h>
#include <common.h>

static uint32_t last_load_loc = 0x400000;

bool elf_is_valid(uint8_t * data)
{
  elf_header_t * header = (elf_header_t *)data;
  /* check the file magic */
  DebugOutput("[TEST] %x %x %x %x\n", header->e_ident[0], header->e_ident[1], header->e_ident[2], header->e_ident[3]);
	if(header->e_ident[0] == 0x7f && header->e_ident[1] == 'E' && header->e_ident[2] == 'L' && header->e_ident[3] == 'F')
	{
		return true;
	}
  return false;
}

void elf_get_info(elf_header_t * elf_header)
{
  DebugOutput("[ELF_LOADER] File info: %s, %s, %s\n", 
    elf_header->e_ident[4] == 1 ? "64-bit" : "32-bit",
    elf_header->e_ident[5] == 1 ? "Little-Endian" : "Big-Endian",
    elf_header->e_ident[6] == 1 ? "ELF 1.0" : "Unknown ELF version"
  );
}

static uint32_t elf_get_free_load_loc()
{
	last_load_loc += 0x400000;
	return last_load_loc;
}

uint8_t elf_load_file(uint8_t * data)
{
  elf_header_t * header = (elf_header_t *)data;
  /*
    if the elf file is not valid, abort the file execution process
  */
  if (!elf_is_valid(data))
  {
    DebugOutput("[ELF_LOADER] ELF file magic is invalid!\n");
    return 0;
  }
  elf_get_info(header);
	if(header->e_type != 2)
	{
		DebugOutput("File is not executable!\n");
		return 0;
	}
  uint32_t load_loc = elf_get_free_load_loc();
  uint32_t cr3 = pmalloc(4096);
  elf_program_header_t * program_header = (elf_program_header_t *)(data + header->e_phoff);
	for(int i = 0; i < header->e_phnum; i++, program_header++)
	{
		switch(program_header->p_type)
		 {
		 	case 0:
		 		break;
		 	case 1:
		 		DebugOutput("LOAD: offset 0x%x vaddr 0x%x paddr 0x%x filesz 0x%x memsz 0x%x\n",
		 				program_header->p_offset, program_header->p_vaddr, program_header->p_paddr, program_header->p_filesz, program_header->p_memsz);
		 		MapVirtualToPhys(program_header->p_vaddr, load_loc);
		 		memcpy(program_header->p_vaddr, data + program_header->p_offset, program_header->p_filesz);
		 		break;
		 	default:
		 	 DebugOutput("Unsupported p_type! Bail out!\n");
		 	 return 0;
		 }
	}
  return START("elf32", header->e_entry);
}