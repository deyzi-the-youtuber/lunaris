#include <common.h>
#include <lunaris/debug.h>
#include <lunaris/video.h>
#include <lunaris/mm.h>
#include <multiboot.h>
#include <lunaris/errno.h>
#include <stdint.h>
#include <system.h>

#define CMDLINE_MAX_ENTRIES 3

static const char * CmdlineEntries[CMDLINE_MAX_ENTRIES] = {"root=", "quiet", "debug"};

char * get_cmdline(struct multiboot_header_tag * tag, uintptr_t address)
{
  tag = (struct multiboot_header_tag *)(address + 8);
  while(tag->type != MULTIBOOT_TAG_TYPE_END)
  {
    switch (tag->type)
    {
      case MULTIBOOT_TAG_TYPE_CMDLINE:
        struct multiboot_tag_string * tag_str = (struct multiboot_tag_string *)tag;
        return (char *)(tag_str->string);
        break;
    
      default:
        break;
    }
    tag = (struct multiboot_header_tag *) ((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7));
  }
}

int parse_cmdline(const char * cmdline)
{
  if(!cmdline)
  {
    return -EINVAL;
  }
  DebugOutput("[CMDLINE] %s\n", cmdline);

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
          DebugOutput("[CMDLINE] Root: %s\n", valueStart);
          break;
        }
        case 1:
          // quiet
          break;
        case 2:
          // debug
          break;
        default:
          // unknown
          break;
      }
    }
  }
  return 0;
}
