CC_FLAGS = -I include -m32 -nostdlib -nostartfiles -g -ffreestanding -std=gnu99 -fno-omit-frame-pointer
ASM_FLAGS = -f elf32
LD_FLAGS = -m elf_i386 -nostdlib

OBJOUT = lunaris

OBJ = arch/x86/boot/grub.o \
kernel/main.o \
drivers/video/vga.o \
lib/itoa.o \
arch/x86/eip.o \
arch/x86/cpu.o \
drivers/serial/serial.o \
kernel/hal/hal.o \
drivers/serial/debug.o \
lib/strlen.o \
arch/x86/gdt.o \
arch/x86/gdt_install.o \
arch/x86/idt.o \
arch/x86/idt_install.o \
arch/x86/exceptions.o \
arch/x86/irq.o \
arch/x86/isr.o \
arch/x86/pic.o \
kernel/timer/timer.o \
lib/memset.o \
mm/paging.o \
lib/ftoa.o \
arch/x86/fpu.o \
lib/memcpy.o \
mm/malloc.o \
drivers/pci/pci.o \
arch/x86/tss_install.o \
proc/tasking/task.o \
drivers/blk/ide/ide.o \
lib/strcmp.o \
lib/indexof.o \
lib/substrr.o \
lib/strcat.o \
lib/strcpy.o \
proc/syscalls/syscall.o \
kernel/cmdline.o \
kernel/trace.o \
kernel/usermode.o \
lib/strstr.o \
lib/memcmp.o \
lib/strcspn.o \
lib/strchr.o \
arch/x86/cpuid.o \
arch/x86/cmos.o \
drivers/keyboard/kbd.o \
lib/rand.o \
arch/x86/exception.o \
ipc/signal.o \
ipc/spinlock.o \
drivers/tty/tty.o \
drivers/mbr/mbr.o \
arch/x86/apic.o \
kernel/ksyms.o \
lib/strncmp.o \
arch/x86/acpi.o \
mm/page.o \
kernel/loader/elf.o \
fs/devfs/devfs.o \
kernel/printk.o \
kernel/panic.o \
fs/ext2/ext2.o \
drivers/video/gop.o \
drivers/sound/pcspk.o

%.o: %.c
	@gcc $(CC_FLAGS) -o $@ -lc -c $<
	@echo "CC    " $<

%.o: %.S
	@nasm $(ASM_FLAGS) -o $@ $<
	@echo "AS    " $<

$(OBJOUT): link.ld $(OBJ)
	@../tools/gen-ksyms.sh
	@gcc $(CC_FLAGS) -lc -c symlist.c -o symlist.o
	@ld $(LD_FLAGS) -T $< -o $@ $(OBJ) symlist.o
	@nm lunaris -X 32_64 >> kernel.map
	@echo "$@ is ready"
	@echo "You may need to recompile to get the kernel symbols working"

grub:
	@../tools/diskedit.sh disk.img -m
	@sudo cp $(OBJOUT) /mnt/boot/
	@sudo cp grub.cfg /mnt/boot/grub/
	@../tools/diskedit.sh disk.img -u
	@qemu-img convert -f raw -O vpc disk.img disk.vhd

qemu:
	@qemu-system-x86_64 -m 2G -drive format=raw,file=disk.img -serial stdio -cpu core2duo -vga std -audiodev pa,id=snd0 -machine pcspk-audiodev=snd0

debugger:
	@qemu-system-x86_64 -kernel $(OBJOUT) -m 1G -d int -drive format=raw,file=disk.img -serial stdio -cpu core2duo -vga std -audiodev pa,id=snd0 -machine pcspk-audiodev=snd0

vbox:
	@VBoxManage modifyvm "Lunaris" --acpi on --ioapic on --longmode on --graphicscontroller vboxvga
	@VirtualBoxVM --startvm "Lunaris"

image:
	@cp lunaris ./isodir/boot/ 
	@cp grub.cfg ./isodir/boot/grub/ 
	@grub-mkrescue ./isodir -o lunaris.iso

clean:
	@rm -f $(OBJ)
	@rm -f $(OBJOUT)
	@rm -f lunaris.map
	@rm -f disk.vhd
	@rm -f lunaris.iso
	@rm -f symlist.o
