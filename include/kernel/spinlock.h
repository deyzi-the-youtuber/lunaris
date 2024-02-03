#include <stdatomic.h>

#ifndef __SPINLOCK_H
#define __SPINLOCK_H

typedef atomic_flag spinlock;

void spin_lock_acquire(spinlock * lock);
void spin_lock_release(spinlock * lock);

#endif