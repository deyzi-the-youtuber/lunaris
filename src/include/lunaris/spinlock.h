#include <stdatomic.h>

#ifndef __SPINLOCK_H
#define __SPINLOCK_H

#define DEFINE_SPINLOCK(name) static spinlock name = ATOMIC_FLAG_INIT;

typedef atomic_flag spinlock;

void spinlock_new(void);
void spinlock_acquire(spinlock * lock);
void spinlock_release(spinlock * lock);

#endif
