/* c runtime stage 0 */

extern int main();

void sys_exit(int status)
{
    // x86_32 bit syscall for exiting the program
    asm volatile (
        "movl $1, %%eax \n"
        "movl %0, %%ebx \n"
        "int $0x80 \n"
        :
        : "r"(status)
        : "%eax", "%ebx"
    );
}

void _start(int argc, char * argv[])
{
  int ret = main(argc, argv);
  sys_exit(ret);
  /* wait for program termination */
  for(;;);
}