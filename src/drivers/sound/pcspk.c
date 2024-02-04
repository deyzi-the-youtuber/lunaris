#include <kernel/sys/io.h>
#include <kernel/sound/pcspk.h>
#include <stdint.h>

void pc_speaker_beep(uint16_t freq)
{
  uint32_t div;
  uint8_t tmp;

  div = 1193180 / freq;
  IoPortByteWrite(0x43, 0xb6);
  IoPortByteWrite(0x42, (uint8_t) (div));
  IoPortByteWrite(0x42, (uint8_t) (div >> 8));

  tmp = IoPortByteRead(0x61);
  if (tmp != (tmp | 3)) {
    IoPortByteWrite(0x61, tmp | 3);
  }
}

void pc_speaker_stop()
{
  uint8_t tmp = IoPortByteRead(0x61) & 0xFC;
  IoPortByteWrite(0x61, tmp);
}