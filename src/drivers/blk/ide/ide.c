#include <stdint.h>
#include <lunaris/mm.h>
#include <lunaris/cpu.h>
#include <lunaris/isr.h>
#include <lunaris/blk/ide.h>
#include <sys/io.h>
#include <common.h>

uint8_t boot_disk = 0;
uint8_t ata_buf[512];
uint8_t ata_buf2[512];

void prepareDisk(int disk, int address)
{
	outb(0x1F1,0x00);
	outb(0x1F2,0x01);
	outb(0x1F3, (uint8_t)address);
	outb(0x1F4, (uint8_t)(address >> 8));
	outb(0x1F5, (uint8_t)(address >> 16));
	outb(0x1F6, 0xE0 | (disk << 4) | ((address >> 24) & 0x0F));
	outb(0x1F7, 0x20);
	while (!(inb(0x1F7) & 0x08));
}

void readSector(int disk, int address, uint8_t *sect)
{
	prepareDisk(disk, address);
	for(int i = 0; i < 256; i++)
	{
		uint16_t tmp = inw(0x1F0);
		sect[i * 2] = (uint8_t)tmp;
		sect[i * 2 + 1] = (uint8_t)(tmp >> 8);
	}
}

void writeSector(int disk, int address, const uint8_t *sect)
{
  prepareDisk(disk, address);

  // Command for writing to disk
  outb(0x1F7, 0x30);

  for (int i = 0; i < 256; i++) 
	{
    uint16_t tmp = (uint16_t)(sect[i * 2 + 1] << 8 | sect[i * 2]);
    outw(0x1F0, tmp);
  }
}

void writeSectors(int disk, int address, int sectors, uint8_t *sect)
{
	for(int i = 0; i < sectors; i++)
	{
		writeSector(disk, address + i, sect + i * 512);
	}
}

void readSectors(int disk, int address, int sectors, uint8_t *sect)
{
	for(int i = 0; i < sectors; i++)
	{
		readSector(disk, address + i, sect + i * 512);
	}
}

int getFirstPartition(int disk)
{
	prepareDisk(disk, 0);
	uint16_t pos = 0;
	for(int i = 0; i < 256; i++)
	{
		uint16_t tmpword = inw(0x1F0);
		if(i == 227)
		{
			pos = tmpword;
		}
	}
	return pos;
}
