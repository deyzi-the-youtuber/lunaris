#include <stdint.h>

#ifndef __MBR_H
#define __MBR_H

typedef struct
{
	uint8_t  status;
	uint8_t  chs_first_sector[3];
	uint8_t  type;
	uint8_t  chs_last_sector[3];
	uint32_t lba_first_sector;
	uint32_t sector_count;
}Partition;

typedef struct
{
	uint8_t     boostrap[446];
  Partition 	partitions[4];
	uint8_t     signature[2];
} __attribute__((packed)) MBR;

int get_num_partitions(int disk);

#endif