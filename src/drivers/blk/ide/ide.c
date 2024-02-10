#include <stdint.h>
#include <kernel/mm/malloc.h>
#include <kernel/cpu.h>
#include <kernel/isr.h>
#include <kernel/blk/ide.h>
#include <kernel/sys/io.h>
#include <common.h>

uint8_t boot_disk = 0;
uint8_t ata_buf[512];
uint8_t ata_buf2[512];

void prepareDisk(int disk, int address)
{
	IoPortByteWrite(0x1F1,0x00);
	IoPortByteWrite(0x1F2,0x01);
	IoPortByteWrite(0x1F3, (uint8_t)address);
	IoPortByteWrite(0x1F4, (uint8_t)(address >> 8));
	IoPortByteWrite(0x1F5, (uint8_t)(address >> 16));
	IoPortByteWrite(0x1F6, 0xE0 | (disk << 4) | ((address >> 24) & 0x0F));
	IoPortByteWrite(0x1F7, 0x20);
	while (!(IoPortByteRead(0x1F7) & 0x08));
}

void readSector(int disk, int address, uint8_t *sect)
{
	prepareDisk(disk, address);
	for(int i = 0; i < 256; i++)
	{
		uint16_t tmp = IoPortWordRead(0x1F0);
		sect[i * 2] = (uint8_t)tmp;
		sect[i * 2 + 1] = (uint8_t)(tmp >> 8);
	}
}

void writeSector(int disk, int address, const uint8_t *sect)
{
  prepareDisk(disk, address);

  // Command for writing to disk
  IoPortByteWrite(0x1F7, 0x30);

  for (int i = 0; i < 256; i++) 
	{
    uint16_t tmp = (uint16_t)(sect[i * 2 + 1] << 8 | sect[i * 2]);
    IoPortWordWrite(0x1F0, tmp);
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
		uint16_t tmpword = IoPortWordRead(0x1F0);
		if(i == 227)
		{
			pos = tmpword;
		}
	}
	return pos;
}