#include <kernel/task.h>
#include <stdint.h>

void notify(uint32_t sig)
{
  Process * c = getCurrentProcess();
	c->notify(sig);
}