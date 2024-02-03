#include <stdint.h>

#ifndef __SCHED_H
#define __SCHED_H

typedef struct ThreadList
{
  struct thread * thread;
  struct ThreadList * next;
}ThreadList;

void init_scheduler(struct thread * initial_thread);
void make_thread_ready(struct thread *t);
void make_thread_not_ready(struct thread *t);
void schedule();
extern void switch_thread(struct ThreadList *t);

#endif