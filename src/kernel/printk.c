#include <kernel/video/vga.h>
#include <kernel/printk.h>
#include <kernel/config.h>
#include <common.h>
#include <stdarg.h>

extern volatile uint8_t jiffies;

static void printk_print_time()
{
  char buff[32];
  float f = (float)jiffies / CONFIG_TIMER_FREQ;
  ftoa(buff, f);
  VideoOutputCharVGA('[');
  VideoOutputCharVGA(' ');
  VideoOutputVGA(buff);
  VideoOutputCharVGA(' ');
  VideoOutputCharVGA(']');
  VideoOutputCharVGA(' ');
}

void vprintk(char fmt[], va_list ap)
{
  printk_print_time();
  while (*fmt)
  {
    if (*fmt == '%')
    {
      *fmt++;
      switch (*fmt)
      {
      case 's':
        char * str = va_arg(ap, char *);
        VideoOutputVGA(str);
        break;

      case 'd':
        int num = va_arg(ap, int);
        char nums[12];
        itoa(num, nums, 10);
        VideoOutputVGA(nums);
        break;

      case 'x':
        int hex = va_arg(ap, int);
        char hexs[12];
        itoa(hex, hexs, 16);
        VideoOutputVGA(hexs);
        break;

      case 'p':
        void * ptr = va_arg(ap, void *);
        char ptrs[12];
        itoa((int)ptr, ptrs, 16);
        VideoOutputVGA(ptrs);
        break;

      case 'f':
        char buf[32];
        float f = va_arg(ap, double);
        ftoa(buf, f);
        VideoOutputVGA(buf);
        break;

      default:
        break;
      }
    }
    else
    {
      switch (*fmt)
      {
        case '\n':
          x = 0;
          y++;
          break;
        case '\t':
          VideoOutputVGA("  ");
          break;
        default:
          VideoOutputCharVGA(*fmt);
          break;
      }
    }
    fmt++;
  }
  VideoUpdateCursorVGA(y * VGA_ROWS + x);
}

void printk(char fmt[], ...)
{
  /*
    TODO:
    Store kernel messages into a buffer
  */
  va_list ap;
  va_start(ap, fmt);
  vprintk(fmt, ap);
  va_end(ap);
}