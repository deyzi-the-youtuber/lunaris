#include <common.h>
#include <lunaris/video.h>
#include <sys/types.h>
#include <stdarg.h>

size_t vsprintf(const char fmt[], va_list arg)
{
  size_t ret = 0;
  int field_width;
  for(int i = 0; fmt[i]; i++)
  {
    if(fmt[i] == '%')
    {
      i++;
      switch (fmt[i]) 
      {
        case 's':
        {
          char * str = va_arg(arg, char *);
          ret += strlen(str);
          video_puts(str);
          break;
        }
        case 'd':
        {
          int num = va_arg(arg, int);
          char nums[255];
          itoa(num, nums, 10);
          ret += strlen(nums);
          video_puts(nums);
          break;
        }
        case 'x':
        {
          uint32_t hex = va_arg(arg, int);
          char hexs[255];
          itoa(hex, hexs, 16);
          ret += strlen(hexs);
          video_puts(hexs);
          break;
        }
        case 'c':
        {
          int c = va_arg(arg, int);
          ret++;
          video_putc(c);
          break;
        }
        default:
        {
          break;
        }
      }
    }
    else 
    {
      video_putc(fmt[i]);
      ret++;
    }
  }
  return ret;
}

