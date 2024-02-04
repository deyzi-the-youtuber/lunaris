#include <kernel/sys/io.h>
#include <kernel/video/vga.h>
#include <kernel/pic.h>
#include <stdint.h>
#include <kernel/printk.h>

void InitializePIC()
{
  printk("picdev: init\n");
  uint8_t a1, a2;

  a1 = IoPortByteRead(PIC1_DATA);
  a2 = IoPortByteRead(PIC2_DATA);

  IoPortByteWrite(PIC1_COMMAND, ICW1);
  IoPortByteWrite(PIC2_COMMAND, ICW1);

  IoPortByteWrite(PIC1_DATA, 0x20);

  IoPortByteWrite(PIC2_DATA, 0x28);

  IoPortByteWrite(PIC1_DATA, 4);

  IoPortByteWrite(PIC2_DATA, 2);

  IoPortByteWrite(PIC1_DATA, ICW4_8086);
  IoPortByteWrite(PIC2_DATA, ICW4_8086);

  IoPortByteWrite(PIC1_DATA, a1);
  IoPortByteWrite(PIC2_DATA, a2);
}

void EndOfInterruptPIC(uint8_t irq)
{
  if (irq >= 0x8)
    IoPortByteWrite(PIC2, PIC_EOI);
  IoPortByteWrite(PIC1, PIC_EOI);
}