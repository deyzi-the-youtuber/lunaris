#include <stdint.h>
#include <kernel/task.h>
#include <kernel/sys/tty.h>
#include <kernel/debug.h>
#include <kernel/errno.h>
#include <kernel/signal.h>
#include <kernel/fs/devfs.h>
#include <kernel/video/vga.h>
#include <common.h>
#include <kernel/printk.h>

tty_dev_t tty[NTTY];

/*
  Append a character into a tty buf.
*/
int putq(queue_buffer_t *qb, char ch)
{
  // this buffer has been full now.
  if (qb->q_count == QBUFSIZ)
  {
    return -1;
  }
  qb->q_char[qb->q_end] = ch;
  qb->q_end = (qb->q_end + 1) % QBUFSIZ;
  qb->q_count++;
  return 0;
}

/*
  Get the first character from a tty buf.
*/
char getq(queue_buffer_t *qb)
{
  char ch;
  // if this buffer is empty
  if (qb->q_count == 0)
  {
    return -1;
  }
  ch = qb->q_char[qb->q_start];
  qb->q_start = (qb->q_start + 1) % QBUFSIZ;
  qb->q_count--;
  return ch;
}

/* Erase the last character of a tty buf */
char eraseq(queue_buffer_t *qb)
{
  // if this buffer is empty
  if (qb->q_count == 0)
  {
    return -1;
  }
  qb->q_end = (qb->q_end - 1 + QBUFSIZ) % QBUFSIZ;
  qb->q_count--;
  return 0;
}

/* ---------------------------------------------- */

/* 
  take characterers from raw list, make some parse & erase and place
  it into canon list.
*/
int tty_canon(tty_dev_t *tp)
{
  char ch;
  int i;

  // if raw mode
  if (tp->t_flag & TTY_RAW)
  {
    while ((ch = getq(&tp->raw_q)) >= 0)
    {
      putq(&tp->can_q, ch);
    }
    return 0;
  }
  // canon mode
  while ((ch = getq(&tp->raw_q)) >= 0)
  {
    switch (ch)
    {
    case '\t':
      // expand tab into spaces
      for (i = 0; i < 4 - (tp->col % 4); i++)
        putq(&tp->can_q, ' ');
      return 0;
      break;
    case CERASE:
      eraseq(&tp->can_q);
      x--;
      VideoOutputCharVGA('\0');
      x--;
      break;
    default:
      putq(&tp->can_q, ch);
      break;
    }
  }
  return 0;
}

/* output characters with buffering */
int tty_output(tty_dev_t *tp, char ch)
{
  int i;
  //
  switch (ch)
  {
  case '\t':
    // expand tab into spaces
    for (i = 0; i < 4 - (tp->col % 4); i++)
      putq(&tp->out_q, ' ');
    break;
  case 10:
    putq(&tp->out_q, '\n');
    break;
  default:
    putq(&tp->out_q, ch);
    break;
  }
  return 0;
}

/*
  Place a character on raw TTY input queue, if a carriage character
  arrives, wake up the awaitors.
*/
int tty_input(tty_dev_t *tp, char ch)
{
  uint32_t cnt;

  putq(&tp->raw_q, ch);
  cnt = tp->can_q.q_count;
  tty_canon(tp);

  if (tp->t_flag & TTY_ECHO)
  {
    if (ch == CERASE)
    {
      if (cnt <= 0)
      {
        return 0;
      }
    }
    tty_output(tp, ch);
    tty_start(tp);
  }

  if ((tp->t_flag & TTY_RAW) == 0)
  {
    if (CINTR == ch)
    {
      notify(SIGINT);
    }
  }
  if (ch == CEOF || ch == '\n')
  {
    eraseq(&tp->can_q);
    //wakeup((uint32_t)tp);
    return 0;
  }
  return 0;
}

/* ---------------------------------------- */

/* output actually starts here. */
int tty_start(tty_dev_t *tp)
{
  void (*putc)(char);
  char ch;

  putc = tp->putc;
  while ((ch = getq(&tp->out_q)) >= 0)
  {
    putc(ch);
  }
  return 0;
}

/* ---------------------------------------------- */

int tty_open(uint16_t dev)
{
  tty_dev_t * tp;

  if (MINOR(dev) >= NTTY)
  {
    return -ENODEV;
  }
  tp = &tty[MINOR(dev)];
  tp->t_flag = TTY_ECHO;
  tp->putc = &VideoOutputCharVGA;
  tp->raw_q.q_count = 0;
  tp->can_q.q_count = 0;
  tp->out_q.q_count = 0;
  return 0;
}

/* called in iput() */
int tty_close(uint16_t dev)
{
  return 0;
}

/* ---------------------------------------------- */

/*
  If the list is not full, wait until
*/
int tty_read(uint16_t dev, char *buf, uint32_t cnt)
{
  DebugOutput("[TTY] Reading from tty%d...\n", dev);
  tty_dev_t *tp;
  char ch;
  int i;

  if (MINOR(dev) >= NTTY)
  {
    return -ENODEV;
  }
  tp = &tty[MINOR(dev)];
  // if no data on canonical list
  if (tp->can_q.q_count < cnt)
  {
    sleep(getCurrentProcess());
  }
  //
  for (i = 0; i < cnt; i++)
  {
    if ((ch = getq(&tp->can_q)) < 0)
      break;
    buf[i] = ch;
  }
  return i + 1;
}

int tty_write(uint16_t dev, char *buf, uint32_t cnt)
{
  tty_dev_t *tp;
  int i;

  if (MINOR(dev) >= NTTY)
  {
    return -ENODEV;
  }
  tp = &tty[MINOR(dev)];
  for (i = 0; i < cnt; i++)
  {
    tty_output(tp, buf[i]);
  }
  tty_start(tp);
  VideoUpdateCursorVGA(y * VGA_ROWS + x);
  return 0;
}

void dev_tty_read(uint8_t * buffer, uint32_t count)
{
  tty_read(0, buffer, count);
}

void dev_tty_write(uint8_t * buffer, uint32_t count)
{
  tty_write(0, buffer, count);
}

void tty_init()
{
  printk("tty: init\n");
  tty[0].t_flag = TTY_ECHO;
  tty[0].putc = &VideoOutputCharVGA;
  tty[0].raw_q.q_count = 0;
  tty[0].can_q.q_count = 0;
  tty[0].out_q.q_count = 0;
  devfs_create_device(VFS_CHARACTER_DEVICE, true, true, "tty0", dev_tty_read, dev_tty_write, "/");
  DebugOutput("[TTY] Device tty0 created\n");
}