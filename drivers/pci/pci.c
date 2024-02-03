#include <stdint.h>
#include <kernel/sys/io.h>
#include <kernel/pci.h>
#include <kernel/mm/malloc.h>
#include <kernel/video/vga.h>
#include <kernel/debug.h>

uint16_t ConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
  uint32_t address;
  uint32_t lbus = (uint32_t)bus;
  uint32_t lslot = (uint32_t)slot;
  uint32_t lfunc = (uint32_t)func;
  uint16_t tmp = 0;

  // Create configuration address as per Figure 1
  address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));

  // Write out the address
  IoPortDwordWrite(PCI_CONFIG_ADDRESS, address);
  // Read in the data
  // (offset & 2) * 8) = 0 will choose the first word of the 32-bit register
  tmp = (uint16_t)((IoPortDwordRead(PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xFFFF);
  return tmp;
}

uint16_t getClassId(uint16_t bus, uint16_t device, uint16_t function)
{
  uint32_t r0 = ConfigReadWord(bus, device, function, PCI_SUBCLASS);
  return (r0 & ~0x00FF) >> 8;
}

uint16_t getSubClassId(uint16_t bus, uint16_t device, uint16_t function)
{
  uint32_t r0 = ConfigReadWord(bus, device, function, PCI_SUBCLASS);
  return (r0 & ~0xFF00);
}

int FilterDevice(uint8_t bus, uint8_t slot, uint8_t function)
{
  uint16_t vendor_id = ConfigReadWord(bus, slot, function, 0x00);
  return !(vendor_id == 0xffff || !vendor_id);
}

void GetDeviceDetails(PCIdevice *device, uint8_t details, uint8_t bus, uint8_t slot, uint8_t function)
{
  device->bus = bus;
  device->slot = slot;
  device->function = function;

  device->class_id = getClassId(bus, slot, function);
  device->subclass_id = getSubClassId(bus, slot, function);

  if (!details)
    return;

  device->vendor_id = ConfigReadWord(bus, slot, function, 0x00);
  device->device_id = ConfigReadWord(bus, slot, function, PCI_DEVICE_ID);

  device->interface_id = ConfigReadWord(bus, slot, function, PCI_PROG_IF);

  device->system_vendor_id = ConfigReadWord(bus, slot, function, PCI_SYSTEM_VENDOR_ID);
  device->system_id = ConfigReadWord(bus, slot, function, PCI_SYSTEM_ID);

  device->revision = ConfigReadWord(bus, slot, function, PCI_REVISION_ID);
  device->interrupt = ConfigReadWord(bus, slot, function, PCI_INTERRUPT_LINE);
}

void DumpPCI()
{
  for (int bus = 0; bus < 256; bus++)
  {
    for (int device = 0; device < 32; device++)
    {
      if (FilterDevice(bus, device, 0) != 0xFFFF)
      {
        uint16_t classAndSub = ConfigReadWord(bus, device, 0, 10);
        if ((classAndSub >> 8) < 0x11)
        {
          DebugOutput("[PCI] Found ");
          ParseDevicePCI((uint8_t)(classAndSub >> 8), (uint8_t)classAndSub, (uint8_t)(ConfigReadWord(bus, device, 0, 8) >> 8));
          DebugOutput(" at 0x%x, 0x%x", bus, device);
          if (((uint8_t)(ConfigReadWord(bus, device, 0, 14) & 0xFF)) & 0x80 != 0)
          {
            DebugOutput(" With Multiple functions\n");
          }
          else
          {
            DebugOutput("\n");
          }
        }
      }
    }
  }
}

void ParseDevicePCI(uint8_t classCode, uint8_t subClass, uint8_t progIF)
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
  DebugOutput(" (Sub 0x%x, 0x%x)", subClass, progIF);
}