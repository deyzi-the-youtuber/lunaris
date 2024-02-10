#include <stdint.h>
#include <common.h>
#include <kernel/firmware/smbios.h>
#include <kernel/printk.h>
#include <kernel/debug.h>
#include <kernel/errno.h>

smbios_entry * smbios;

static smbios_entry * smbios_locate_entry()
{
  DebugOutput("[SMBIOS] Entry search start\n");
  uint8_t * smbios = (uint8_t *)0xF0000;
  int length, i;
  uint8_t checksum;
  while ((uint32_t)smbios < 0x100000)
  {
    if (strncmp(smbios, "_SM_", 4) == 0) 
    {
      length = smbios[5];
      checksum = 0;
      for(i = 0; i < length; i++)
      {
        checksum += smbios[i];
      }
      DebugOutput("[SMBIOS] Checksum: %d\n", checksum);
      if(checksum == 0)
      {
        DebugOutput("[SMBIOS] Entry point located at 0x%x\n", smbios);
        return (smbios_entry *)smbios;
      }
    }
    smbios += 16;
  }
  DebugOutput("[SMBIOS] Entry search end\n");
  return (smbios_entry *)NULL;
}

static uint32_t smbios_get_table_len(smbios_header * hd)
{
  uint32_t i;
  const char * strtab = (char *)hd + hd->Length;
  // Scan until we find a double zero byte
  for (i = 1; strtab[i - 1] != '\0' || strtab[i] != '\0'; i++);
  return hd->Length + i + 1;
}

int smbios_init()
{
  smbios = smbios_locate_entry();
  if (smbios == (smbios_entry *)NULL)
  {
    PANIC("SMBIOS support required!");
  }
  DebugOutput("[SMBIOS] SMBIOS version %d.%d detected!\n", smbios->MajorVersion, smbios->MinorVersion);
  printk("smbios: smbios %d.%d initializing...\n", smbios->MajorVersion, smbios->MinorVersion);
  DebugOutput("[SMBIOS] SMBIOS %d.%d initializing...\n", smbios->MajorVersion, smbios->MinorVersion);
  return -ENOSYS;
}