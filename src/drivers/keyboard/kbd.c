#include <sys/io.h>
#include <lunaris/kbd.h>
#include <lunaris/isr.h>
#include <sys/tty.h>
#include <fs/devfs.h>
#include <fs/vfs.h>
#include <lunaris/mm.h>
#include <lunaris/debug.h>
#include <lunaris/task.h>
#include <lunaris/video.h>
#include <common.h>
#include <lunaris/printk.h>

static uint32_t mode = 0;

/* translate into flags which indicated the satus of shift, ctrl & alt. */
char shift(char sc)
{
  char ch = sc & 0x7f;

  if (mode & E0ESC)
  {
    switch (ch)
    {
    case 0x1D:
      return CTRL;
    case 0x38:
      return ALT;
    }
  }
  else
  {
    switch (ch)
    {
    case 0x2A:
    case 0x36:
      return SHIFT;
    case 0x1D:
      return CTRL;
    case 0x38:
      return ALT;
    }
  }
  return 0;
}

/*
 * Each keyboard interrupt came along with a 8bit scan code (via inb(KEYBOARD_ENCODER)), if bit 8 is
 * set, it's releasing a key, else pressing.
 * */


void do_keybd_intr(REGISTERS * reg)
{
  uint8_t sc, ch, m;
  uint8_t * map = keybd_map;

  // got no data
  if ((inb(KEYBOARD_ONBOARD_CONTROLLER) & KB_STAT_DIB) == 0)
  {
    return;
  }
  sc = inb(KEYBOARD_ENCODER);

  // ignore capslock yet.
  if ((sc & 0x7f) == 0x3A)
    return;

  // check E0ESC
  if (sc == 0xE0)
    mode |= E0ESC;

  // check shift, ctrl and alt
  if ((m = shift(sc)))
  {
    if (sc & 0x80)
      mode &= ~m;
    else
      mode |= m;
    return;
  }
  map = (mode & SHIFT) ? shift_map : keybd_map;
  ch = map[sc & 0x7f];

  if (mode & CTRL)
  {
    switch (ch)
    {
    case 'c':
      ch = CINTR;
    case 'd':
      ch = CEOF;
    case 'x':
      ch = CKILL;
    case 'q':
      ch = CSTART;
    case 's':
      ch = CSTOP;
    case 'z':
      ch = CSUSP;
    case '\\':
      ch = CQUIT;
    }
  }

  // on pressed
  if ((sc & 0x80) == 0 && ch != '\0')
  {
    tty_input(&tty[0], ch);
    video_remap(video_getx(), video_gety());
  }
  // on released
  else
  {
    mode &= ~E0ESC;
  }

  return;
}

void dev_keybd_read()
{
  for(;;);
}

void keyboard_init(void)
{
  printk("kbd: init\n");
  /* install keyboard irq */
  printk("kbd: installing irq handler...\n");
  RegisterInterrupt(0x21, do_keybd_intr);
  
  /* register devfs "input" directory node */
  printk("kbd: registering device...\n");
  devfs_create_device(VFS_DIRECTORY, true, false, "input", NULL, NULL, "/");
  devfs_create_device(VFS_CHARACTER_DEVICE, true, false, "kbddev", dev_keybd_read, NULL, "input");
}
