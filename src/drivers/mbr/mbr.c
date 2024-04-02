#include <lunaris/blk/mbr.h>
#include <lunaris/blk/ide.h>

#define MBR_MAX 4

int get_num_partitions(int disk)
{
  MBR mbr;
  readSector(disk, 0, (uint8_t *)&mbr);
  int p = 0;
  for (int i = 0; i < MBR_MAX; i++)
  {
    if (mbr.partitions[i].status != 0x00)
    {
      p++;
    }
  }
  return p;
}
