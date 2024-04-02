#include <sys/io.h>
#include <lunaris/serial.h>

int SerialInitialize()
{
  outb(COM1 + 1, 0x00); // Disable all interrupts
  outb(COM1 + 3, 0x80); // Enable DLAB (set baud rate divisor)
  outb(COM1 + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
  outb(COM1 + 1, 0x00); //                  (hi byte)
  outb(COM1 + 3, 0x03); // 8 bits, no parity, one stop bit
  outb(COM1 + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
  outb(COM1 + 4, 0x0B); // IRQs enabled, RTS/DSR set
  outb(COM1 + 4, 0x1E); // Set in loopback mode, test the serial chip
  outb(COM1 + 0, 0xAE); // Test serial chip (send byte 0xAE and check if serial returns same byte)

  // Check if serial is faulty (i.e: not same byte as sent)
  if (inb(COM1 + 0) != 0xAE)
  {
    return 0;
  }

  // If serial is not faulty set it in normal operation mode
  outb(COM1 + 4, 0x0F);
  return 1;
}

static int SerialTransitEmpty()
{
  return inb(COM1 + 5) & 0x20;
}

void SerialWrite(char a)
{
  while (SerialTransitEmpty() == 0);
  outb(COM1, a);
}

static int SerialReceived()
{
  return inb(COM1 + 5) & 1;
}

static char _serial_read()
{
  while (SerialReceived() == 0);
  return inb(COM1);
}

void SerialRead(uint8_t * buffer, uint32_t count)
{
  int i = 0;
  do
  {
    if (SerialReceived() != 0)
    {
      buffer[i] = (char)_serial_read();
      buffer[i + 1] = 0;
      i++;
    }
  }while(i < count);
}
