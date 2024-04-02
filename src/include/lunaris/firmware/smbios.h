#include <stdint.h>

#ifndef __SMBIOS_H
#define __SMBIOS_H

typedef struct
{
 	int8_t EntryPointString[4];    /* _SM_ */
 	uint8_t Checksum;
 	uint8_t Length;                /* Since version 2.1 of SMBIOS, this is 0x1f */
 	uint8_t MajorVersion;
 	uint8_t MinorVersion;
 	uint16_t MaxStructureSize;
 	uint8_t EntryPointRevision;
 	int8_t FormattedArea[5];
 	int8_t EntryPointString2[5];
 	uint8_t Checksum2;
 	uint16_t TableLength;
 	uint8_t TableAddress;
 	uint16_t NumberOfStructures;
 	uint8_t BCDRevision;
}smbios_entry;

typedef struct
{
 	uint8_t Type;
 	uint8_t Length;
 	uint16_t Handle;
}smbios_header;

enum
{
  BIOS_INFORMATION = 0,
  SYSTEM_INFORMATION = 1,
  SYSTEM_ENCLOSURE = 3,
  CPU_INFORMATION = 4,
  CACHE_INFORMATION = 7,
  SYSTEM_SLOTS = 9,
  PHYSICAL_MEM_ARRAY = 16,
  MEMORY_DEVICE = 17,
  MEM_ARRAY_MAP_DEV = 19
};

int smbios_init();

#endif