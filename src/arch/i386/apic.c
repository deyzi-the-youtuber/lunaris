#include <stdint.h>
#include <cpuid.h>
#include <common.h>
#include <lunaris/apic.h>
#include <lunaris/errno.h>
#include <lunaris/printk.h>

bool check_apic(void)
{
  uint32_t eax, unused, edx;
  __get_cpuid(1, &eax, &unused, &unused, &edx);
  return edx & (1 << 9);
}

int initialize_apic()
{
  if (check_apic() != true)
  {
    printk("apic: apic not supported\n");
    return -1;
  }
  printk("apic: apic is supported\n");
  return 0;
}
