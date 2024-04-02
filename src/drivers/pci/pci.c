#include <stdint.h>
#include <sys/io.h>
#include <lunaris/pci.h>
#include <lunaris/mm.h>
#include <lunaris/debug.h>
#include <common.h>
#include <lunaris/printk.h>

void pci_device_probe();

uint16_t pci_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
  uint32_t address;
  uint32_t lbus = (uint32_t)bus;
  uint32_t lslot = (uint32_t)slot;
  uint32_t lfunc = (uint32_t)func;
  uint16_t tmp = 0;

  // Create configuration address as per Figure 1
  address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));

  // Write out the address
  outl(PCI_CONFIG_ADDRESS, address);
  // Read in the data
  // (offset & 2) * 8) = 0 will choose the first word of the 32-bit register
  tmp = (uint16_t)((inl(PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xFFFF);
  return tmp;
}

uint16_t pci_get_class(uint16_t bus, uint16_t device, uint16_t function)
{
  uint32_t r0 = pci_read_word(bus, device, function, PCI_SUBCLASS);
  return (r0 & ~0x00FF) >> 8;
}

uint16_t pci_get_subclass(uint16_t bus, uint16_t device, uint16_t function)
{
  uint32_t r0 = pci_read_word(bus, device, function, PCI_SUBCLASS);
  return (r0 & ~0xFF00);
}

uint16_t pci_get_device(uint16_t bus, uint16_t device, uint16_t function)
{
  uint32_t r0 = pci_read_word(bus, device, function, 2);
  return r0;
}

uint16_t pci_get_vendor(uint16_t bus, uint16_t slot, uint16_t function)
{
  uint16_t vendor_id = pci_read_word(bus, slot, function, 0x00);
  return vendor_id;
}

int pci_device_filter(uint16_t bus, uint16_t slot, uint16_t function)
{
  uint16_t vendor_id = pci_get_vendor(bus, slot, function);
  return !(vendor_id == 0xffff || !vendor_id);
}

void pci_get_details(pci_device * device, uint8_t bus, uint8_t slot, uint8_t function)
{
  device->bus = bus;
  device->slot = slot;
  device->function = function;

  device->class_id = pci_get_class(bus, slot, function);
  device->subclass_id = pci_get_subclass(bus, slot, function);

  device->vendor_id = pci_read_word(bus, slot, function, 0x00);
  device->device_id = pci_read_word(bus, slot, function, PCI_DEVICE_ID);

  device->interface_id = pci_read_word(bus, slot, function, PCI_PROG_IF);

  device->system_vendor_id = pci_read_word(bus, slot, function, PCI_SYSTEM_VENDOR_ID);
  device->system_id = pci_read_word(bus, slot, function, PCI_SYSTEM_ID);

  device->revision = pci_read_word(bus, slot, function, PCI_REVISION_ID);
  device->interrupt = pci_read_word(bus, slot, function, PCI_INTERRUPT_LINE);
}

void pci_init()
{
  pci_enum();
}

void _pci_do_probe(pci_device * dev)
{
  // not implemented yet
}

void pci_enum()
{
  printk("pci: scanning pci devices...\n");
  for (int bus = 0; bus < 256; bus++)
  {
    for (int device = 0; device < 32; device++)
    {
      if (pci_device_filter(bus, device, 0) != 0xFFFF)
      {
        uint16_t classAndSub = pci_read_word(bus, device, 0, 10);
        if ((classAndSub >> 8) < 0x11)
        {
          uint16_t vendor = pci_get_vendor(bus, device, 0);
          uint16_t dev = pci_get_device(bus, device, 0);
          DebugOutput("[PCI] ");
          pci_device_parse((uint8_t)(classAndSub >> 8), (uint8_t)classAndSub, (uint8_t)(pci_read_word(bus, device, 0, 8) >> 8));
          DebugOutput(": vendor: 0x%x, device: 0x%x\n", vendor, dev);
          pci_device * pci_dev;
          pci_get_details(pci_dev, bus, device, 0);
          _pci_do_probe(pci_dev);
        }
      }
    }
  }
}

void pci_device_parse(uint8_t classCode, uint8_t subClass, uint8_t progIF)
{
  switch (classCode)
  {
  case 0x1:
    DebugOutput("Mass Storage Controller");
    break;
  case 0x2:
    DebugOutput("Network Controller");
    break;
  case 0x3:
    DebugOutput("Display Controller");
    break;
  case 0x04:
    DebugOutput("Multimedia Controller");
    break;
  case 0x05:
    DebugOutput("Memory Controller");
    break;
  case 0x06:
    DebugOutput("Bridge Device");
    break;
  case 0x07:
    DebugOutput("Simple Communication Controller");
    break;
  case 0x08:
    DebugOutput("Base System Peripheral");
    break;
  case 0x09:
    DebugOutput("Input Device");
    break;
  case 0x0A:
    DebugOutput("Docking Station");
    break;
  case 0x0B:
    DebugOutput("Processor");
    break;
  case 0x0c:
    DebugOutput("Serial Bus Controller");
    break;
  case 0x0D:
    DebugOutput("Wireless Controller");
    break;
  case 0x0E:
    DebugOutput("Intelligent IO Controller");
    break;
  case 0x0F:
    DebugOutput("Satellite Communication Controller");
    break;
  case 0x10:
    DebugOutput("Encryption Controller");
    break;
  case 0x11:
    DebugOutput("Data Acquisition and Signal Processing Controller");
    break;
  default:
    break;
  }
}
