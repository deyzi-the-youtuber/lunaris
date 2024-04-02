all:
	@gcc -m32 -c crt0.c -o crt0.o -ffreestanding -nostdlib
	@gcc -m32 -c init.c -o init.o -ffreestanding -nostdlib
	@gcc -m32 -c lib/stdio.c -o stdio.o -ffreestanding -nostdlib
	@gcc -m32 -c lib/unistd.c -o unistd.o -ffreestanding -nostdlib
	@ld -T link.ld  -m elf_i386 -o init stdio.o init.o crt0.o unistd.o
	@../../../tools/diskedit.sh ../../../src/disk.img -m
	@cp init /mnt
	@../../../tools/diskedit.sh ../../../src/disk.img -u

clean:
	rm -f crt0.o init.o stdio.o unistd.o
