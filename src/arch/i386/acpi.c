#include <lunaris/acpi.h>
#include <sys/io.h>
#include <lunaris/mm.h>
#include <lunaris/debug.h>
#include <common.h>
#include <stdint.h>
#include <lunaris/printk.h>

uint8_t validate_checksum(uint8_t * pointer, uint32_t size)
{
  uint8_t bytes = 0;
  while(size--)
  {
    bytes += *pointer++;
  }
  DebugOutput("[ACPI] Checksum validation returning %d\n", bytes);
  return bytes;
}

static XSDP_t * xsdp_detect()
{
  DebugOutput("[ACPI] XSDP search start\n");
  /* get extended bios data area pointer */
  uint16_t ebda_pointer = *(uint16_t *)0x040E;
  /* extended bios data area */
  char * ebda = ((char *)(uintptr_t) ebda_pointer);
  char * pointer = ebda;
  /*
    we need to find the string "RSD PTR " 
    NEVER forget the space at the end
  */
  while(pointer >= ebda + 0x400)
  {
    /* check for string */
    if (strncmp(pointer, "RSD PTR ", 8) == 0)
    {
      if (!validate_checksum((uint8_t *)pointer, sizeof(XSDP_t)))
      {
        return (XSDP_t *)pointer;
      }
    }
    /*
      the string is always on a 16 byte boundary so we increment the pointer by 16 bytes 
    */
    pointer += 0x10;
  }

  while (pointer < ((char *)0x000FFFFF))
  {
    if (!strncmp(pointer, "RSD PTR ", 8))
      if (!validate_checksum(pointer, sizeof(XSDP_t)))
        return (XSDP_t *)pointer;
        
    pointer += 0x10;
  }
  return (XSDP_t *)NULL;
}

static void * findFACP(void * RootSDT)
{
  DebugOutput("[ACPI] FACP search start\n");
  XSDT_t * xsdt = (XSDT_t *)RootSDT;
  uint32_t header_length = xsdt->h.Length;
  int entries = (header_length - sizeof(xsdt->h)) / sizeof(uint32_t);
  for (int i = 0; i < entries; i++)
  {
    acpi_sdt_header_t * h = (acpi_sdt_header_t *)(uint32_t)xsdt->PointerToOtherSDT[i];
    if (!strncmp(h->Signature, "FACP", 4))
      return (void *)h;
  }
  // No FACP found
  return NULL;
}

int acpi_init()
{
  XSDP_t * xsdp = xsdp_detect();
  if (xsdp == (XSDP_t *)NULL)
  {
    printk("ACPI: XSDP not found!\n");
    DebugOutput("[ACPI] XSDP is NULL!\n");
    return -1;
  }
  printk("ACPI: XSDP found at 0x%x\n", xsdp);
  DebugOutput("[ACPI] Verifying XSDP version...\n");
  switch (xsdp->Revision)
  {
    case 1:
      printk("ACPI: newer ACPI version needed\n");
      DebugOutput("[ACPI] Version is outdated\n");
      return -1;
      break;
    case 2:
      break;
    default:
      printk("ACPI: unknown version!\n");
      DebugOutput("[ACPI] Unknown version\n");
      return -1;
      break;
  }
  void * facp_ptr = findFACP((void *)(uint32_t)xsdp->XsdtAddress);
  return 0;
}
