#include <stdint.h>
#include <kernel/sys/io.h>
#include <kernel/pci.h>
#include <kernel/mm/malloc.h>
#include <kernel/video/vga.h>
#include <kernel/debug.h>
#include <common.h>

uint16_t pci_config_read_short(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset)
{
  uint32_t address;
  uint32_t lbus  = (uint32_t)bus;
  uint32_t lslot = (uint32_t)slot;
  uint32_t lfunc = (uint32_t)function;
  uint16_t tmp = 0;
  address = (uint32_t)((lbus << 16) | (lslot << 11)
    | (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
  IoPortDwordWrite(PCI_CONFIG_ADDRESS, address);
  tmp = (uint16_t)((IoPortDwordRead(PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xFFFF);
  return tmp;
}

void pci_enumerate()
{
  DebugOutput("[PCI] Bruteforce start\n");
  uint16_t vendor;
  uint16_t dev;
  uint16_t class;
  uint16_t subclass;
  for (int bus = 0; bus < 256; bus++)
  {
    for (int dev = 0; dev < 32; dev++)
    {
      for (int function = 0; function < 8; function++)
      {
        vendor = pci_get_vendor(bus, dev, function);
        dev = pci_get_device(bus, dev, function);
        class = pci_get_class(bus, dev, function);
        subclass = pci_get_subclass(bus, dev, function);
        if (vendor == 0xFFFF)
        {
          /* invalid device */
          continue;
        }
        DebugOutput("[PCI] Vendor: 0x%x Device: 0x%x Class: 0x%x Subclass: 0x%x\n", vendor, dev, class, subclass);
      }
    }
  }
}

void pci_init()
{
  pci_enumerate();
}

uint16_t pci_get_vendor(uint16_t bus, uint16_t device, uint16_t function)
{
  uint32_t r0 = pci_config_read_short(bus, device, function, 0);
  return r0;
}

uint16_t pci_get_device(uint16_t bus, uint16_t device, uint16_t function)
{
  uint32_t r0 = pci_config_read_short(bus, device, function, 2);
  return r0;
}

uint16_t pci_get_class(uint16_t bus, uint16_t device, uint16_t function)
{
  uint32_t r0 = pci_config_read_short(bus, device, function, 0xA);
  return (r0 & ~0x00FF) >> 8;
}

uint16_t pci_get_subclass(uint16_t bus, uint16_t device, uint16_t function)
{
  uint32_t r0 = pci_config_read_short(bus, device, function, 0xA);
  return (r0 & ~0xFF00);
}

uint16_t pci_check_vendor(uint8_t bus, uint8_t slot)
{
  uint16_t vendor, device;
  /* check if device is valid */
  if ((vendor = pci_config_read_short(bus, slot, 0, 0)) != 0xFFFF)
  {
    device = pci_config_read_short(bus,slot,0,2);
  }return (vendor);
}