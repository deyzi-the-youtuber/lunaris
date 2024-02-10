#include <multiboot.h>
#include <stdint.h>

#ifndef __KERN_MODULE_H
#define __KERN_MODULE_H

int modules_load(struct multiboot_tag_module * tag, uintptr_t addr);
void initcall(int (*func)(void));
void exitcall(void (*func)(void));

#define module_init(x) initcall(x)
#define module_exit(x) exitcall(x)

#endif