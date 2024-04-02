#include <lunaris/trace.h>
#include <lunaris/debug.h>
#include <stdint.h>
#include <lunaris/printk.h>

/* Assume, as is often the case, that EBP is the first thing pushed. If not, we are in trouble. */

void StackTrace(uint32_t MaxFrames)
{
  DebugOutput("[KERNEL] Dumping stack...\n");
  printk("call trace:\n");
  struct stackframe *stk;
  asm ("movl %%ebp,%0" : "=r"(stk) ::);
  for(uint32_t frame = 0; stk && frame < MaxFrames; ++frame)
  {
    // Unwind to previous stack frame
    uintptr_t offset;
    char * name = trace_address(&offset, stk->eip);
    printk("[<0x%x>] <%s+0x%x>\n", stk->eip, name, stk->ebp);
    stk = stk->ebp;
  }
}
