#include <stdint.h>

#ifndef __TTY_H
#define __TTY_H

#define NTTY 255
#define QBUFSIZ  1024 /* size of a line buffer, and the limit of each line input. */

typedef struct
{
  int q_start;
  int q_end;
  int q_count;
  char q_char[QBUFSIZ];
}queue_buffer_t;

typedef struct
{
  int t_flag;
  int dev; /* device number */
  int col; /* position x of the cursor */
  int row; /* position y of the cursor */
  void (*putc)(char);
  queue_buffer_t raw_q;
  queue_buffer_t can_q;
  queue_buffer_t out_q;
}tty_dev_t;

#define TTY_CANON   0x0 /* if not raw, it's a canon */
#define TTY_RAW     0x1
#define TTY_ECHO    0x2
#define TTY_WANTED  0x10

/* control characters */
#define CINTR    28 /* ctrl-c */
#define CQUIT   034 /* ctrl-\ */
#define CERASE  '\b' /* DEL, BS */
#define CKILL   025 /* ctrl-x */
#define CEOF    004 /* ctrl-d */
#define CEOL    00
#define CSTART  021 /* ctrl-q */
#define CSTOP   023 /* ctrl-s */
#define CSUSP   032 /* ctrl-z */

extern tty_dev_t tty[NTTY];

void tty_init();
int tty_start(tty_dev_t *tp);
int tty_input(tty_dev_t *tp, char ch);

#endif