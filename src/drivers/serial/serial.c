#include <kernel/sys/io.h>
#include <kernel/video/vga.h>
#include <kernel/sys/serial.h>

int SerialInitialize()
{
  IoPortByteWrite(COM1 + 1, 0x00); // Disable all interrupts
  IoPortByteWrite(COM1 + 3, 0x80); // Enable DLAB (set baud rate divisor)
  IoPortByteWrite(COM1 + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
  IoPortByteWrite(COM1 + 1, 0x00); //                  (hi byte)
  IoPortByteWrite(COM1 + 3, 0x03); // 8 bits, no parity, one stop bit
  IoPortByteWrite(COM1 + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
  IoPortByteWrite(COM1 + 4, 0x0B); // IRQs enabled, RTS/DSR set
  IoPortByteWrite(COM1 + 4, 0x1E); // Set in loopback mode, test the serial chip
  IoPortByteWrite(COM1 + 0, 0xAE); // Test serial chip (send byte 0xAE and check if serial returns same byte)

  // Check if serial is faulty (i.e: not same byte as sent)
  if (IoPortByteRead(COM1 + 0) != 0xAE)
  {
    return 0;
  }

  // If serial is not faulty set it in normal operation mode
  IoPortByteWrite(COM1 + 4, 0x0F);
  return 1;
}

int SerialTransitEmpty()
{
  return IoPortByteRead(COM1 + 5) & 0x20;
}

void SerialWrite(char a)
{
  while (SerialTransitEmpty() == 0);
  IoPortByteWrite(COM1, a);
}

int SerialReceived()
{
  return IoPortByteRead(COM1 + 5) & 1;
}

char SerialRead()
{
  while (SerialReceived() == 0);
  return IoPortByteRead(COM1);
}

void _serial_read(void * buffer, uint32_t count)
{
  uint32_t i;
  char buf[2];
  do
  {
    if (SerialReceived() != 0)
    {
      buf[0] = (char)SerialRead();
      buf[1] = '\0';
      i++;
    }
  }while(i < count);
  return (void)buf;
}