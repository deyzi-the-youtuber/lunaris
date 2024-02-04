/*
  Lunaris kernel "vga.c"
  Jan 28, 2024

  DESCRIPTION:
    [+] Generic VGA BIOS graphics driver.
    [+] Usable on x86_64 systems (without UEFI).
    [+] Incompatible with UEFI.
*/

#include <kernel/video/vga.h>
#include <system.h>
#include <kernel/sys/io.h>
#include <common.h>
#include <stdarg.h>
#include <stdint.h>
#include <kernel/config.h>

extern SysInfo kinfo;
int x, y = 0;
int color = VGA_DEFAULT;

void VideoOutputCharVGA(const char c)
{
  switch (c)
  {
    case '\n':
      x = 0;
      y++;
      break;
  
    default:
      if (c != '\b')
      {
        uint16_t position = y * VGA_ROWS + x;
        uint16_t *where = (uint16_t *)(VGA_TEXT_BUFFER + position * 2);
        *where = (uint16_t)c | (color << 8);
        x++;
      }
  }
  scroll();
}

void scroll()
{
  // Check if the cursor is at the last row of the VGA buffer
  if (y >= VGA_COL)
  {
      // Move all rows up by one (excluding the first row)
    for (int i = 1; i < VGA_COL; i++)
    {
      memcpy((void *)VGA_TEXT_BUFFER + (i - 1) * VGA_ROWS * 2, (void *)VGA_TEXT_BUFFER + i * VGA_ROWS * 2, VGA_ROWS * 2);
    }

    // Clear the last row
    char * lastRow = (void *)VGA_TEXT_BUFFER + 24 * VGA_ROWS * 2;
    memset(lastRow, 0, VGA_ROWS * 2);

    // Move the cursor up by one row
    y--;
  }
}

void VideoOutputVGA(char * string)
{
  for(int i = 0; string[i] != 0; i++)
  {
    VideoOutputCharVGA(string[i]);
  }
  VideoUpdateCursorVGA(y * VGA_ROWS + x);
}

void VideoClearVGA(void)
{
  char *TEXT_BUFFER = (char *)0xB8000;
  for (int i = 0; i < 80 * 25; i++)
  {
    *TEXT_BUFFER++ = '\0'; // Null character
    *TEXT_BUFFER++ = 0x7;  // Color for the cursor. Without this, it'd be black
  }
  VideoUpdateCursorVGA(0);
}

void VideoSetColor(int c)
{
  color = c;
}

void VideoUpdateBackgroundVGA(int col)
{
  char *TEXT_BUFFER = (char *)0xB8000;
  for (int i = 0; i < 80 * 25 * 4; i++)
  {
    if (i % 2 == 1)
    {
      *TEXT_BUFFER++ = col | (col << 4);
    }
  }
}

void VideoUpdateCursorVGA(uint16_t position)
{
  IoPortByteWrite(0x3D4, 14);
  IoPortByteWrite(0x3D5, position >> 8);
  IoPortByteWrite(0x3D4, 15);
  IoPortByteWrite(0x3D5, position);
}