#include <stdatomic.h>
#include <kernel/spinlock.h>

spinlock spin_lock_new()
{
  spinlock spinlock_new = ATOMIC_FLAG_INIT;
  return spinlock_new;
}

void spin_lock_acquire(spinlock * lock)
{
  while( atomic_flag_test_and_set_explicit(lock, memory_order_acquire))
  {
    __builtin_ia32_pause();
  }
}
 
void spin_lock_release(spinlock * lock)
{
  atomic_flag_clear_explicit(lock, memory_order_release);
}