#include <stdint.h>
#include <common.h>
#include <multiboot.h>

#ifndef __SYSTEM_H
#define __SYSTEM_H

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

extern void usermode_start_init();
extern void move_to_usermode();
/* Kernel start */
extern uint32_t kernel_base;
/* Kernel end */
extern uint32_t kernel_end;
extern uint32_t bss_start;
extern uint32_t bss_end;
extern uint32_t * ksyms_start;
extern uint32_t * ksyms_end;
/* Kernel stage 2 */
void KernelMainStage2();

int parse_cmdline(const char * cmdline);
char * get_cmdline(struct multiboot_header_tag * tag, uintptr_t address);
void install_exception_handlers(void);

#endif
