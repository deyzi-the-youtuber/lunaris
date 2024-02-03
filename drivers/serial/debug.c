#include <kernel/sys/serial.h>
#include <kernel/video/vga.h>
#include <kernel/task.h>
#include <common.h>
#include <stdarg.h>

static void DebugOutputChar(const char c)
{
  SerialWrite(c);
}

void DebugOutput(const char * string, ...)
{
  va_list ARG;
  va_start(ARG, string);
  while (*string)
  {
    if (*string == '%')
    {
      *string++;
      switch (*string)
      {
        case 's':
          const char * s2 = va_arg(ARG, const char *);
          while (*s2)
          {
            DebugOutputChar(*s2);
            *s2++;
          }
          break;
        case 'd':
          int num = va_arg(ARG, int);
          char nums[12];
          itoa(num, nums, 10);
          for (int i = 0; nums[i] != '\0'; i++)
          {
            DebugOutputChar(nums[i]);
          }
          break;
        case 'x':
          int hex = va_arg(ARG, int);
          char hexs[12];
          itoa(hex, hexs, 16);
          for (int i = 0; hexs[i] != '\0'; i++)
          {
            DebugOutputChar(hexs[i]);
          }
          break;
        /*case 'c':
          char c[2];
          c[0] = va_arg(ARG, int);
          c[1] = '\0';
          for (int i = 0; c[i] != '\0'; i++)
          {
            DebugOutputChar(c[i]);
          }
          break;*/
      }
    }
    else
    {
      switch (*string)
      {
        case '\n':
          DebugOutputChar('\r');
        default:
          DebugOutputChar(*string);
          break;

      }
    }
    string++;
  }
}

void InitializeDebugIO(void)
{
  DebugOutput("[INFO] You can now type in the debug console :)\n");
  char c[2];
  while (1)
  {
    c[0] = (char)SerialRead();
    c[1] = '\0';
    if (c[0] != 13)
      DebugOutput("%s", c);
    else
      DebugOutput("\n\r");
  }
}