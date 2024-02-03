<h1 align=center>Lunaris</h1>
<p align=center>The light of the moon</p>

![image](https://github.com/deyzi-the-youtuber/lunaris/blob/main/assets/luna.svg)

**What is Lunaris?**\
Lunaris is a UNIX-like kernel that is written completely in C.\
Lunaris takes inspiration from Linux, and the Fiwix kernel.

Lunaris might just be the new Linux 👀 (if it weren't so unstable)

## Features
Current features for v0.1 are
- x86 support
- IDE support
- PCI device support
- TTY support
- Basic VGA text mode support
- VFS support
- Ext2 support
- Devfs psuedo-filesystem support
- Memory allocation
- Kernel stack tracing
- Paging support
- Multitasking
- System call support
- ACPI 2.0 Support

## Compiling the Lunaris kernel
To compile the Lunaris kernel, you need to install the following:
- Unix-like system / environment
- Binutils
- NASM
- GCC
- grub-mkrescue (needed for creating ISO image)

After installing, run 'make', and the kernel image will be built.\
To get kernel symbols working, go into any file of the C source files, make a small change, then revert the change, and recompile. \
This should allow kernel symbols to be used in the kernel.

## Licensing
Lunaris is licensed under the GNU General Public License version 2. For more information, see LICENSE.
