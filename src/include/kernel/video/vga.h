#include <stdint.h>

#ifndef __VGA_H
#define __VGA_H

#define VGA_TEXT_BUFFER 0xB8000

#define VGA_ROWS 80
#define VGA_COL 25

enum
{
  VGA_BLACK = 0x0,
  VGA_BLUE = 0x1,
  VGA_GREEN = 0x2,
  VGA_CYAN = 0x3,
  VGA_RED = 0x4,
  VGA_PURPLE = 0x5,
  VGA_ORANGE = 0x6,
  VGA_DEFAULT = 0x7,
  VGA_GRAY = 0x8,
  VGA_LAVENDER = 0x9,
  VGA_LGREEN = 0xA,
  VGA_LCYAN = 0xB,
  VGA_LRED = 0xC,
  VGA_LPURPLE = 0xD,
  VGA_YELLOW = 0xE,
  VGA_WHITE = 0xF
};

extern int x, y;

void VideoOutputVGA(char * string);
void VideoOutputCharVGA(const char c);
void VideoClearVGA(void);
void VideoUpdateBackgroundVGA(int col);
void VideoUpdateCursorVGA(uint16_t position);
void VideoSetColor(int c);
void scroll();

#endif