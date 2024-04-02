CC_FLAGS = -I include -m32 -nostdlib -nostartfiles -g -ffreestanding -fno-omit-frame-pointer -fstack-protector
ASM_FLAGS = -f elf32 -g
LD_FLAGS = -m elf_i386 -nostdlib -znoexecstack

OBJOUT = lunaris

# Cross compiler prefix
# This isn't needed for NASM
export CROSS_COMPILE=
export ARCH=i386

C_SOURCE := $(shell find . -type f -name "*.c")
ASM_SOURCE := $(shell find . -type f -name "*.S")

C_OBJECTS := $(patsubst %.c, %.o, $(C_SOURCE))
ASM_OBJECTS := $(patsubst %.S, %.o, $(ASM_SOURCE))

all: gen_kconfig $(OBJOUT)

%.o: %.c
	@$(CROSS_COMPILE)gcc $(CC_FLAGS) -o $@ -lc -c $<
	@echo "CC    " $<

%.o: %.S
	@nasm $(ASM_FLAGS) -o $@ $<
	@echo "AS    " $<

$(OBJOUT): $(ASM_OBJECTS) $(C_OBJECTS)
	@../tools/gen-ksyms.sh
	@$(CROSS_COMPILE)ld $(LD_FLAGS) -T link.ld -o $(OBJOUT).o -r $^
	@$(CROSS_COMPILE)gcc $(CC_FLAGS) -o symlist.o -lc -c symlist.c
	@$(CROSS_COMPILE)ld $(LD_FLAGS) -T link.ld -o $(OBJOUT) $(OBJOUT).o symlist.o
# get symbol map
	@$(CROSS_COMPILE)nm lunaris >> kernel.map
	@echo "$@ is ready"

grub:
	@../tools/diskedit.sh disk.img -m
	@sudo cp $(OBJOUT) /mnt/boot/
	@rm -f /mnt/boot/lunaris.old
	@../tools/diskedit.sh disk.img -u
	@qemu-img convert -f raw -O vpc disk.img disk.vhd

qemu:
	@qemu-system-$(ARCH) -m 2G -drive format=raw,file=disk.img -serial stdio -vga cirrus -audiodev pa,id=snd0 -machine pcspk-audiodev=snd0 -smbios type=2
 
debugger:
	@qemu-system-$(ARCH) -m 2G -d int -drive format=raw,file=disk.img -cpu qemu32 -vga cirrus -audiodev pa,id=snd0 -machine pcspk-audiodev=snd0 -smbios type=2 -no-reboot

iso:
	@cp lunaris ./isodir/boot/ 
	@cp grub.cfg ./isodir/boot/grub/ 
	@grub-mkrescue ./isodir -o lunaris.iso

gen_kconfig:
	@python ../tools/gen-config.py .config include/lunaris/config.h -v

clean:
	@rm -f $(C_OBJECTS) $(ASM_OBJECTS) $(OBJOUT) symlist.c
