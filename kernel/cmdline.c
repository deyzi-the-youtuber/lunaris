#include <common.h>
#include <kernel/debug.h>
#include <kernel/video/vga.h>
#include <kernel/mm/malloc.h>
#include <system.h>

#define CMDLINE_MAX_ENTRIES 3

static const char * CmdlineEntries[CMDLINE_MAX_ENTRIES] = {"root=", "quiet", "debug"};

int parse_cmdline(const char * cmdline, SysInfo * sys)
{
  DebugOutput("[CMDLINE] %s\n", cmdline);
  SysInfo dummy;

  if (cmdline == NULL || sys == NULL)
    return 1;

  // The length of possible entries in the kernel command line
  int len = sizeof(CmdlineEntries) / sizeof(CmdlineEntries[0]);

  for (int i = 0; i < len; i++)
  {
    const char * p = strstr(cmdline, CmdlineEntries[i]);
    if (p != NULL)
    {
      DebugOutput("[CMDLINE] Found cmdline entry: %s\n", CmdlineEntries[i]);
      switch (i)
      {
        case 0:
        {
          int prefixLen = strlen(CmdlineEntries[i]);
          const char* valueStart = p + prefixLen;
          const char* valueEnd = strchr((char *)valueStart, ' ');
          if (valueEnd == NULL)
            valueEnd = valueStart + strlen((char *)valueStart);

          int paramLen = valueEnd - valueStart;
          if (paramLen >= 256)
            paramLen = 256 - 1;
          memcpy(sys->RootPath, (char *)valueStart, paramLen);
          sys->RootPath[paramLen] = '\0';
          DebugOutput("[CMDLINE] Root drive: %s\n", sys->RootPath);
          break;
        }
        case 1:
          sys->quiet = 1;
          break;
        case 2:
          sys->debugInfo = 1;
          break;
        default:
          break;
      }
    }
  }
  return 0;
}
